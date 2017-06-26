//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Renderer/SkyboxRenderer.h"

using namespace puffin;

SkyboxRenderer::SkyboxRenderer(MasterManagerPtr master_manager,
    StateMachinePtr state_machine, FogPtr fog) :
    BaseRenderer("core_skybox_renderer")
{
    if (!master_manager)
        logErrorAndThrow(name_, "SkyboxRenderer::SkyboxRenderer()",
            "Object [MasterManager] pointer not set.");

    if (!state_machine)
        logErrorAndThrow(name_, "SkyboxRenderer::SkyboxRenderer()",
            "Object [StateMachine] pointer not set.");

    if (!fog)
        logErrorAndThrow(name_, "SkyboxRenderer::SkyboxRenderer()",
            "Object [Fog] pointer not set.");

    state_machine_ = state_machine;
    master_manager_ = master_manager;
    fog_ = fog;

    loadShaders();

    logDebug(name_, "SkyboxRenderer::SkyboxRenderer()",
        "Skybox renderer created.");
}

SkyboxRenderer::~SkyboxRenderer()
{
    logDebug(name_, "SkyboxRenderer::~SkyboxRenderer()",
        "Skybox renderer destroyed.");
}

void SkyboxRenderer::loadShaders()
{
    shader_program_ = master_manager_->shaderManager()->
        createShaderProgram("skybox_shader_program", "shaders/SkyboxVS.glsl",
            "shaders/SkyboxFS.glsl");
}

void SkyboxRenderer::render(ScenePtr scene)
{
    if (!scene)
        return;

    auto skybox = scene->getActiveSkybox();
    if (!skybox)
        return;

    state_machine_->unbindAllTextures();
    state_machine_->activateShaderProgram(shader_program_);
    setShaderUniforms(skybox);

    // Configure
    state_machine_->depthTest()->enable(true);
    state_machine_->depthTest()->enableDepthMask(false);
    state_machine_->alphaBlend()->enable(false);

    // Bind texture
    auto texture = skybox->getTexture();
    master_manager_->textureManager()->setTextureSlot(0);
    if (texture)
        state_machine_->bindTexture(texture);
    else
        state_machine_->unbindTexture(TextureType::TEXTURE_CUBE);

    draw(skybox);
}

void SkyboxRenderer::setShaderUniforms(SkyboxPtr skybox) const
{
    // Camera matrices
    master_manager_->shaderManager()->setUniform(shader_program_,
        "matrices.view_matrix", active_camera_->getViewMatrixStatic());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "matrices.projection_matrix", active_camera_->getProjectionMatrix());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "matrices.model_matrix", skybox->getModelMatrix());

    // Texture and color
    master_manager_->shaderManager()->setUniform(shader_program_,
        "color.filter_color", skybox->getFilterColor());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "color.cube_texture", static_cast<GLint>(0));
    master_manager_->shaderManager()->setUniform(shader_program_,
        "color.light_color", master_manager_->lightManager()->
        directionalLight()->getColor());

    // Fog parameters
    master_manager_->shaderManager()->setUniform(shader_program_,
        "fog.enabled", fog_->isEnabled());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "fog.color", fog_->getColor());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "fog.density",
        fog_->getSkyboxFogOverallDensity());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "fog.transition_power",
        fog_->getSkyboxFogTransitionPower());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "fog.height", fog_->getSkyboxFogHeight());
}

void SkyboxRenderer::draw(SkyboxPtr skybox) const
{
    state_machine_->bindMesh(skybox);
    skybox->draw();
}