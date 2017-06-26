//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Renderer/FontRenderer.h"

using namespace puffin;

FontRenderer::FontRenderer(MasterManagerPtr master_manager,
    StateMachinePtr state_machine,
    DisplayConfigurationPtr display_configuration) :
    BaseRenderer("core_font_renderer")
{
    if (!master_manager)
        logErrorAndThrow(name_, "FontRenderer::FontRenderer()",
            "Object [MasterManager] pointer not set.");

    if (!state_machine)
        logErrorAndThrow(name_, "FontRenderer::FontRenderer()",
            "Object [StateMachine] pointer not set.");

    if (!display_configuration)
        logErrorAndThrow(name_, "FontRenderer::FontRenderer()",
            "Object [DisplayConfiguration] pointer not set.");

    display_configuration_ = display_configuration;
    state_machine_ = state_machine;
    master_manager_ = master_manager;

    if (FT_Init_FreeType(&ft_library_))
        logErrorAndThrow(name_, "FontRenderer::FontRenderer()",
            "FreeType library initialization error.");

    if (FT_Stroker_New(ft_library_, &stroker_))
        logErrorAndThrow(name_, "FontRenderer::FontRenderer()",
            "FreeType stroker initialization error.");

    loadShaders();
    createCharacterModel();

    logDebug(name_, "FontRenderer::FontRenderer()", "Font renderer created");
}

FontRenderer::~FontRenderer()
{
    for (auto &face : font_faces_)
        FT_Done_Face(face.second);

    FT_Done_FreeType(ft_library_);

    logDebug(name_, "FontRenderer::~FontRenderer()", "Font renderer destroyed");
}

void FontRenderer::loadShaders()
{
    shader_program_ = master_manager_->shaderManager()->
        createShaderProgram("font_shader_program", "shaders/FontVs.glsl",
            "shaders/FontFs.glsl");
}

void FontRenderer::createCharacterModel()
{
    character_texture_ = master_manager_->textureManager()->
        createTexture2D("single_character_texture");
    master_manager_->textureManager()->setTextureFilter(character_texture_,
        TextureFilter::BILINEAR);

    std::vector<GLfloat> texture_coords = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    character_object_ = master_manager_->meshManager()->
        createObject3D("single_character");

    Object3DEntityPtr entity(new Object3DEntity());
    entity->setVerticesCount(6);
    character_object_->addEntity(entity);

    master_manager_->meshManager()->setMeshData(character_object_,
        texture_coords, VertexDataType::TEXTURE_COORD, false);
}

FT_Face FontRenderer::createFontFace(TextPtr text)
{
    for (const auto &font_face : font_faces_)
    {
        if (font_face.first == text->getFont())
            return font_face.second;
    }

    FT_Face font_face{};
    FT_Error result = FT_New_Face(ft_library_, text->getFont().c_str(), 0,
        &font_face);

    if (result == FT_Err_Unknown_File_Format)
        logErrorAndThrow(name_, "FontRenderer::createFontFace()",
            "Unknown font file format.");
    else if (result)
        logErrorAndThrow(name_, "FontRenderer::createFontFace()",
            "Opening font file [" + text->getFont() + "] error.");

    logInfo(name_, "FontRenderer::createFontFace()",
        "Font face for font [" + text->getFont() + "] created.");

    font_faces_[text->getFont()] = font_face;
    return font_face;
}

GLint FontRenderer::processWhitespaces(FT_Face font_face, wchar_t character,
    TextPtr text, GLint &cur_pos_x, GLint &cur_pos_y) const
{
    if (character == ' ')
    {
        if (render_type_ == TextRenderType::NO_OUTLINE ||
            render_type_ == TextRenderType::OUTLINE)
            cur_pos_x += (font_face->glyph->advance.x >> 6) +
            text->getHorizontalSpacing();

        return -1;
    }

    if (character == '\n')
    {
        if (render_type_ == TextRenderType::NO_OUTLINE ||
            render_type_ == TextRenderType::OUTLINE)
        {
            cur_pos_x = text->getPosition().x;
            cur_pos_y += text->getFontSize() + text->getVerticalSpacing();
        }

        return -1;
    }

    return 0;
}

FT_BitmapGlyph FontRenderer::getCharacterGlyph(FT_Face font_face,
    wchar_t character, GLint outline_size) const
{
    FT_UInt glyph_index = FT_Get_Char_Index(font_face, character);
    FT_Load_Glyph(font_face, glyph_index, FT_LOAD_DEFAULT);

    FT_Glyph glyph;
    FT_Get_Glyph(font_face->glyph, &glyph);

    if (render_type_ == TextRenderType::OUTLINE)
    {
        FT_Stroker_Set(stroker_, static_cast<FT_Fixed>(64 * outline_size),
            FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
        FT_Glyph_StrokeBorder(&glyph, stroker_, false, true);
    }

    if (FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true))
        logErrorAndThrow(name_, "FontRenderer::getCharacterGlyph()",
            "Cannot get character bitmap glyph.");

    FT_BitmapGlyph bitmap_glyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
    return bitmap_glyph;
}

void FontRenderer::render(ScenePtr scene)
{
    if (!scene)
        return;

    auto text_container = scene->getTextContainer();
    if (!text_container.size())
        return;

    state_machine_->activateShaderProgram(shader_program_);

    master_manager_->textureManager()->setUnpackPixelAlignment(1);
    master_manager_->textureManager()->setTextureSlot(0);
    state_machine_->unbindAllTextures();

    state_machine_->bindTexture(character_texture_);
    state_machine_->bindMesh(character_object_);

    state_machine_->alphaBlend()->enable(true);
    state_machine_->depthTest()->enable(false);
    state_machine_->alphaBlend()->setBlendFunction(BlendFunction::NORMAL);

    for (const auto &text : text_container)
    {
        auto font_face = createFontFace(text);
        FT_Set_Pixel_Sizes(font_face, 0, text->getFontSize());

        GLint cursor_pos_x = text->getPosition().x;
        GLint cursor_pos_y = text->getPosition().y;

        for (const auto &character : text->getText())
        {
            GLint cursor_x = cursor_pos_x;
            GLint cursor_y = cursor_pos_y;

            if (text->getOutlineSize() > 0)
            {
                render_type_ = TextRenderType::OUTLINE;
                renderSingleCharacter(font_face, character, text, cursor_x,
                    cursor_y);
                render_type_ = TextRenderType::TEXT;
            }
            else
                render_type_ = TextRenderType::NO_OUTLINE;

            renderSingleCharacter(font_face, character, text, cursor_pos_x,
                cursor_pos_y);

            if (render_type_ == TextRenderType::TEXT)
            {
                cursor_pos_x = cursor_x;
                cursor_pos_y = cursor_y;
            }
        }
    }

    master_manager_->textureManager()->setUnpackPixelAlignment(4);
}

std::vector<glm::vec3> FontRenderer::calculateVertices(GLint cursor_x,
    GLint cursor_y, FT_BitmapGlyph bitmap) const
{
    std::vector<glm::vec3> vertices(4);

    vertices[0] = glm::vec3(cursor_x + bitmap->left, cursor_y - bitmap->top,
        0.0f);
    vertices[1] = glm::vec3(vertices[0].x, vertices[0].y + bitmap->bitmap.rows,
        0.0f);
    vertices[2] = glm::vec3(vertices[0].x + bitmap->bitmap.width, vertices[0].y,
        0.0f);
    vertices[3] = glm::vec3(vertices[2].x, vertices[1].y, 0.0f);

    return vertices;
}

void FontRenderer::setShaderUniforms(TextPtr text) const
{
    master_manager_->shaderManager()->setUniform(shader_program_,
        "color.font_texture", static_cast<GLint>(0));

    if (render_type_ == TextRenderType::OUTLINE)
        master_manager_->shaderManager()->setUniform(shader_program_,
            "color.font_color", text->getOutlineColor());
    else
        master_manager_->shaderManager()->setUniform(shader_program_,
            "color.font_color", text->getFontColor());
}

void FontRenderer::renderSingleCharacter(FT_Face font_face, wchar_t character,
    TextPtr text, GLint &cur_pos_x, GLint &cur_pos_y)
{
    if (processWhitespaces(font_face, character, text, cur_pos_x, cur_pos_y))
        return;

    FT_BitmapGlyph bitmap_glyph = getCharacterGlyph(font_face, character,
        text->getOutlineSize());
    master_manager_->textureManager()->setTexture2DData(character_texture_,
        bitmap_glyph->bitmap.buffer, bitmap_glyph->bitmap.width,
        bitmap_glyph->bitmap.rows, 1);

    auto vertices = calculateVertices(cur_pos_x, cur_pos_y, bitmap_glyph);

    for (auto &vertex : vertices)
    {
        vertex.x = calculateScreenCoordX(vertex.x);
        vertex.y = calculateScreenCoordY(vertex.y);
    }

    std::vector<GLfloat> vertices_buffer;

    for (const auto &index : {0, 1, 2, 2, 1, 3})
    {
        vertices_buffer.push_back(vertices[index].x);
        vertices_buffer.push_back(vertices[index].y);
        vertices_buffer.push_back(vertices[index].z);
    }

    master_manager_->meshManager()->setMeshData(character_object_,
        vertices_buffer, VertexDataType::POSITION, true);

    setShaderUniforms(text);
    character_object_->draw();

    if (render_type_ == TextRenderType::NO_OUTLINE ||
        render_type_ == TextRenderType::OUTLINE)
        cur_pos_x += (font_face->glyph->advance.x >> 6) +
        text->getHorizontalSpacing();
}