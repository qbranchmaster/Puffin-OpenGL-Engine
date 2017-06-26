//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_OBJECT_3D_RENDERER_H
#define PUFFIN_OBJECT_3D_RENDERER_H

#include "Puffin/Configuration/ShadowMapConfiguration.h"
#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Manager/MasterManager.h"
#include "Puffin/Renderer/BaseRenderer.h"
#include "Puffin/Renderer/Fog.h"
#include "Puffin/Renderer/PolygonMode.h"
#include "Puffin/Renderer/StencilBuffer.h"

namespace puffin
{
    class Object3DRenderer : public BaseRenderer
    {
        friend class MasterRenderer;
        friend class ShadowMapRenderer;
        friend class WaterRenderer;

    public:
        Object3DRenderer(MasterManagerPtr master_manager,
            StateMachinePtr state_machine,
            DisplayConfigurationPtr display_configuration,
            ShadowMapConfigurationPtr shadow_map_configuration, FogPtr fog,
            PolygonModePtr polygon_mode);
        virtual ~Object3DRenderer();

        void enableFullRender(GLboolean enable)
        {
            full_render_ = enable;
        }

    protected:
        void render(ScenePtr scene);
        void render(ScenePtr scene, ShaderProgramPtr shader_program);

        void renderObject3D(Object3DPtr object3d);
        void renderObject3DEntities(Object3DPtr object3d,
            ShaderProgramPtr shader_program, GLboolean use_materials);

        void loadShaders();
        void prepareRendering();
        void setCameraMatricesUniforms(ShaderProgramPtr shader_program,
            Object3DPtr object3d);
        void setFogUniforms(ShaderProgramPtr shader_program);
        void setOutlineUniforms(ShaderProgramPtr shader_program,
            OutlinePtr outline);
        void setPolygonModeUniforms(ShaderProgramPtr shader_program);
        void setLightsUniforms(ShaderProgramPtr shader_program);
        void setEnvironmentMapUniforms(ShaderProgramPtr shader_program);
        void setShadowMapUniforms(ShaderProgramPtr shader_program);
        void setMaterials(Object3DPtr object3d, GLuint entity_index,
            ShaderProgramPtr shader_program);

        void setClippingDistance(const glm::vec4 &plane)
        {
            clip_plane_ = plane;
        }

        GLboolean full_render_{true};

        DisplayConfigurationPtr display_configuration_{nullptr};
        FogPtr fog_{nullptr};
        MasterManagerPtr master_manager_{nullptr};
        PolygonModePtr polygon_mode_{nullptr};
        ShadowMapConfigurationPtr shadow_map_{nullptr};
        StateMachinePtr state_machine_{nullptr};
        StencilBufferPtr stencil_buffer_{nullptr};

        ShaderProgramPtr basic_shader_{nullptr};
        ShaderProgramPtr outline_shader_{nullptr};
        ShaderProgramPtr polygon_mode_shader_{nullptr};

        SkyboxPtr active_skybox_{nullptr};
        TexturePtr shadow_map_texture_{nullptr};
        std::vector<TexturePtr> point_light_shadow_maps_;

        glm::vec4 clip_plane_{0.0f, 0.0f, 0.0f, 0.0f};
    };

    using Object3DRendererPtr = std::shared_ptr<Object3DRenderer>;
} // namespace puffin

#endif // PUFFIN_OBJECT_3D_RENDERER_H