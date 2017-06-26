//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Manager/TextureManager.h"

using namespace puffin;

TextureManager::TextureManager(StateMachinePtr state_machine,
    DisplayConfigurationPtr display_configuration) :
    BaseManager("core_texture_manager")
{
    if (!state_machine)
        logErrorAndThrow(name_, "TextureManager::TextureManager()",
            "Object [StateMachine] pointer not set.");

    if (!display_configuration)
        logErrorAndThrow(name_, "TextureManager::TextureManager()",
            "Object [DisplayConfiguration] pointer not set.");

    state_machine_ = state_machine;
    display_configuration_ = display_configuration;

    setDefaultTextureFilter();

    logDebug(name_, "TextureManager::TextureManager()",
        "Texture manager created.");
}

TextureManager::~TextureManager()
{
    logDebug(name_, "TextureManager::~TextureManager()",
        "Texture manager destroyed.");
}

void TextureManager::setDefaultTextureFilter()
{
    default_texture_filter_[TextureType::TEXTURE_2D] = TextureFilter::
        BILINEAR;
    default_texture_filter_[TextureType::TEXTURE_2D_MULTISAMPLED] =
        TextureFilter::BILINEAR;
    default_texture_filter_[TextureType::TEXTURE_CUBE] =
        TextureFilter::BILINEAR;
}

TexturePtr TextureManager::loadTextureCube(std::array<std::string, 6> textures,
    std::string texture_name)
{
    // No checking if cube texture already exists - it is difficult to
    // compare textures' paths

    TexturePtr texture(new Texture(TextureType::TEXTURE_CUBE, 0, 0, 3, "",
        nullptr, texture_name));

    state_machine_->bindTexture(texture);

    for (GLint i = 0; i < 6; i++)
    {
        GLint height = 0;
        GLint width = 0;
        GLint channels = 0;

        GLubyte *image = SOIL_load_image(textures[i].c_str(), &width, &height,
            &channels, SOIL_LOAD_RGB);
        if (!image)
            logErrorAndThrow(name_, "TextureManager::loadTextureCube()",
                "Texture [" + textures[i] + "] loading error.");

        logInfo(name_, "TextureManager::loadTextureCube()",
            "Texture [" + textures[i] + "] loaded.");

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
            height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }

    setTextureFilter(texture, default_texture_filter_[texture->getType()]);
    setTextureWrap(texture, TextureWrap::CLAMP_TO_EDGE);

    texture_container_.push_back(texture);
    return texture;
}

void TextureManager::setTextureWrap(TexturePtr texture,
    TextureWrap wrap_mode) const
{
    state_machine_->bindTexture(texture);

    switch (texture->getType())
    {
    case TextureType::TEXTURE_CUBE:
        switch (wrap_mode)
        {
        case TextureWrap::CLAMP_TO_EDGE:
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_EDGE);
            break;
        default:
            logWarning(name_, "TextureManager::setTextureWrap()",
                "Not supported texture wrap for cube texture.");
            break;
        }
        break;
    case TextureType::TEXTURE_2D:
    case TextureType::TEXTURE_2D_MULTISAMPLED:
        switch (wrap_mode)
        {
        case TextureWrap::REPEAT:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case TextureWrap::CLAMP_TO_BORDER:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_BORDER);
            break;
        case TextureWrap::CLAMP_TO_EDGE:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_EDGE);
            break;
        }
        break;
    }
}

TexturePtr TextureManager::createTexture2D(std::string texture_name)
{
    TexturePtr texture(new Texture(TextureType::TEXTURE_2D, 0, 0, 3, "",
        nullptr, texture_name));

    state_machine_->bindTexture(texture);
    setTextureFilter(texture, default_texture_filter_[texture->getType()]);
    setTextureWrap(texture, TextureWrap::CLAMP_TO_EDGE);

    texture_container_.push_back(texture);
    return texture;
}

void TextureManager::setTexture2DData(TexturePtr texture, GLubyte *data,
    GLint width, GLint height, GLint color_channels)
{
    if (texture->getType() != TextureType::TEXTURE_2D)
        logErrorAndThrow(name_, "TextureManager::setTexture2DData()",
            "Invalid texture type.");

    if (width <= 0 || height <= 0)
        logErrorAndThrow(name_, "TextureManager::setTexture2DData()",
            "Texture size value out of range: {0 < VALUE}.");

    if (color_channels != 1 && color_channels != 3)
        logErrorAndThrow(name_, "TextureManager::setTexture2DData()",
            "Texture color channels value out of range: {VALUE = [1, 3]}.");

    GLint channels = (color_channels == 1) ? GL_RED : GL_RGB;

    state_machine_->bindTexture(texture);

    glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels,
        GL_UNSIGNED_BYTE, data);

    texture->width_ = width;
    texture->height_ = height;
    texture->color_channels_ = channels;
}

void TextureManager::setTexture2DBorderColor(TexturePtr texture,
    const glm::vec4 &color) const
{
    if (texture->getType() != TextureType::TEXTURE_2D)
    {
        logWarning(name_, "TextureManager::setTexture2DBorderColor()",
            "Invalid texture type.");
        return;
    }

    state_machine_->bindTexture(texture);

    GLfloat border_color[] = {glm::clamp(color.r, 0.0f, 1.0f),
        glm::clamp(color.g, 0.0f, 1.0f),
        glm::clamp(color.b, 0.0f, 1.0f),
        glm::clamp(color.a, 0.0f, 1.0f)};

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
}

void TextureManager::setTextureFilter(TexturePtr texture,
    TextureFilter texture_filter) const
{
    state_machine_->bindTexture(texture);

    switch (texture->getType())
    {
    case TextureType::TEXTURE_CUBE:
        switch (texture_filter)
        {
        case TextureFilter::BILINEAR:
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR);
            break;
        }
        break;
    default:
        logWarning(name_, "TextureManager::setTextureFilter()",
            "Not supported texture filter for cube texture.");
        break;
    case TextureType::TEXTURE_2D:
    case TextureType::TEXTURE_2D_MULTISAMPLED:
        switch (texture_filter)
        {
        case TextureFilter::NEAREST:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case TextureFilter::BILINEAR:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case TextureFilter::BILINEAR_WITH_MIPMAPS:
            generateTextureMipmap(texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case TextureFilter::TRILINEAR:
            generateTextureMipmap(texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        }
        break;
    }
}

void TextureManager::generateTextureMipmap(TexturePtr texture) const
{
    if (!texture->has_mipmap_)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        texture->has_mipmap_ = true;
    }
}

void TextureManager::freeTexture2DRawData(TexturePtr texture) const
{
    if (texture->image_data_ != nullptr)
    {
        SOIL_free_image_data(texture->image_data_);
        texture->image_data_ = nullptr;
    }
    else
        logWarning(name_, "TextureManager::freeTextureRawData()",
            "Texture raw data is unavailable.");
}

TexturePtr TextureManager::loadTexture2D(std::string path, GLboolean auto_free,
    std::string texture_name)
{
    if (path.empty())
        logErrorAndThrow(name_, "TextureManager::loadTexture2D()",
            "Empty texture file path.");

    // Search in loaded textures; if texture already exists return it
    for (const auto &texture : texture_container_)
    {
        if (texture->getPath() == path)
            return texture;
    }

    GLint height = 0;
    GLint width = 0;
    GLint channels = 0;

    GLubyte *image_data = SOIL_load_image(path.c_str(), &width, &height,
        &channels, SOIL_LOAD_AUTO);
    if (!image_data)
        logErrorAndThrow(name_, "TextureManager::loadTexture2D()",
            "Loading texture [" + path + "] error.");

    logInfo(name_, "TextureManager::loadTexture2D()",
        "Texture [" + path + "] loaded.");

    TexturePtr texture(new Texture(TextureType::TEXTURE_2D, width, height,
        channels, path, image_data, texture_name));

    state_machine_->bindTexture(texture);

    glTexImage2D(GL_TEXTURE_2D, 0, channels == 3 ? GL_RGB : GL_RGBA, width,
        height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE,
        image_data);

    setTextureFilter(texture, default_texture_filter_[texture->getType()]);
    setTextureWrap(texture, TextureWrap::REPEAT);

    if (auto_free)
        freeTexture2DRawData(texture);

    texture_container_.push_back(texture);
    return texture;
}

TexturePtr TextureManager::createTextureCube(GLint size,
    std::string texture_name)
{
    if (size <= 0)
        logErrorAndThrow(name_, "TextureManager::createTextureCube()",
            "Texture size value out of range: {0 < VALUE}.");

    TexturePtr texture(new Texture(TextureType::TEXTURE_CUBE, 0, 0, 3, "",
        nullptr, texture_name));

    state_machine_->bindTexture(texture);

    for (GLint i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
            size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    setTextureFilter(texture, default_texture_filter_[texture->getType()]);
    setTextureWrap(texture, TextureWrap::CLAMP_TO_EDGE);

    texture_container_.push_back(texture);
    return texture;
}

TexturePtr TextureManager::createTextureDepthBuffer(GLint width, GLint height,
    std::string texture_name)
{
    if (width <= 0 || height <= 0)
        logErrorAndThrow(name_, "TextureManager::createTextureDepthBuffer()",
            "Texture size value out of range: {0 < VALUE}.");

    TexturePtr texture(new Texture(TextureType::TEXTURE_2D, width, height, 1,
        "", nullptr, texture_name));

    state_machine_->bindTexture(texture);

    // GL_DEPTH_COMPONENT32 is an overkill, so this buffer uses
    // GL_DEPTH_COMPONENT. It means that buffer's size is chosen by
    // OpenGL implementation.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    setTextureFilter(texture, default_texture_filter_[texture->getType()]);
    setTextureWrap(texture, TextureWrap::CLAMP_TO_BORDER);
    setTexture2DBorderColor(texture, glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

    texture_container_.push_back(texture);
    return texture;
}

TexturePtr TextureManager::createTextureRgbBuffer(GLint width, GLint height,
    GLboolean multisample, std::string texture_name)
{
    if (width <= 0 || height <= 0)
        logErrorAndThrow(name_, "TextureManager::createTextureRgbBuffer()",
            "Texture size value out of range: {0 < VALUE}.");

    TextureType type = multisample ? TextureType::TEXTURE_2D_MULTISAMPLED :
        TextureType::TEXTURE_2D;

    TexturePtr texture(new Texture(type, width, height, 3, "", nullptr,
        texture_name));

    state_machine_->bindTexture(texture);

    if (multisample)
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
            display_configuration_->getMsaaSamples(), GL_RGB, width, height,
            GL_TRUE);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, nullptr);

    setTextureFilter(texture, default_texture_filter_[texture->getType()]);
    setTextureWrap(texture, TextureWrap::REPEAT);

    texture_container_.push_back(texture);
    return texture;
}

void TextureManager::setTextureSlot(GLint slot_index)
{
    if (slot_index < 0)
        logErrorAndThrow(name_, "TextureManager::setTextureSlot()",
            "Texture slot value out of range: {0 < VALUE}.");

    glActiveTexture(GL_TEXTURE0 + slot_index);
}

void TextureManager::setUnpackPixelAlignment(GLint alignment)
{
    if (alignment != 1 && alignment != 2 && alignment != 4 &&
        alignment != 8)
        logErrorAndThrow(name_, "TextureManager::setUnpackPixelAlignment()",
            "Invalid alignment value. Allowed values: {1, 2, 4, 8}.");

    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
}