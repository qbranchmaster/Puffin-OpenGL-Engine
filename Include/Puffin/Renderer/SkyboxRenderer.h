//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_SKYBOX_RENDERER_H
#define PUFFIN_SKYBOX_RENDERER_H

#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Manager/MasterManager.h"
#include "Puffin/Renderer/BaseRenderer.h"
#include "Puffin/Renderer/Fog.h"

namespace puffin
{
    class SkyboxRenderer : public BaseRenderer
    {
        friend class MasterRenderer;
        friend class WaterRenderer;

    public:
        SkyboxRenderer(MasterManagerPtr master_manager,
            StateMachinePtr state_machine, FogPtr fog);
        virtual ~SkyboxRenderer();

    protected:
        void render(ScenePtr scene);

        void loadShaders();
        void setShaderUniforms(SkyboxPtr skybox) const;
        void draw(SkyboxPtr skybox) const;

        ShaderProgramPtr shader_program_{nullptr};

        MasterManagerPtr master_manager_{nullptr};
        StateMachinePtr state_machine_{nullptr};
        FogPtr fog_{nullptr};
    };

    using SkyboxRendererPtr = std::shared_ptr<SkyboxRenderer>;
} // namespace puffin

#endif // PUFFIN_SKYBOX_RENDERER_H