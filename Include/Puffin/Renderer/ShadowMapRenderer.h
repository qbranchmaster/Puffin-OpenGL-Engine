//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_SHADOW_MAP_RENDERER_H
#define PUFFIN_SHADOW_MAP_RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <vector>

#include "Puffin/Configuration/ShadowMapConfiguration.h"
#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Manager/MasterManager.h"
#include "Puffin/Renderer/BaseRenderer.h"
#include "Puffin/Renderer/Object3DRenderer.h"

namespace puffin
{
    class ShadowMapRenderer : public BaseRenderer
    {
        friend class MasterRenderer;

    public:
        ShadowMapRenderer(MasterManagerPtr master_manager,
            StateMachinePtr state_machine,
            Object3DRendererPtr object3d_renderer,
            ShadowMapConfigurationPtr shadow_map_configuration);
        virtual ~ShadowMapRenderer();

    protected:
        void loadShaders();
        void createFrameBufferDirectionalLight();
        void createFrameBufferPointLights();

        void render(ScenePtr scene);
        void renderDirectionalLight(ScenePtr scene);
        void renderPointLights(ScenePtr scene);

        ShaderProgramPtr depth_map_directional_shader_{nullptr};
        ShaderProgramPtr depth_map_point_shader_{nullptr};

        MasterManagerPtr master_manager_{nullptr};
        Object3DRendererPtr object3d_renderer_{nullptr};
        ShadowMapConfigurationPtr shadow_map_configuration_{nullptr};
        StateMachinePtr state_machine_{nullptr};

        FrameBufferPtr dir_light_frame_buffer_{nullptr};
        std::vector<FrameBufferPtr> point_light_frame_buffer_container_;

        glm::mat4 pl_projection_matrix_{1.0f};
    };

    using ShadowMapRendererPtr = std::shared_ptr<ShadowMapRenderer>;
} // namespace puffin

#endif // PUFFIN_SHADOW_MAP_RENDERER_H