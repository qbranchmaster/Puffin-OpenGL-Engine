//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_TEXTURE_H
#define PUFFIN_TEXTURE_H

#include <GL/glew.h>

#include <memory>

#include "Puffin/Common/Logger.h"
#include "Puffin/Texture/TextureType.h"

namespace puffin
{
    class Texture
    {
        friend class FrameBufferManager;
        friend class StateMachine;
        friend class TextureManager;

    public:
        Texture(TextureType texture_type, GLint width, GLint height,
            GLint color_channels, std::string path, GLubyte *data,
            std::string name = "");
        virtual ~Texture();

        std::string getName() const
        {
            return name_;
        }

        std::string getPath() const
        {
            return path_;
        }

        GLint getWidth() const
        {
            if (type_ == TextureType::TEXTURE_CUBE)
            {
                logWarning(name_, "Texture::getWidth()",
                    "Cannot get width from cube texture.");
                return 0;
            }

            return width_;
        }

        GLint getHeight() const
        {
            if (type_ == TextureType::TEXTURE_CUBE)
            {
                logWarning(name_, "Texture::getHeight()",
                    "Cannot get height from cube texture.");
                return 0;
            }

            return height_;
        }

        GLint getColorChannelsCount() const
        {
            return color_channels_;
        }

        TextureType getType() const
        {
            return type_;
        }

    protected:
        std::string name_{"unnamed_texture"};

        GLint width_{0};
        GLint height_{0};
        GLint color_channels_{0};
        GLubyte *image_data_{nullptr};
        std::string path_{};

        GLuint handle_{0};
        GLboolean has_mipmap_{false};

        TextureType type_{TextureType::TEXTURE_UNDEFINED};
    };

    using TexturePtr = std::shared_ptr<Texture>;
} // namespace puffin

#endif // PUFFIN_TEXTURE_H