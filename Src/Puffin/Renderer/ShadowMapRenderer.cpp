//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Renderer/ShadowMapRenderer.h"

using namespace puffin;

ShadowMapRenderer::ShadowMapRenderer(MasterManagerPtr master_manager,
    StateMachinePtr state_machine, Object3DRendererPtr object3d_renderer,
    ShadowMapConfigurationPtr shadow_map_configuration) :
    BaseRenderer("core_shadow_map_renderer")
{
    if (!master_manager)
        logErrorAndThrow(name_, "ShadowMapRenderer::ShadowMapRenderer()",
            "Object [MasterManager] pointer not set.");

    if (!state_machine)
        logErrorAndThrow(name_, "ShadowMapRenderer::ShadowMapRenderer()",
            "Object [StateMachine] pointer not set.");

    if (!object3d_renderer)
        logErrorAndThrow(name_, "ShadowMapRenderer::ShadowMapRenderer()",
            "Object [Object3DRenderer] pointer not set.");

    if (!shadow_map_configuration)
        logErrorAndThrow(name_, "ShadowMapRenderer::ShadowMapRenderer()",
            "Object [ShadowMapConfiguration] pointer not set.");

    master_manager_ = master_manager;
    state_machine_ = state_machine;
    object3d_renderer_ = object3d_renderer;
    shadow_map_configuration_ = shadow_map_configuration;

    loadShaders();

    pl_projection_matrix_ = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f,
        shadow_map_configuration_->getShadowDistance());

    logDebug(name_, "ShadowMapRenderer::ShadowMapRenderer()",
        "Shadow map renderer created.");
}

ShadowMapRenderer::~ShadowMapRenderer()
{
    logDebug(name_, "ShadowMapRenderer::~ShadowMapRenderer()",
        "Shadow map renderer destroyed.");
}

void ShadowMapRenderer::loadShaders()
{
    depth_map_directional_shader_ = master_manager_->shaderManager()->
        createShaderProgram("depth_map_directional_shader",
            "shaders/DepthMapDirectionalVs.glsl",
            "shaders/DepthMapDirectionalFs.glsl");

    depth_map_point_shader_ = master_manager_->shaderManager()->
        createShaderProgram("depth_map_point_shader",
            "shaders/DepthMapPointVs.glsl",
            "shaders/DepthMapPointFs.glsl",
            "shaders/DepthMapPointGs.glsl");
}

void ShadowMapRenderer::renderDirectionalLight(ScenePtr scene)
{
    auto dir_light = master_manager_->lightManager()->directionalLight();

    if (!dir_light->isEnabled())
        return;

    if (!dir_light_frame_buffer_)
        createFrameBufferDirectionalLight();

    // TODO: Check that code
    active_camera_->cameraBox()->update(shadow_map_configuration_->
        getShadowDistance());
    dir_light->calculateViewMatrix(active_camera_->cameraBox()->getCenter());
    active_camera_->cameraBox()->recalculateSpace(dir_light->getViewMatrix());

    glm::vec3 projection_vector(active_camera_->cameraBox()->getWidth(),
        active_camera_->cameraBox()->getHeight(),
        active_camera_->cameraBox()->getLength());

    dir_light->calculateProjectionMatrix(projection_vector);
    dir_light->calculateViewMatrix(active_camera_->getPosition());
    //---

    state_machine_->activateShaderProgram(depth_map_directional_shader_);
    master_manager_->shaderManager()->setUniform(depth_map_directional_shader_,
        "matrices.light_space_matrix", dir_light->getProjectionViewMatrix());

    state_machine_->bindFrameBuffer(dir_light_frame_buffer_);

    auto map_size = shadow_map_configuration_->
        getShadowMapSizeDirectionalLight();
    glViewport(0, 0, map_size, map_size);
    glClear(GL_DEPTH_BUFFER_BIT);

    object3d_renderer_->render(scene, depth_map_directional_shader_);
    object3d_renderer_->shadow_map_texture_ = dir_light_frame_buffer_->
        getDepthTextureBuffer();
}

void ShadowMapRenderer::createFrameBufferDirectionalLight()
{
    auto map_size = shadow_map_configuration_->
        getShadowMapSizeDirectionalLight();

    dir_light_frame_buffer_ = master_manager_->frameBufferManager()->
        createFrameBuffer("depth_map_dir_light");
    master_manager_->frameBufferManager()->addTextureBuffer(
        dir_light_frame_buffer_, TextureBufferType::DEPTH_BUFFER,
        map_size, map_size, true);

    master_manager_->frameBufferManager()->disableDrawBuffer(
        dir_light_frame_buffer_);
    master_manager_->frameBufferManager()->disableReadBuffer(
        dir_light_frame_buffer_);
}

void ShadowMapRenderer::createFrameBufferPointLights()
{
    auto map_size = shadow_map_configuration_->getShadowMapSizePointLight();

    for (int i = 0; i < master_manager_->lightManager()->
        getMaxPointLightsCount(); i++)
    {
        FrameBufferPtr frame_buffer = master_manager_->frameBufferManager()->
            createFrameBuffer("depth_map_point_lights_" + std::to_string(i));
        master_manager_->frameBufferManager()->addCubeMapDepthBuffer(
            frame_buffer, map_size);

        master_manager_->frameBufferManager()->disableDrawBuffer(frame_buffer);
        master_manager_->frameBufferManager()->disableReadBuffer(frame_buffer);

        point_light_frame_buffer_container_.push_back(frame_buffer);
    }
}

void ShadowMapRenderer::render(ScenePtr scene)
{
    if (!scene)
        return;

    auto models = scene->getObject3DContainer();

    if (models.empty())
        return;

    state_machine_->unbindAllTextures();
    state_machine_->depthTest()->enable(true);
    state_machine_->depthTest()->enableDepthMask(true);
    state_machine_->alphaBlend()->enable(false);
    state_machine_->faceCulling()->enable(true);

    renderDirectionalLight(scene);
    renderPointLights(scene);
}

void ShadowMapRenderer::renderPointLights(ScenePtr scene)
{
    if (!point_light_frame_buffer_container_.size())
        createFrameBufferPointLights();

    state_machine_->activateShaderProgram(depth_map_point_shader_);
    master_manager_->shaderManager()->setUniform(depth_map_point_shader_,
        "shadow_distance", shadow_map_configuration_->getShadowDistance());

    object3d_renderer_->point_light_shadow_maps_.clear();

    for (GLuint i = 0; i < master_manager_->lightManager()->
        getPointLightsCount(); i++)
    {
        auto pl = master_manager_->lightManager()->getPointLight(i);

        if (!pl->isEnabled())
            continue;

        std::vector<glm::mat4> shadow_transforms;
        auto light_pos = pl->getPosition();

        shadow_transforms.push_back(pl_projection_matrix_ *
            glm::lookAt(light_pos, light_pos + glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f)));
        shadow_transforms.push_back(pl_projection_matrix_ *
            glm::lookAt(light_pos, light_pos + glm::vec3(-1.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, -1.0f, 0.0f)));
        shadow_transforms.push_back(pl_projection_matrix_ *
            glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)));
        shadow_transforms.push_back(pl_projection_matrix_ *
            glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, -1.0f)));
        shadow_transforms.push_back(pl_projection_matrix_ *
            glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f, 1.0f),
                glm::vec3(0.0f, -1.0f, 0.0f)));
        shadow_transforms.push_back(pl_projection_matrix_ *
            glm::lookAt(light_pos, light_pos + glm::vec3(0.0f, 0.0f, -1.0f),
                glm::vec3(0.0f, -1.0f, 0.0f)));

        master_manager_->shaderManager()->setUniform(depth_map_point_shader_,
            "light_position", pl->getPosition());

        for (GLint i = 0; i < 6; i++)
            master_manager_->shaderManager()->setUniform(
                depth_map_point_shader_, "shadow_matrices[" +
                std::to_string(i) + "].mat", shadow_transforms[i]);

        state_machine_->bindFrameBuffer(point_light_frame_buffer_container_[i]);
        auto map_size = shadow_map_configuration_->getShadowMapSizePointLight();
        glViewport(0, 0, map_size, map_size);
        glClear(GL_DEPTH_BUFFER_BIT);

        object3d_renderer_->render(scene, depth_map_point_shader_);
        object3d_renderer_->point_light_shadow_maps_.push_back(
            point_light_frame_buffer_container_[i]->getCubeTextureBuffer());
    }
}