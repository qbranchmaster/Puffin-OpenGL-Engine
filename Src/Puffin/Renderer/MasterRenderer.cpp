//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Renderer/MasterRenderer.h"

using namespace puffin;

MasterRenderer::MasterRenderer(MasterManagerPtr master_manager,
    StateMachinePtr state_machine, DisplayPtr display,
    DisplayConfigurationPtr display_configuration)
{
    if (!master_manager)
        logErrorAndThrow(name_, "MasterRenderer::MasterRenderer()",
            "Object [MasterManager] pointer not set.");

    if (!state_machine)
        logErrorAndThrow(name_, "MasterRenderer::MasterRenderer()",
            "Object [StateMachine] pointer not set.");

    if (!display)
        logErrorAndThrow(name_, "MasterRenderer::MasterRenderer()",
            "Object [Display] pointer not set.");

    if (!display_configuration)
        logErrorAndThrow(name_, "MasterRenderer::MasterRenderer()",
            "Object [DisplayConfiguration] pointer not set.");

    master_manager_ = master_manager;
    state_machine_ = state_machine;
    target_display_ = display;
    display_configuration_ = display_configuration;

    fog_.reset(new Fog());
    polygon_mode_.reset(new PolygonMode());
    fps_counter_.reset(new FpsCounter());
    shadow_map_.reset(new ShadowMapConfiguration());

    // Create renderers
    font_renderer_.reset(new FontRenderer(master_manager_, state_machine_,
        display_configuration_));
    skybox_renderer_.reset(new SkyboxRenderer(master_manager_, state_machine_,
        fog_));
    postprocess_renderer_.reset(new PostprocessRenderer(master_manager_,
        state_machine_, display_configuration_,
        master_manager_->frameBufferManager()));
    model3d_renderer_.reset(new Object3DRenderer(master_manager_,
        state_machine_, display_configuration_, shadow_map_, fog_,
        polygon_mode_));
    particle_renderer_.reset(new ParticleRenderer(master_manager_,
        state_machine_, display_configuration_, fps_counter_));
    shadow_map_renderer_.reset(new ShadowMapRenderer(master_manager_,
        state_machine_, model3d_renderer_, shadow_map_));
    water_renderer_.reset(new WaterRenderer(master_manager_, state_machine_,
        display_configuration_, fog_, fps_counter_, model3d_renderer_,
        skybox_renderer_));

    createScreenModel();

    logDebug(name_, "MasterRenderer::MasterRenderer()",
        "Master renderer created.");
}

MasterRenderer::~MasterRenderer()
{
    logDebug(name_, "MasterRenderer::~MasterRenderer()",
        "Master renderer destroyed.");
}

void MasterRenderer::start()
{
    rendering_enabled_ = true;
    while (rendering_enabled_)
    {
        fps_counter_->startDeltaMeasure();

        target_display_->pollEvents();

        if (rendering_function_ != nullptr)
            rendering_function_();

        target_display_->swapBuffers();

        fps_counter_->endDeltaMeasure();

        if (target_display_->isClosing())
            stop();
    }
}

void MasterRenderer::stop()
{
    rendering_enabled_ = false;
}

void MasterRenderer::assignRenderingFunction(std::function<void()> function)
{
    if (!function)
        logErrorAndThrow(name_, "MasterRenderer::assignRenderingFunction()",
            "Specified empty rendering function pointer.");

    rendering_function_ = function;

    logDebug(name_, "MasterRenderer::assignRenderingFunction()",
        "Rendering function set.");
}

void MasterRenderer::createScreenModel()
{
    screen_ = master_manager_->meshManager()->createObject3D("screen");

    std::vector<GLfloat> positions = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };

    std::vector<GLfloat> texture_coords = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    master_manager_->meshManager()->setMeshData(screen_, positions,
        VertexDataType::POSITION, false);
    master_manager_->meshManager()->setMeshData(screen_, texture_coords,
        VertexDataType::TEXTURE_COORD, false);

    Object3DEntityPtr entity(new Object3DEntity());
    entity->setVerticesCount(6);
    screen_->addEntity(entity);
}

void MasterRenderer::drawScene(ScenePtr scene)
{
    if (!scene)
        return;

    state_machine_->depthTest()->enableDepthMask(true);
    state_machine_->depthTest()->enable(true);
    state_machine_->faceCulling()->enable(true);

    // Render shadow map
    if (!polygon_mode_->isEnabled() && shadow_map_->isShadowsEnabled() &&
        master_manager_->lightManager()->isLightingEnabled())
        shadow_map_renderer_->render(scene);

    // Render water tiles reflection and refraction
    active_camera_->update_camera_box_ = false;
    model3d_renderer_->enableFullRender(false);
    water_renderer_->renderToFrameBuffers(scene);
    active_camera_->update_camera_box_ = true;

    state_machine_->unbindFrameBuffer();
    clear();

    if (!polygonMode()->isEnabled())
    {
        state_machine_->bindFrameBuffer(postprocess_renderer_->
            frame_buffer_multisample_);
        clear();
    }

    if (!polygonMode()->isEnabled())
        skybox_renderer_->render(scene);

    model3d_renderer_->enableFullRender(true);
    model3d_renderer_->render(scene);
    water_renderer_->render(scene);
    particle_renderer_->render(scene);

    if (!polygonMode()->isEnabled())
    {
        state_machine_->alphaBlend()->enable(false);
        state_machine_->activateShaderProgram(postprocess_renderer_->
            shader_program_);
        postprocess_renderer_->setShaderUniforms();

        state_machine_->bindMesh(screen_);

        master_manager_->textureManager()->setTextureSlot(0);
        state_machine_->bindTexture(postprocess_renderer_->
            getFrameBufferToRender()->getRgbTextureBuffer());
        state_machine_->unbindFrameBuffer();
        clear();

        screen_->draw();

        // GUI
        font_renderer_->render(scene);
    }

    active_camera_->update(fps_counter_->getDelta());
    fps_counter_->update();
}