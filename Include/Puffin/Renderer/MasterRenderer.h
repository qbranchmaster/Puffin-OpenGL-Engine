//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_MASTER_RENDERER_H
#define PUFFIN_MASTER_RENDERER_H

#include <GL/glew.h>

#include <functional>

#include "Puffin/Camera/Camera.h"
#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Display/Display.h"
#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Manager/MasterManager.h"
#include "Puffin/Mesh/Scene.h"
#include "Puffin/Renderer/Fog.h"
#include "Puffin/Renderer/FontRenderer.h"
#include "Puffin/Renderer/FpsCounter.h"
#include "Puffin/Renderer/Object3DRenderer.h"
#include "Puffin/Renderer/ParticleRenderer.h"
#include "Puffin/Renderer/PolygonMode.h"
#include "Puffin/Renderer/PostprocessRenderer.h"
#include "Puffin/Renderer/ShadowMapRenderer.h"
#include "Puffin/Renderer/SkyboxRenderer.h"
#include "Puffin/Renderer/StencilBuffer.h"
#include "Puffin/Renderer/WaterRenderer.h"

namespace puffin
{
    class MasterRenderer
    {
    public:
        MasterRenderer(MasterManagerPtr master_manager,
            StateMachinePtr state_machine, DisplayPtr display,
            DisplayConfigurationPtr display_configuration);
        virtual ~MasterRenderer();

        GLboolean isRenderingEnabled() const
        {
            return rendering_enabled_;
        }

        void start();
        void stop();
        void assignRenderingFunction(std::function<void()> function);

        PostprocessRendererPtr postprocessRenderer() const
        {
            return postprocess_renderer_;
        }

        MasterManagerPtr masterManager() const
        {
            return master_manager_;
        }

        FpsCounterPtr fpsCounter() const
        {
            return fps_counter_;
        }

        FogPtr fog() const
        {
            return fog_;
        }

        PolygonModePtr polygonMode() const
        {
            return polygon_mode_;
        }

        ShadowMapConfigurationPtr shadowMap() const
        {
            return shadow_map_;
        }

        void useCamera(CameraPtr camera)
        {
            if (!camera)
                logErrorAndThrow(name_, "MasterRenderer::useCamera()",
                    "Object [Camera] pointer not set.");

            active_camera_ = camera;

            model3d_renderer_->useCamera(active_camera_);
            skybox_renderer_->useCamera(active_camera_);
            shadow_map_renderer_->useCamera(active_camera_);
            water_renderer_->useCamera(active_camera_);
            particle_renderer_->useCamera(active_camera_);
        }

        void drawScene(ScenePtr scene);

    protected:
        void createScreenModel();

        void clear()
        {
            auto colour = target_display_->getBackgroundColor();
            glClearColor(colour.r, colour.g, colour.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT);

            glViewport(0, 0, display_configuration_->getWidth(),
                display_configuration_->getHeight());
        }

        std::string name_{"core_master_renderer"};

        GLboolean rendering_enabled_{false};
        std::function<void()> rendering_function_{nullptr};

        MasterManagerPtr master_manager_{nullptr};
        StateMachinePtr state_machine_{nullptr};
        DisplayPtr target_display_{nullptr};
        DisplayConfigurationPtr display_configuration_{nullptr};

        FogPtr fog_{nullptr};
        FpsCounterPtr fps_counter_{nullptr};
        PolygonModePtr polygon_mode_{nullptr};
        ShadowMapConfigurationPtr shadow_map_{nullptr};

        CameraPtr active_camera_{nullptr};
        Object3DPtr screen_{nullptr};

        FontRendererPtr font_renderer_{nullptr};
        Object3DRendererPtr model3d_renderer_{nullptr};
        ParticleRendererPtr particle_renderer_{nullptr};
        PostprocessRendererPtr postprocess_renderer_{nullptr};
        ShadowMapRendererPtr shadow_map_renderer_{nullptr};
        SkyboxRendererPtr skybox_renderer_{nullptr};
        WaterRendererPtr water_renderer_{nullptr};
    };

    using MasterRendererPtr = std::shared_ptr<MasterRenderer>;
} // namespace puffin

#endif // PUFFIN_MASTER_RENDERER_H