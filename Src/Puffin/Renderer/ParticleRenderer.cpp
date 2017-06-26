//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Renderer/ParticleRenderer.h"

using namespace puffin;

ParticleRenderer::ParticleRenderer(MasterManagerPtr master_manager,
    StateMachinePtr state_machine, DisplayConfigurationPtr display_configuration,
    FpsCounterPtr fps_counter) : BaseRenderer("core_particle_renderer")
{
    if (!master_manager)
        logErrorAndThrow(name_, "ParticleRenderer::ParticleRenderer()",
            "Object [MasterManager] pointer not set.");

    if (!state_machine)
        logErrorAndThrow(name_, "ParticleRenderer::ParticleRenderer()",
            "Object [StateMachine] pointer not set.");

    if (!display_configuration)
        logErrorAndThrow(name_, "ParticleRenderer::ParticleRenderer()",
            "Object [DisplayConfiguration] pointer not set.");

    if (!fps_counter)
        logErrorAndThrow(name_, "ParticleRenderer::ParticleRenderer()",
            "Object [FpsCounter] pointer not set.");

    master_manager_ = master_manager;
    state_machine_ = state_machine;
    display_configuration_ = display_configuration;
    fps_counter_ = fps_counter;

    loadShaderProgram();
    createParticleModel();

    logDebug(name_, "ParticleRenderer::ParticleRenderer()",
        "Particle renderer created.");
}

ParticleRenderer::~ParticleRenderer()
{
    logDebug(name_, "ParticleRenderer::~ParticleRenderer()",
        "Particle renderer destroyed.");
}

void ParticleRenderer::loadShaderProgram()
{
    shader_program_ = master_manager_->shaderManager()->
        createShaderProgram("particle_shader_program",
            "shaders/ParticleVs.glsl", "shaders/ParticleFs.glsl");
}

void ParticleRenderer::createParticleModel()
{
    particle_model_ = master_manager_->meshManager()->
        createObject3D("particle_model");

    std::vector<GLfloat> positions = {
        -0.1f,  0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f,
        0.1f,  0.1f, 0.0f,
        0.1f,  0.1f, 0.0f,
        -0.1f, -0.1f, 0.0f,
        0.1f, -0.1f, 0.0f
    };

    master_manager_->meshManager()->setMeshData(particle_model_, positions,
        VertexDataType::POSITION, false);

    Object3DEntityPtr entity(new Object3DEntity());
    entity->setVerticesCount(6);
    particle_model_->addEntity(entity);
}

void ParticleRenderer::setCameraUniforms(ShaderProgramPtr shader_program)
{
    master_manager_->shaderManager()->setUniform(shader_program,
        "view_matrix", active_camera_->getViewMatrix());
    master_manager_->shaderManager()->setUniform(shader_program,
        "projection_matrix", active_camera_->getProjectionMatrix());
}

void ParticleRenderer::render(ScenePtr scene)
{
    if (!scene)
        return;

    auto particle_systems = scene->getParticleSystemContainer();
    if (!particle_systems.size())
        return;

    state_machine_->bindMesh(particle_model_);

    state_machine_->depthTest()->enable(true);
    state_machine_->faceCulling()->enable(true);
    state_machine_->faceCulling()->setCulledSide(CulledSide::BACK);
    state_machine_->alphaBlend()->enable(true);

    // Disabling depth mask prevents from sorting particles in wrong order
    state_machine_->depthTest()->enableDepthMask(false);

    master_manager_->textureManager()->setTextureSlot(0);
    state_machine_->activateShaderProgram(shader_program_);
    setCameraUniforms(shader_program_);

    for (const auto &particle_system : particle_systems)
    {
        state_machine_->alphaBlend()->setBlendFunction(particle_system->
            getBlendingType());

        state_machine_->bindTexture(particle_system->getParticlesTexture());

        auto particles = particle_system->getParticlesList();
        auto view_matrix = active_camera_->getViewMatrix();

        // Render particles
        for (const auto &particle : particles)
        {
            glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f),
                particle->getPosition());
            // Remove rotation part of the matrix
            model_matrix[0][0] = view_matrix[0][0];
            model_matrix[1][0] = view_matrix[0][1];
            model_matrix[2][0] = view_matrix[0][2];
            model_matrix[0][1] = view_matrix[1][0];
            model_matrix[1][1] = view_matrix[1][1];
            model_matrix[2][1] = view_matrix[1][2];
            model_matrix[0][2] = view_matrix[2][0];
            model_matrix[1][2] = view_matrix[2][1];
            model_matrix[2][2] = view_matrix[2][2];

            auto scale = particle->getScale();
            model_matrix = glm::scale(model_matrix,
                glm::vec3(scale, scale, scale));

            master_manager_->shaderManager()->setUniform(shader_program_,
                "model_matrix", model_matrix);

            // Calculate texture
            auto atlas_size = particle_system->getTextureAtlasSize() *
                particle_system->getTextureAtlasSize();

            auto max_life_length = particle->getMaxLifeLenght();
            auto life_length = particle->getCurrentLifeLenght();

            GLfloat texture_time = max_life_length / atlas_size;
            GLint p_index = particle->getTextureIndex();
            GLfloat p_time = (p_index + 1) * texture_time;

            if (life_length > p_time)
                particle->setTextureIndex(p_index + 1);

            GLint row = particle->getTextureIndex() /
                particle_system->getTextureAtlasSize();
            GLint col = particle->getTextureIndex() -
                (row * particle_system->getTextureAtlasSize());

            master_manager_->shaderManager()->setUniform(shader_program_,
                "texture_row", static_cast<GLint>(row));
            master_manager_->shaderManager()->setUniform(shader_program_,
                "texture_col", static_cast<GLint>(col));
            master_manager_->shaderManager()->setUniform(shader_program_,
                "atlas_size", particle_system->getTextureAtlasSize());

            particle_model_->draw();
        }

        particle_system->updateParticles(fps_counter_->getDelta(),
            active_camera_);
    }
}