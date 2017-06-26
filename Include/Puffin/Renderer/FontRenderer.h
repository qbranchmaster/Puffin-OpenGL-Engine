//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_FONT_RENDERER_H
#define PUFFIN_FONT_RENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>

#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Manager/MasterManager.h"
#include "Puffin/Mesh/Object3D.h"
#include "Puffin/Renderer/BaseRenderer.h"
#include "Puffin/UI/Text.h"


namespace puffin
{
    class FontRenderer : public BaseRenderer
    {
        friend class MasterRenderer;

    public:
        FontRenderer(MasterManagerPtr master_manager,
            StateMachinePtr state_machine,
            DisplayConfigurationPtr display_configuration);
        virtual ~FontRenderer();

    protected:
        void render(ScenePtr scene);

        void loadShaders();
        void setShaderUniforms(TextPtr text) const;
        void createCharacterModel();

        FT_Face createFontFace(TextPtr text);
        FT_BitmapGlyph getCharacterGlyph(FT_Face font_face, wchar_t character,
            GLint outline_size) const;

        FT_Library ft_library_{};
        FT_Stroker stroker_{};
        std::unordered_map<std::string, FT_Face> font_faces_;

        void renderSingleCharacter(FT_Face font_face, wchar_t character,
            TextPtr text, GLint &cur_pos_x, GLint &cur_pos_y);
        GLint processWhitespaces(FT_Face font_face, wchar_t character,
            TextPtr text, GLint &cur_pos_x, GLint &cur_pos_y) const;
        std::vector<glm::vec3> calculateVertices(GLint cursor_x, GLint cursor_y,
            FT_BitmapGlyph bitmapGlyph) const;

        Object3DPtr character_object_{nullptr};
        ShaderProgramPtr shader_program_{nullptr};
        TexturePtr character_texture_{nullptr};

        GLfloat calculateScreenCoordX(GLfloat x)
        {
            static auto v_width = display_configuration_->getWidth();
            return (x / static_cast<GLfloat>(v_width) * 2.0f - 1.0f);
        }

        GLfloat calculateScreenCoordY(GLfloat y)
        {
            static auto v_height = display_configuration_->getHeight();
            return (1.0f - y / static_cast<GLfloat>(v_height) * 2.0f);
        }

        DisplayConfigurationPtr display_configuration_{nullptr};
        StateMachinePtr state_machine_{nullptr};
        MasterManagerPtr master_manager_{nullptr};

        enum class TextRenderType
        {
            NO_OUTLINE,
            OUTLINE,
            TEXT,
        };

        TextRenderType render_type_{TextRenderType::NO_OUTLINE};
    };

    using FontRendererPtr = std::shared_ptr<FontRenderer>;
} // namespace puffin

#endif // PUFFIN_FONT_RENDERER_H