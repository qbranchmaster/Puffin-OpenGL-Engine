//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Texture/Texture.h"

using namespace puffin;

Texture::Texture(TextureType texture_type, GLint width, GLint height,
    GLint color_channels, std::string path, GLubyte *data, std::string name)
{
    if (!name.empty())
        name_ = name;

    if (width < 0 || height < 0)
        logErrorAndThrow(name_, "Texture::Texture()",
            "Texture size value out of range: {0 <= VALUE}.");

    if (color_channels <= 0)
        logErrorAndThrow(name_, "Texture::Texture()",
            "Color channels value out of range: {0 < VALUE}.");

    path_ = path;
    type_ = texture_type;

    width_ = width;
    height_ = height;
    color_channels_ = color_channels;

    image_data_ = data;

    glGenTextures(1, &handle_);

    logDebug(name_, "Texture::Texture()", "Texture created.");
}

Texture::~Texture()
{
    if (handle_)
        glDeleteTextures(1, &handle_);

    logDebug(name_, "Texture::~Texture()", "Texture destroyed.");
}