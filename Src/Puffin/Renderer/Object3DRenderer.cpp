//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Renderer/Object3DRenderer.h"

using namespace puffin;

Object3DRenderer::Object3DRenderer(MasterManagerPtr master_manager,
    StateMachinePtr state_machine,
    DisplayConfigurationPtr display_configuration,
    ShadowMapConfigurationPtr shadow_map_configuration, FogPtr fog,
    PolygonModePtr polygon_mode) : BaseRenderer("core_object3d_renderer")
{
    if (!master_manager)
        logErrorAndThrow(name_, "Object3DRenderer::Object3DRenderer()",
            "Object [MasterManager] pointer not set.");

    if (!state_machine)
        logErrorAndThrow(name_, "Object3DRenderer::Object3DRenderer()",
            "Object [StateMachine] pointer not set.");

    if (!display_configuration)
        logErrorAndThrow(name_, "Object3DRenderer::Object3DRenderer()",
            "Object [DisplayConfiguration] pointer not set.");

    if (!shadow_map_configuration)
        logErrorAndThrow(name_, "Object3DRenderer::Object3DRenderer()",
            "Object [ShadowMapConfiguration] pointer not set.");

    if (!fog)
        logErrorAndThrow(name_, "Object3DRenderer::Object3DRenderer()",
            "Object [Fog] pointer not set.");

    if (!polygon_mode)
        logErrorAndThrow(name_, "Object3DRenderer::Object3DRenderer()",
            "Object [PolygonMode] pointer not set.");

    master_manager_ = master_manager;
    state_machine_ = state_machine;
    display_configuration_ = display_configuration;
    shadow_map_ = shadow_map_configuration;
    fog_ = fog;
    polygon_mode_ = polygon_mode;

    stencil_buffer_.reset(new StencilBuffer());
    stencil_buffer_->enable(true);

    loadShaders();

    logDebug(name_, "Object3DRenderer::Object3DRenderer()",
        "Object 3D renderer created.");
}

Object3DRenderer::~Object3DRenderer()
{
    logDebug(name_, "Object3DRenderer::~Object3DRenderer()",
        "Object 3D renderer destroyed.");
}

void Object3DRenderer::loadShaders()
{
    basic_shader_ = master_manager_->shaderManager()->
        createShaderProgram("object3d_basic_shader",
            "shaders/Object3DBasicVs.glsl", "shaders/Object3DBasicFs.glsl",
            "shaders/Object3DBasicGs.glsl");

    outline_shader_ = master_manager_->shaderManager()->
        createShaderProgram("object3d_outline_shader",
            "shaders/Object3DOutlineVs.glsl",
            "shaders/Object3DOutlineFs.glsl");

    polygon_mode_shader_ = master_manager_->shaderManager()->
        createShaderProgram("object3d_polygon_mode_shader",
            "shaders/Object3DPolygonVs.glsl", "shaders/Object3DPolygonFs.glsl");
}

void Object3DRenderer::render(ScenePtr scene)
{
    if (!scene)
        return;

    auto objects_3d = scene->getObject3DContainer();
    if (objects_3d.empty())
        return;

    active_skybox_ = scene->getActiveSkybox();

    for (const auto &object_3d : objects_3d)
        renderObject3D(object_3d);
}

void Object3DRenderer::setCameraMatricesUniforms(
    ShaderProgramPtr shader_program, Object3DPtr object3d)
{
    master_manager_->shaderManager()->setUniform(shader_program,
        "matrices.view_matrix", active_camera_->getViewMatrix());
    master_manager_->shaderManager()->setUniform(shader_program,
        "matrices.projection_matrix", active_camera_->
        getProjectionMatrix());
    master_manager_->shaderManager()->setUniform(shader_program,
        "matrices.model_matrix", object3d->getModelMatrix());
}

void Object3DRenderer::setPolygonModeUniforms(ShaderProgramPtr shader_program)
{
    master_manager_->shaderManager()->setUniform(shader_program,
        "lines_color", polygon_mode_->getLinesColor());
}

void Object3DRenderer::renderObject3DEntities(Object3DPtr object3d,
    ShaderProgramPtr shader_program, GLboolean use_materials)
{
    for (GLuint i = 0; i < object3d->getEntitiesCount(); i++)
    {
        if (use_materials)
            setMaterials(object3d, i, shader_program);

        object3d->draw(i);
    }
}

void Object3DRenderer::setFogUniforms(ShaderProgramPtr shader_program)
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

void Object3DRenderer::setOutlineUniforms(ShaderProgramPtr shader_program,
    OutlinePtr outline)
{
    master_manager_->shaderManager()->setUniform(shader_program,
        "outline_color", outline->getColor());
}

void Object3DRenderer::setLightsUniforms(ShaderProgramPtr shader_program)
{
    auto light_manager = master_manager_->lightManager();

    master_manager_->shaderManager()->setUniform(shader_program,
        "lighting_enabled", light_manager->isLightingEnabled());

    // Directional
    if (shadow_map_->isShadowsEnabled() && light_manager->directionalLight()->
        isEnabled())
    {
        master_manager_->shaderManager()->setUniform(shader_program,
            "matrices.dir_light_pv_matrix", master_manager_->lightManager()->
            directionalLight()->getProjectionViewMatrix());
    }

    master_manager_->shaderManager()->setUniform(shader_program,
        "directional_light.enabled", light_manager->directionalLight()->
        isEnabled() ? 1 : 0);

    if (light_manager->directionalLight()->isEnabled())
    {
        master_manager_->shaderManager()->setUniform(shader_program,
            "directional_light.direction", light_manager->directionalLight()->
            getDirection());
        master_manager_->shaderManager()->setUniform(shader_program,
            "directional_light.color", light_manager->directionalLight()->
            getColor());
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

void Object3DRenderer::prepareRendering()
{
    state_machine_->depthTest()->enable(true);
    state_machine_->depthTest()->enableDepthMask(true);
    state_machine_->alphaBlend()->enable(true);
    state_machine_->alphaBlend()->setBlendFunction(BlendFunction::NORMAL);
    state_machine_->faceCulling()->enable(true);
    state_machine_->unbindAllTextures();
}

void Object3DRenderer::setEnvironmentMapUniforms(
    ShaderProgramPtr shader_program)
{
    constexpr GLint env_texture_index = 2;

    master_manager_->textureManager()->setTextureSlot(env_texture_index);
    master_manager_->shaderManager()->setUniform(shader_program,
        "env_map_texture", env_texture_index);
    state_machine_->bindTexture(active_skybox_->getTexture());
    master_manager_->shaderManager()->setUniform(shader_program,
        "matrices.env_map_model_matrix", active_skybox_->getModelMatrix());
}

void Object3DRenderer::setMaterials(Object3DPtr object3d, GLuint entity_index,
    ShaderProgramPtr shader_program)
{
    auto material = object3d->getEntity(entity_index)->getMaterial();
    if (!material)
        return;

    // Setup material parameters
    master_manager_->shaderManager()->setUniform(shader_program,
        "object_material.ka", material->getKa());
    master_manager_->shaderManager()->setUniform(shader_program,
        "object_material.kd", material->getKd());
    master_manager_->shaderManager()->setUniform(shader_program,
        "object_material.ks", material->getKs());
    master_manager_->shaderManager()->setUniform(shader_program,
        "object_material.shininess", material->getShininess());
    master_manager_->shaderManager()->setUniform(shader_program,
        "object_material.reflectivity", material->getReflectivity());

    // Setup and bind textures
    constexpr GLint diffuse_texture_index = 0;
    constexpr GLint normalmap_texture_index = 1;
    constexpr GLint shadow_map_texture_index = 3;
    constexpr GLint shadow_map_point_texture_index = 4;

    // Diffuse texture
    master_manager_->textureManager()->setTextureSlot(diffuse_texture_index);
    master_manager_->shaderManager()->setUniform(shader_program,
        "object_material.has_diffuse_texture", material->getDiffuseTexture() ?
        1 : 0);

    if (material->getDiffuseTexture())
    {
        master_manager_->shaderManager()->setUniform(shader_program,
            "object_material.diffuse_texture", diffuse_texture_index);
        state_machine_->bindTexture(material->getDiffuseTexture());
    }
    else
        state_machine_->unbindTexture(TextureType::TEXTURE_2D);

    // Normal map texture
    master_manager_->textureManager()->setTextureSlot(normalmap_texture_index);
    master_manager_->shaderManager()->setUniform(shader_program,
        "object_material.has_normalmap_texture",
        material->getNormalMapTexture() ? 1 : 0);

    if (material->getNormalMapTexture())
    {
        master_manager_->shaderManager()->setUniform(shader_program,
            "object_material.normalmap_texture", normalmap_texture_index);
        state_machine_->bindTexture(material->getNormalMapTexture());
    }
    else
        state_machine_->unbindTexture(TextureType::TEXTURE_2D);

    // Shadow map directional light
    master_manager_->textureManager()->setTextureSlot(shadow_map_texture_index);
    master_manager_->shaderManager()->setUniform(shader_program,
        "shadow_map_texture", shadow_map_texture_index);

    if (master_manager_->lightManager()->isLightingEnabled() &&
        master_manager_->lightManager()->directionalLight()->isEnabled() &&
        shadow_map_->isShadowsEnabled() && !polygon_mode_->isEnabled())
        state_machine_->bindTexture(shadow_map_texture_);
    else
        state_machine_->unbindTexture(TextureType::TEXTURE_2D);

    // Shadow map point light
    for (GLuint i = 0; i < master_manager_->lightManager()->
        getPointLightsCount(); i++)
    {
        auto pl = master_manager_->lightManager()->getPointLight(i);

        master_manager_->textureManager()->setTextureSlot(
            shadow_map_point_texture_index + i);
        master_manager_->shaderManager()->setUniform(shader_program,
            "point_shadow_map_" + std::to_string(i + 1),
            static_cast<GLint>(shadow_map_point_texture_index + i));

        if (master_manager_->lightManager()->isLightingEnabled() &&
            shadow_map_->isShadowsEnabled() && !polygon_mode_->isEnabled() &&
            pl->isEnabled())
            state_machine_->bindTexture(point_light_shadow_maps_[i]);
        else
            state_machine_->unbindTexture(TextureType::TEXTURE_CUBE);
    }
}

void Object3DRenderer::setShadowMapUniforms(ShaderProgramPtr shader_program)
{
    master_manager_->shaderManager()->setUniform(shader_program,
        "shadow.distance", shadow_map_->getShadowDistance());
    master_manager_->shaderManager()->setUniform(shader_program,
        "shadow.transition_distance", shadow_map_->
        getShadowTransitionDistance());
    master_manager_->shaderManager()->setUniform(shader_program,
        "shadow.enabled", shadow_map_->isShadowsEnabled());
    master_manager_->shaderManager()->setUniform(shader_program,
        "shadow.map_size", shadow_map_->getShadowMapSizeDirectionalLight());
    master_manager_->shaderManager()->setUniform(shader_program,
        "shadow.pcf_filter_count", shadow_map_->getPcfSamplesCount());
}

void Object3DRenderer::renderObject3D(Object3DPtr object3d)
{
    if (!object3d)
        return;

    prepareRendering();
    state_machine_->bindMesh(object3d);

    // Check object's modifiers    
    OutlinePtr outline = std::static_pointer_cast<Outline>
        (object3d->getModifier(Object3DModifierType::OUTLINE));

    GLboolean render_outline = false;
    if (outline != nullptr && outline->isEnabled())
        render_outline = true;

    GLboolean polygon_mode = polygon_mode_->isEnabled();

    if (full_render_ && render_outline && !polygon_mode)
    {
        stencil_buffer_->enableDrawing(true);
        stencil_buffer_->setAction(StencilBufferAction::REPLACE);
        stencil_buffer_->passesAlways(1);
    }

    if (polygon_mode && !polygon_mode_->isUsingPipeline())
    {
        state_machine_->activateShaderProgram(polygon_mode_shader_);

        setCameraMatricesUniforms(polygon_mode_shader_, object3d);
        setPolygonModeUniforms(polygon_mode_shader_);

        renderObject3DEntities(object3d, polygon_mode_shader_, false);
    }
    else
    {
        state_machine_->activateShaderProgram(basic_shader_);
        master_manager_->shaderManager()->setUniform(basic_shader_,
            "clip_plane", clip_plane_);

        setFogUniforms(basic_shader_);
        setLightsUniforms(basic_shader_);
        setCameraMatricesUniforms(basic_shader_, object3d);
        setEnvironmentMapUniforms(basic_shader_);

        if (shadow_map_->isShadowsEnabled())
            setShadowMapUniforms(basic_shader_);

        renderObject3DEntities(object3d, basic_shader_, true);
    }

    // Second draw for outlining
    if (full_render_ && render_outline && !polygon_mode)
    {
        stencil_buffer_->passesNotEqual(1);
        stencil_buffer_->enableDrawing(false);

        // Disable depth testing if outline should be always visible
        if (outline->isAlwaysVisible())
            state_machine_->depthTest()->enable(false);

        // Scale object's outline
        auto prev_scale = object3d->getScale();
        object3d->setScale(prev_scale * outline->getScale());

        state_machine_->activateShaderProgram(outline_shader_);
        setCameraMatricesUniforms(outline_shader_, object3d);
        setOutlineUniforms(outline_shader_, outline);

        renderObject3DEntities(object3d, outline_shader_, false);

        object3d->setScale(prev_scale);

        stencil_buffer_->enableDrawing(true);
        stencil_buffer_->setAction(StencilBufferAction::KEEP);
    }
}

void Object3DRenderer::render(ScenePtr scene, ShaderProgramPtr shader_program)
{
    if (!scene || !shader_program)
        return;

    auto objects_3d = scene->getObject3DContainer();
    if (objects_3d.empty())
        return;

    prepareRendering();

    for (const auto &object : objects_3d)
    {
        state_machine_->bindMesh(object);
        master_manager_->shaderManager()->setUniform(shader_program,
            "matrices.model_matrix", object->getModelMatrix());

        for (GLuint i = 0; i < object->getEntitiesCount(); i++)
            object->draw(i);
    }
}