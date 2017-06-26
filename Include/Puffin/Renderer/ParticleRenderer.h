//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_PARTICLE_RENDERER_H
#define PUFFIN_PARTICLE_RENDERER_H

#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Manager/MasterManager.h"
#include "Puffin/Mesh/Particle.h"
#include "Puffin/Renderer/BaseRenderer.h"
#include "Puffin/Renderer/FpsCounter.h"

namespace puffin
{
    class ParticleRenderer : public BaseRenderer
    {
        friend class MasterRenderer;

    public:
        ParticleRenderer(MasterManagerPtr master_manager,
            StateMachinePtr state_machine,
            DisplayConfigurationPtr display_configuration,
            FpsCounterPtr fps_counter);
        virtual ~ParticleRenderer();

    protected:
        void loadShaderProgram();
        void createParticleModel();

        void render(ScenePtr scene);
        void setCameraUniforms(ShaderProgramPtr shader_program);

        Object3DPtr particle_model_{nullptr};
        ShaderProgramPtr shader_program_{nullptr};

        DisplayConfigurationPtr display_configuration_{nullptr};
        MasterManagerPtr master_manager_{nullptr};
        StateMachinePtr state_machine_{nullptr};
        FpsCounterPtr fps_counter_{nullptr};
    };

    using ParticleRendererPtr = std::shared_ptr<ParticleRenderer>;
} // namespace puffin

#endif // PUFFIN_PARTICLE_RENDERER_H