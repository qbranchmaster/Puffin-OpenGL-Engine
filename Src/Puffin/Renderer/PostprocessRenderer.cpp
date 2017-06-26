//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Renderer/PostprocessRenderer.h"

using namespace puffin;

PostprocessRenderer::PostprocessRenderer(MasterManagerPtr master_manager,
    StateMachinePtr state_machine,
    DisplayConfigurationPtr display_configuration,
    FrameBufferManagerPtr frame_buffer_manager)
{
    if (!master_manager)
        logErrorAndThrow(name_, "PostprocessRenderer::PostprocessRenderer()",
            "Object [MasterManager] pointer not set.");

    if (!state_machine)
        logErrorAndThrow(name_, "PostprocessRenderer::PostprocessRenderer()",
            "Object [StateMachine] pointer not set.");

    if (!display_configuration)
        logErrorAndThrow(name_, "PostprocessRenderer::PostprocessRenderer()",
            "Object [DisplayConfiguration] pointer not set.");

    if (!frame_buffer_manager)
        logErrorAndThrow(name_, "PostprocessRenderer::PostprocessRenderer()",
            "Object [FrameBufferManager] pointer not set.");

    master_manager_ = master_manager;
    state_machine_ = state_machine;
    display_configuration_ = display_configuration;
    frame_buffer_manager_ = frame_buffer_manager;

    loadShaders();
    createFrameBuffers();

    logDebug(name_, "PostprocessRenderer::PostprocessRenderer()",
        "Postprocess renderer created.");
}

PostprocessRenderer::~PostprocessRenderer()
{
    logDebug(name_, "PostprocessRenderer::~PostprocessRenderer()",
        "Postprocess renderer destroyed.");
}

void PostprocessRenderer::loadShaders()
{
    shader_program_ = master_manager_->shaderManager()->
        createShaderProgram("postprocess_shader", "shaders/PostprocessVs.glsl",
            "shaders/PostprocessFs.glsl");
}

void PostprocessRenderer::setShaderUniforms()
{
    master_manager_->shaderManager()->setUniform(shader_program_,
        "color.screen_texture", static_cast<GLint>(0));
    master_manager_->shaderManager()->setUniform(shader_program_,
        "color.kernel_size", kernel_size_);
    master_manager_->shaderManager()->setUniform(shader_program_,
        "color.effect", static_cast<GLint>(effect_));
    master_manager_->shaderManager()->setUniform(shader_program_,
        "color.tint_color", tint_color_);
}

void PostprocessRenderer::createFrameBuffers()
{
    auto v_width = display_configuration_->getWidth();
    auto v_height = display_configuration_->getHeight();

    frame_buffer_multisample_ = frame_buffer_manager_->
        createFrameBuffer("postprocess_frame_buffer_multisample");
    frame_buffer_manager_->addTextureBuffer(frame_buffer_multisample_,
        TextureBufferType::RGB_BUFFER, v_width, v_height, true);
    frame_buffer_manager_->addRenderBuffer(frame_buffer_multisample_,
        RenderBufferType::DEPTH_STENCIL_BUFFER, v_width, v_height, true);

    frame_buffer_simple_ = frame_buffer_manager_->
        createFrameBuffer("postprocess_frame_buffer_simple");
    frame_buffer_manager_->addTextureBuffer(frame_buffer_simple_,
        TextureBufferType::RGB_BUFFER, v_width, v_height, false);
}

FrameBufferPtr PostprocessRenderer::getFrameBufferToRender() const
{
    state_machine_->bindFrameBuffer(frame_buffer_multisample_,
        FrameBufferBindType::ONLY_READ);
    state_machine_->bindFrameBuffer(frame_buffer_simple_,
        FrameBufferBindType::ONLY_WRITE);

    auto v_width = display_configuration_->getWidth();
    auto v_height = display_configuration_->getHeight();

    glBlitFramebuffer(0, 0, v_width, v_height, 0, 0, v_width, v_height,
        GL_COLOR_BUFFER_BIT, GL_LINEAR);

    return frame_buffer_simple_;
}