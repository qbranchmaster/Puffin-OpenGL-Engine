//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_WATER_RENDERER_H
#define PUFFIN_WATER_RENDERER_H

#include <GL/glew.h>

#include <memory>
#include <string>
#include <vector>

#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Manager/MasterManager.h"
#include "Puffin/Renderer/BaseRenderer.h"
#include "Puffin/Renderer/FpsCounter.h"
#include "Puffin/Renderer/Fog.h"
#include "Puffin/Renderer/Object3DRenderer.h"
#include "Puffin/Renderer/SkyboxRenderer.h"

namespace puffin
{
    class WaterRenderer : public BaseRenderer
    {
        friend class MasterRenderer;

    public:
        WaterRenderer(MasterManagerPtr master_manager,
            StateMachinePtr state_machine,
            DisplayConfigurationPtr display_configuration, FogPtr fog,
            FpsCounterPtr fps_counter, Object3DRendererPtr model3d_renderer,
            SkyboxRendererPtr skybox_renderer);
        virtual ~WaterRenderer();

        void setReflectionResolution(GLint width, GLint height);
        void setRefractionResolution(GLint width, GLint height);

    protected:
        void render(ScenePtr scene);
        void renderToFrameBuffers(ScenePtr scene);
        void renderReflectionTexture(WaterTilePtr water_tile, ScenePtr scene);
        void renderRefractionTexture(WaterTilePtr water_tile, ScenePtr scene);

        void loadShaders();
        void createFrameBuffers();
        void clearFrameBuffer(GLint width, GLint height) const;
        void setTextureUniforms(ShaderProgramPtr shader_program,
            WaterTilePtr water_tile);
        void setCameraMatricesUniforms(ShaderProgramPtr shader_program,
            WaterTilePtr water_tile);
        void setWaterTileUniforms(ShaderProgramPtr shader_program,
            WaterTilePtr water_tile);
        void setFogUniforms(ShaderProgramPtr shader_program);
        void setLightingUniforms(ShaderProgramPtr shader_program);

        FrameBufferPtr reflection_frame_buffer_{nullptr};
        FrameBufferPtr refraction_frame_buffer_{nullptr};
        ShaderProgramPtr shader_program_{nullptr};

        GLint reflection_width_{640};
        GLint reflection_height_{320};
        GLint refraction_width_{1280};
        GLint refraction_height_{720};

        DisplayConfigurationPtr display_configuration_{nullptr};
        FpsCounterPtr fps_counter_{nullptr};
        FogPtr fog_{nullptr};
        MasterManagerPtr master_manager_{nullptr};
        Object3DRendererPtr model3d_renderer_{nullptr};
        SkyboxRendererPtr skybox_renderer_{nullptr};
        StateMachinePtr state_machine_{nullptr};
    };

    using WaterRendererPtr = std::shared_ptr<WaterRenderer>;
} // namespace puffin

#endif // PUFFIN_WATER_RENDERER_H