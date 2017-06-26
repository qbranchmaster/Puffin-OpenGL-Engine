//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_FRAME_BUFFER_H
#define PUFFIN_FRAME_BUFFER_H

#include <GL/glew.h>

#include <memory>

#include "Puffin/Common/Logger.h"
#include "Puffin/Texture/Texture.h"

namespace puffin
{
    class FrameBuffer
    {
        friend class FrameBufferManager;
        friend class StateMachine;

    public:
        explicit FrameBuffer(std::string name = "");
        virtual ~FrameBuffer();

        std::string getName() const
        {
            return name_;
        }

        TexturePtr getRgbTextureBuffer() const
        {
            return rgb_buffer_texture_;
        }

        TexturePtr getDepthTextureBuffer() const
        {
            return depth_buffer_texture_;
        }

        TexturePtr getCubeTextureBuffer() const
        {
            return cube_buffer_texture_;
        }

    protected:
        std::string name_{"unnamed_frame_buffer"};

        GLuint handle_{0};

        // Texture buffers
        TexturePtr rgb_buffer_texture_{nullptr};
        TexturePtr depth_buffer_texture_{nullptr};
        TexturePtr cube_buffer_texture_{nullptr};

        // Render buffers
        struct RenderBuffer
        {
            GLuint handle;
            GLint width;
            GLint height;
        };

        RenderBuffer depth_stencil_{};
    };

    using FrameBufferPtr = std::shared_ptr<FrameBuffer>;
} // namespace puffin

#endif // PUFFIN_FRAME_BUFFER_H