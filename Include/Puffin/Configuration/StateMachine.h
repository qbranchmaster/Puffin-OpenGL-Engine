//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_STATE_MACHINE_H
#define PUFFIN_STATE_MACHINE_H

#include <GL/glew.h>

#include <map>
#include <memory>

#include "Puffin/Common/Logger.h"
#include "Puffin/Configuration/AlphaBlend.h"
#include "Puffin/Configuration/DepthTest.h"
#include "Puffin/Configuration/FaceCull.h"
#include "Puffin/Mesh/BaseMesh.h"
#include "Puffin/Renderer/FrameBuffer.h"
#include "Puffin/Shader/ShaderProgram.h"
#include "Puffin/Texture/Texture.h"

namespace puffin
{
    enum class FrameBufferBindType
    {
        NORMAL,
        ONLY_READ,
        ONLY_WRITE,
    };


    class StateMachine
    {
    public:
        StateMachine()
        {
            alpha_blend_.reset(new AlphaBlend());
            depth_test_.reset(new DepthTest());
            face_cull_.reset(new FaceCull());

            logDebug(name_, "StateMachine::StateMachine()",
                "State machine created.");
        }

        virtual ~StateMachine()
        {
            logDebug(name_, "StateMachine::~StateMachine()",
                "State machine destroyed.");
        }

        std::string getName() const
        {
            return name_;
        }

        AlphaBlendPtr alphaBlend() const
        {
            return alpha_blend_;
        }

        DepthTestPtr depthTest() const
        {
            return depth_test_;
        }

        FaceCullPtr faceCulling() const
        {
            return face_cull_;
        }

        void activateShaderProgram(ShaderProgramPtr shader_program)
        {
            if (!shader_program)
                logErrorAndThrow(name_, "StateMachine::activateShaderProgram()",
                    "Object [ShaderProgram] pointer not set.");

            if (active_shader_program_ && shader_program->handle_ ==
                active_shader_program_->handle_)
                return;

            glUseProgram(shader_program->handle_);
            active_shader_program_ = shader_program;
        }

        void bindTexture(TexturePtr texture)
        {
            if (!texture)
                logErrorAndThrow(name_, "StateMachine::bindTexture()",
                    "Object [Texture] pointer not set.");

            if (bound_texture_ && texture->handle_ == bound_texture_->handle_)
                return;

            switch (texture->getType())
            {
            case TextureType::TEXTURE_CUBE:
                glBindTexture(GL_TEXTURE_CUBE_MAP, texture->handle_);
                break;
            case TextureType::TEXTURE_2D:
                glBindTexture(GL_TEXTURE_2D, texture->handle_);
                break;
            case TextureType::TEXTURE_2D_MULTISAMPLED:
                glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture->handle_);
            }

            bound_texture_ = texture;
        }

        void unbindTexture(TextureType texture_type)
        {
            if (bound_texture_ == nullptr)
                return;

            switch (texture_type)
            {
            case TextureType::TEXTURE_CUBE:
                glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
                break;
            case TextureType::TEXTURE_2D:
                glBindTexture(GL_TEXTURE_2D, 0);
                break;
            case TextureType::TEXTURE_2D_MULTISAMPLED:
                glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
            }

            bound_texture_ = nullptr;
        }

        void unbindAllTextures()
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

            bound_texture_ = nullptr;
        }

        void bindMesh(BaseMeshPtr mesh)
        {
            if (!mesh)
                logErrorAndThrow(name_, "StateMachine::bindMesh()",
                    "Object [BaseMesh] pointer not set.");

            if (bound_mesh_ && mesh->handle_ == bound_mesh_->handle_)
                return;

            glBindVertexArray(mesh->handle_);
            bound_mesh_ = mesh;
        }

        void unbindMesh()
        {
            if (!bound_mesh_)
                return;

            glBindVertexArray(0);
            bound_mesh_ = nullptr;
        }

        void bindFrameBuffer(FrameBufferPtr frame_buffer,
            FrameBufferBindType bind_type = FrameBufferBindType::NORMAL)
        {
            if (!frame_buffer)
                logErrorAndThrow(name_, "StateMachine::bindFrameBuffer()",
                    "Object [FrameBuffer] pointer not set.");

            switch (bind_type)
            {
            case FrameBufferBindType::NORMAL:
                if (bound_frame_buffer_ && frame_buffer->handle_ ==
                    bound_frame_buffer_->handle_)
                    return;

                glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->handle_);
                bound_frame_buffer_ = frame_buffer;
                break;
            case FrameBufferBindType::ONLY_READ:
                if (bound_only_read_ && frame_buffer->handle_ ==
                    bound_only_read_->handle_)
                    return;

                glBindFramebuffer(GL_READ_FRAMEBUFFER, frame_buffer->handle_);
                bound_only_read_ = frame_buffer;
                break;
            case FrameBufferBindType::ONLY_WRITE:
                if (bound_only_write_ && frame_buffer->handle_ ==
                    bound_only_write_->handle_)
                    return;

                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer->handle_);
                bound_only_write_ = frame_buffer;
                break;
            }
        }

        void unbindFrameBuffer()
        {
            if (!bound_frame_buffer_)
                return;

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            bound_frame_buffer_ = nullptr;
            bound_only_read_ = nullptr;
            bound_only_write_ = nullptr;
        }

    protected:
        std::string name_{"core_state_machine"};

        BaseMeshPtr bound_mesh_{nullptr};
        TexturePtr bound_texture_{nullptr};
        ShaderProgramPtr active_shader_program_{nullptr};

        FrameBufferPtr bound_frame_buffer_{nullptr};
        FrameBufferPtr bound_only_read_{nullptr};
        FrameBufferPtr bound_only_write_{nullptr};

        AlphaBlendPtr alpha_blend_{nullptr};
        DepthTestPtr depth_test_{nullptr};
        FaceCullPtr face_cull_{nullptr};
    };

    using StateMachinePtr = std::shared_ptr<StateMachine>;
} // namespace puffin

#endif // PUFFIN_STATE_MACHINE_H