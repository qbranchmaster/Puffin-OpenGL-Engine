//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_POSTPROCESS_RENDERER_H
#define PUFFIN_POSTPROCESS_RENDERER_H

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "Puffin/Common/Logger.h"
#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Manager/FrameBufferManager.h"
#include "Puffin/Manager/MasterManager.h"

namespace puffin
{
    enum class PostprocessEffect
    {
        NONE,
        NEGATIVE,
        GRAYSCALE,
        SHARPEN,
        BLUR,
        EDGE,
        TINT,
    };

    class PostprocessRenderer
    {
        friend class MasterRenderer;

    public:
        PostprocessRenderer(MasterManagerPtr master_manager,
            StateMachinePtr state_machine,
            DisplayConfigurationPtr display_configuration,
            FrameBufferManagerPtr frame_buffer_manager);
        virtual ~PostprocessRenderer();

        void setEffect(PostprocessEffect effect)
        {
            effect_ = effect;
        }

        void setKernelSize(GLfloat size)
        {
            if (size <= 0.0f)
                logErrorAndThrow(name_, "PostprocessRenderer::setKernelSize()",
                    "Kernel size value out of range: {0 < VALUE}.");

            kernel_size_ = size;
        }

        GLfloat getKernelSize() const
        {
            return kernel_size_;
        }

        void setTintColor(const glm::vec3 &color)
        {
            tint_color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getTintColor() const
        {
            return tint_color_;
        }

    protected:
        void loadShaders();
        void setShaderUniforms();
        void createFrameBuffers();

        FrameBufferPtr getFrameBufferToRender() const;

        std::string name_{"core_postprocess_renderer"};

        DisplayConfigurationPtr display_configuration_{nullptr};
        FrameBufferManagerPtr frame_buffer_manager_{nullptr};
        MasterManagerPtr master_manager_{nullptr};
        ShaderProgramPtr shader_program_{nullptr};
        StateMachinePtr state_machine_{nullptr};

        FrameBufferPtr frame_buffer_multisample_{nullptr};
        FrameBufferPtr frame_buffer_simple_{nullptr};

        PostprocessEffect effect_{PostprocessEffect::NONE};
        GLfloat kernel_size_{300.0f};
        glm::vec3 tint_color_{1.0f, 1.0f, 1.0f};
    };

    using PostprocessRendererPtr = std::shared_ptr<PostprocessRenderer>;
} // namespace puffin

#endif // PUFFIN_POSTPROCESS_RENDERER_H