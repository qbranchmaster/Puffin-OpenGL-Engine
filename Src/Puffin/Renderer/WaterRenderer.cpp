//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Renderer/WaterRenderer.h"

using namespace puffin;

WaterRenderer::WaterRenderer(MasterManagerPtr master_manager,
    StateMachinePtr state_machine,
    DisplayConfigurationPtr display_configuration, FogPtr fog, 
    FpsCounterPtr fps_counter, Object3DRendererPtr model3d_renderer, 
    SkyboxRendererPtr skybox_renderer) : BaseRenderer("core_water_renderer")
{
    if (!master_manager)
        logErrorAndThrow(name_, "WaterRenderer::WaterRenderer()",
            "Object [MasterManager] pointer not set.");

    if (!state_machine)
        logErrorAndThrow(name_, "WaterRenderer::WaterRenderer()",
            "Object [StateMachine] pointer not set.");

    if (!display_configuration)
        logErrorAndThrow(name_, "WaterRenderer::WaterRenderer()",
            "Object [DisplayConfiguration] pointer not set.");

    if (!fps_counter)
        logErrorAndThrow(name_, "WaterRenderer::WaterRenderer()",
            "Object [FpsCounter] pointer not set.");

    if (!fog)
        logErrorAndThrow(name_, "WaterRenderer::WaterRenderer()",
            "Object [Fog] pointer not set.");

    if (!model3d_renderer)
        logErrorAndThrow(name_, "WaterRenderer::WaterRenderer()",
            "Object [Object3DRenderer] pointer not set.");

    if (!skybox_renderer)
        logErrorAndThrow(name_, "WaterRenderer::WaterRenderer()",
            "Object [SkyboxRenderer] pointer not set.");

    master_manager_ = master_manager;
    state_machine_ = state_machine;
    display_configuration_ = display_configuration;
    fps_counter_ = fps_counter;
    fog_ = fog;
    model3d_renderer_ = model3d_renderer;
    skybox_renderer_ = skybox_renderer;

    loadShaders();

    logDebug(name_, "WaterRenderer::WaterRenderer()",
        "Water renderer created.");
}

WaterRenderer::~WaterRenderer()
{
    logDebug(name_, "WaterRenderer::~WaterRenderer()",
        "Water renderer destroyed.");
}

void WaterRenderer::loadShaders()
{
    shader_program_ = master_manager_->shaderManager()->
        createShaderProgram("water_shader_program", "shaders/WaterVs.glsl",
            "shaders/WaterFs.glsl");
}

void WaterRenderer::createFrameBuffers()
{
    reflection_frame_buffer_ = master_manager_->frameBufferManager()->
        createFrameBuffer("reflection_frame_buffer");
    master_manager_->frameBufferManager()->addTextureBuffer(
        reflection_frame_buffer_, TextureBufferType::RGB_BUFFER,
        reflection_width_, reflection_height_, false);
    master_manager_->frameBufferManager()->addRenderBuffer(
        reflection_frame_buffer_, RenderBufferType::DEPTH_STENCIL_BUFFER,
        reflection_width_, reflection_height_, false);

    refraction_frame_buffer_ = master_manager_->frameBufferManager()->
        createFrameBuffer("refraction_frame_buffer");
    master_manager_->frameBufferManager()->addTextureBuffer(
        refraction_frame_buffer_, TextureBufferType::RGB_BUFFER,
        refraction_width_, refraction_height_, false);
    master_manager_->frameBufferManager()->addTextureBuffer(
        refraction_frame_buffer_, TextureBufferType::DEPTH_BUFFER,
        refraction_width_, refraction_height_, false);
}

void WaterRenderer::setReflectionResolution(GLint width, GLint height)
{
    if (width <= 0 || height <= 0)
        logErrorAndThrow(name_, "WaterRenderer::setReflectionResolution()",
            "Reflection resolution value out of range: {0 < VALUE}.");

    if (reflection_frame_buffer_)
    {
        logWarning(name_, "WaterRenderer::setReflectionResolution()",
            "Cannot change reflection resolution during rendering.");
        return;
    }

    reflection_width_ = width;
    reflection_height_ = height;
}

void WaterRenderer::setRefractionResolution(GLint width, GLint height)
{
    if (width <= 0 || height <= 0)
        logErrorAndThrow(name_, "WaterRenderer::setRefractionResolution()",
            "Refraction resolution value out of range: {0 < VALUE}.");

    if (refraction_frame_buffer_)
    {
        logWarning(name_, "WaterRenderer::setRefractionResolution()",
            "Cannot change refraction resolution during rendering.");
        return;
    }

    refraction_width_ = width;
    refraction_height_ = height;
}

void WaterRenderer::renderToFrameBuffers(ScenePtr scene)
{
    if (!scene)
        return;

    auto water_tiles = scene->getWaterTileContainer();
    if (water_tiles.empty())
        return;

    if (!reflection_frame_buffer_ && !refraction_frame_buffer_)
        createFrameBuffers();

    state_machine_->depthTest()->enableDepthMask(true);
    state_machine_->depthTest()->enable(true);
    state_machine_->alphaBlend()->enable(false);
    state_machine_->faceCulling()->enable(true);

    // TODO: Render all water tiles - common framebuffer for each tile
    auto tile = water_tiles[0];
    renderReflectionTexture(tile, scene);
    renderRefractionTexture(tile, scene);
}

void WaterRenderer::clearFrameBuffer(GLint width, GLint height) const
{
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WaterRenderer::renderReflectionTexture(WaterTilePtr water_tile,
    ScenePtr scene)
{
    auto water_level = water_tile->getPosition().y;

    // Setup camera position and orientation for rendering reflection
    auto camera_pos = active_camera_->getPosition();
    GLfloat offset = 2.0f * (camera_pos.y - water_level);

    glm::vec3 new_camera_pos(camera_pos.x, camera_pos.y - offset, camera_pos.z);
    active_camera_->setPosition(new_camera_pos);
    active_camera_->flipPitch();

    // Render reflection
    state_machine_->bindFrameBuffer(reflection_frame_buffer_);
    clearFrameBuffer(reflection_width_, reflection_height_);
    glEnable(GL_CLIP_DISTANCE0);

    // Set clipping plane. Add small offset to water level, because when
    // DUDV map is utlised no offset causes glitches on water edges.
    // Too big value may cause wrong objects to reflect in water.
    model3d_renderer_->setClippingDistance(glm::vec4(0.0f, 1.0f, 0.0f,
        -water_level + 0.02f));

    skybox_renderer_->render(scene);

    state_machine_->depthTest()->enableDepthMask(true);
    model3d_renderer_->render(scene);

    glDisable(GL_CLIP_DISTANCE0);

    // Restore previous camera position and orientation
    active_camera_->setPosition(camera_pos);
    active_camera_->flipPitch();
}

void WaterRenderer::renderRefractionTexture(WaterTilePtr water_tile,
    ScenePtr scene)
{
    auto water_level = water_tile->getPosition().y;

    state_machine_->bindFrameBuffer(refraction_frame_buffer_);
    clearFrameBuffer(refraction_width_, refraction_height_);

    glEnable(GL_CLIP_DISTANCE0);
    model3d_renderer_->setClippingDistance(glm::vec4(0.0f, -1.0f, 0.0f,
        water_level));

    skybox_renderer_->render(scene);

    state_machine_->depthTest()->enableDepthMask(true);
    model3d_renderer_->render(scene);

    glDisable(GL_CLIP_DISTANCE0);
}

void WaterRenderer::setCameraMatricesUniforms(ShaderProgramPtr shader_program,
    WaterTilePtr water_tile)
{
    master_manager_->shaderManager()->setUniform(shader_program_,
        "matrices.view_matrix", active_camera_->getViewMatrix());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "matrices.projection_matrix", active_camera_->getProjectionMatrix());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "matrices.model_matrix", water_tile->getModelMatrix());

    master_manager_->shaderManager()->setUniform(shader_program_,
        "camera_position", active_camera_->getPosition());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "clip_near", active_camera_->getNearPlane());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "clip_far", active_camera_->getFarPlane());
}

void WaterRenderer::setTextureUniforms(ShaderProgramPtr shader_program,
    WaterTilePtr water_tile)
{
    constexpr GLint reflection_texture_slot = 0;
    constexpr GLint refraction_texture_slot = 1;
    constexpr GLint dudv_texture_slot = 2;
    constexpr GLint normal_map_texture_slot = 3;
    constexpr GLint depth_map_texture_slot = 4;

    master_manager_->shaderManager()->setUniform(shader_program_,
        "reflection_texture", reflection_texture_slot);
    master_manager_->shaderManager()->setUniform(shader_program_,
        "refraction_texture", refraction_texture_slot);
    master_manager_->shaderManager()->setUniform(shader_program_,
        "dudv_map", dudv_texture_slot);
    master_manager_->shaderManager()->setUniform(shader_program_, "normal_map",
        normal_map_texture_slot);
    master_manager_->shaderManager()->setUniform(shader_program_, "depth_map",
        depth_map_texture_slot);

    master_manager_->textureManager()->setTextureSlot(reflection_texture_slot);
    state_machine_->bindTexture(reflection_frame_buffer_->getRgbTextureBuffer());

    master_manager_->textureManager()->setTextureSlot(refraction_texture_slot);
    state_machine_->bindTexture(refraction_frame_buffer_->getRgbTextureBuffer());

    master_manager_->textureManager()->setTextureSlot(dudv_texture_slot);
    state_machine_->bindTexture(water_tile->dudv_texture_);

    master_manager_->textureManager()->setTextureSlot(normal_map_texture_slot);
    state_machine_->bindTexture(water_tile->normal_map_texture_);

    master_manager_->textureManager()->setTextureSlot(depth_map_texture_slot);
    state_machine_->bindTexture(refraction_frame_buffer_->
        getDepthTextureBuffer());
}

void WaterRenderer::setWaterTileUniforms(ShaderProgramPtr shader_program,
    WaterTilePtr water_tile)
{
    master_manager_->shaderManager()->setUniform(shader_program_,
        "texture_tiling", water_tile->getTextureTiling());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "shininess", water_tile->getShininess());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "water_color", water_tile->getWaterColor());
    master_manager_->shaderManager()->setUniform(shader_program_,
        "wave_strenght", water_tile->getWaveStrength());
}

void WaterRenderer::render(ScenePtr scene)
{
    if (!scene)
        return;

    auto water_tiles = scene->getWaterTileContainer();
    if (water_tiles.empty())
        return;

    state_machine_->depthTest()->enableDepthMask(true);
    state_machine_->depthTest()->enable(true);
    state_machine_->alphaBlend()->enable(false);
    state_machine_->faceCulling()->enable(true);

    state_machine_->activateShaderProgram(shader_program_);

    // TODO: Render all water tiles - common framebuffer for each tile
    auto water_tile = water_tiles[0];

    master_manager_->shaderManager()->setUniform(shader_program_, "move_factor",
        water_tile->move_factor_);
    water_tile->move_factor_ += (water_tile->getWaveSpeed() *
        fps_counter_->getDelta());

    if (water_tile->move_factor_ >= 1.0f)
        water_tile->move_factor_ = 0.0f;

    setTextureUniforms(shader_program_, water_tile);
    setCameraMatricesUniforms(shader_program_, water_tile);
    setWaterTileUniforms(shader_program_, water_tile);
    setFogUniforms(shader_program_);

    setLightingUniforms(shader_program_);

    state_machine_->bindMesh(water_tile);
    water_tile->draw();
}

void WaterRenderer::setFogUniforms(ShaderProgramPtr shader_program)
{
    master_manager_->shaderManager()->setUniform(shader_program, "fog.enabled",
        fog_->isEnabled() ? 1 : 0);

    if (fog_->isEnabled())
    {
        master_manager_->shaderManager()->setUniform(shader_program,
            "fog.color", fog_->getColor());
        master_manager_->shaderManager()->setUniform(shader_program,
            "fog.density", fog_->getDensity());
    }
}

void WaterRenderer::setLightingUniforms(ShaderProgramPtr shader_program)
{
    auto light_manager = master_manager_->lightManager();
    master_manager_->shaderManager()->setUniform(shader_program_,
        "directional_light.enabled", light_manager->isLightingEnabled() &&
        light_manager->directionalLight()->isEnabled());

    if (light_manager->directionalLight()->isEnabled())
    {
        master_manager_->shaderManager()->setUniform(shader_program_,
            "directional_light.direction", light_manager->directionalLight()->
            getDirection());
        master_manager_->shaderManager()->setUniform(shader_program_,
            "directional_light.color", light_manager->
            directionalLight()->getColor());
    }

    // Point lights
    if (light_manager->getPointLightsCount())
    {
        master_manager_->shaderManager()->setUniform(shader_program,
            "used_point_lights_count", static_cast<GLint>(light_manager->
                getPointLightsCount()));

        for (GLuint i = 0; i < light_manager->getPointLightsCount(); i++)
        {
            std::string uniform_name = "point_lights[" + std::to_string(i)
                + "]";
            auto p_light = light_manager->getPointLight(i);

            master_manager_->shaderManager()->setUniform(shader_program,
                uniform_name + ".enabled", p_light->isEnabled());
            master_manager_->shaderManager()->setUniform(shader_program,
                uniform_name + ".linear_factor", p_light->
                getLinearAttenuationFactor());
            master_manager_->shaderManager()->setUniform(shader_program,
                uniform_name + ".quadratic_factor", p_light->
                getQuadraticAttenuationFactor());
            master_manager_->shaderManager()->setUniform(shader_program,
                uniform_name + ".position", p_light->getPosition());
            master_manager_->shaderManager()->setUniform(shader_program,
                uniform_name + ".color", p_light->getColor());
        }
    }
}