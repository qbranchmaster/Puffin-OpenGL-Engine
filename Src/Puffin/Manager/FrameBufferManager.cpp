//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Manager/FrameBufferManager.h"

using namespace puffin;

FrameBufferManager::FrameBufferManager(StateMachinePtr state_machine,
    DisplayConfigurationPtr display_configuration, 
    TextureManagerPtr texture_manager) : 
    BaseManager("core_frame_buffer_manager")
{
    if (!state_machine)
        logErrorAndThrow(name_, "FrameBufferManager::FrameBufferManager()",
            "Object [StateMachine] pointer not set.");

    if (!display_configuration)
        logErrorAndThrow(name_, "FrameBufferManager::FrameBufferManager()",
            "Object [DisplayConfiguration] pointer not set.");

    if (!texture_manager)
        logErrorAndThrow(name_, "FrameBufferManager::FrameBufferManager()",
            "Object [TextureManager] pointer not set.");

    state_machine_ = state_machine;
    display_configuration_ = display_configuration;
    texture_manager_ = texture_manager;

    logDebug(name_, "FrameBufferManager::FrameBufferManager()",
        "Frame buffer manager created.");
}

FrameBufferManager::~FrameBufferManager()
{
    logDebug(name_, "FrameBufferManager::~FrameBufferManager()",
        "Frame buffer manager destroyed.");
}

FrameBufferPtr FrameBufferManager::createFrameBuffer(std::string buffer_name)
{
    FrameBufferPtr frame_buffer(new FrameBuffer(buffer_name));
    frame_buffer_container_.push_back(frame_buffer);
    return frame_buffer;
}

void FrameBufferManager::addTextureBuffer(FrameBufferPtr frame_buffer,
    TextureBufferType type, GLint width, GLint height, GLboolean mulitsampled)
{
    if (!frame_buffer)
        logErrorAndThrow(name_, "FrameBufferManager::addTextureBuffer()",
            "Object [FrameBuffer] pointer not set.");

    if (width <= 0 || height <= 0)
        logErrorAndThrow(name_, "FrameBufferManager::addTextureBuffer()",
            "Texture buffer size value out of range: {0 < VALUE}.");

    state_machine_->bindFrameBuffer(frame_buffer);

    switch (type)
    {
    case TextureBufferType::DEPTH_BUFFER:
        frame_buffer->depth_buffer_texture_ = texture_manager_->
            createTextureDepthBuffer(width, height);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D, frame_buffer->depth_buffer_texture_->handle_, 0);
        break;
    case TextureBufferType::RGB_BUFFER:
        frame_buffer->rgb_buffer_texture_ = texture_manager_->
            createTextureRgbBuffer(width, height, mulitsampled);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            mulitsampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
            frame_buffer->rgb_buffer_texture_->handle_, 0);
        break;
    }
}

void FrameBufferManager::addRenderBuffer(FrameBufferPtr frame_buffer,
    RenderBufferType type, GLint width, GLint height, GLboolean multisampled)
{
    if (!frame_buffer)
        logErrorAndThrow(name_, "FrameBufferManager::addRenderBuffer()",
            "Object [FrameBuffer] pointer not set.");

    if (width <= 0 || height <= 0)
        logErrorAndThrow(name_, "FrameBufferManager::addRenderBuffer()",
            "Render buffer size value out of range: {0 < VALUE}.");

    state_machine_->bindFrameBuffer(frame_buffer);

    switch (type)
    {
    case RenderBufferType::DEPTH_STENCIL_BUFFER:
        if (!frame_buffer->depth_stencil_.handle)
            glGenRenderbuffers(1, &frame_buffer->depth_stencil_.handle);

        frame_buffer->depth_stencil_.width = width;
        frame_buffer->depth_stencil_.height = height;

        glBindRenderbuffer(GL_RENDERBUFFER,
            frame_buffer->depth_stencil_.handle);

        if (multisampled)
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, 
                display_configuration_->getMsaaSamples(), GL_DEPTH24_STENCIL8, 
                frame_buffer->depth_stencil_.width,
                frame_buffer->depth_stencil_.height);
        else
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                frame_buffer->depth_stencil_.width,
                frame_buffer->depth_stencil_.height);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_RENDERBUFFER, frame_buffer->depth_stencil_.handle);
        break;
    }
}

void FrameBufferManager::addCubeMapDepthBuffer(FrameBufferPtr frame_buffer,
    GLint size)
{
    if (!frame_buffer)
        logErrorAndThrow(name_, "FrameBufferManager::addCubeMapDepthBuffer()",
            "Object [FrameBuffer] pointer not set.");

    if (size <= 0)
        logErrorAndThrow(name_, "FrameBuffer::addCubeMapDepthBuffer",
            "Cube buffer size value out of range: {0 < VALUE}.");

    state_machine_->bindFrameBuffer(frame_buffer);

    frame_buffer->cube_buffer_texture_ = texture_manager_->
        createTextureCube(size);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        frame_buffer->cube_buffer_texture_->handle_, 0);
}

void FrameBufferManager::disableDrawBuffer(FrameBufferPtr frame_buffer)
{
    if (!frame_buffer)
        logErrorAndThrow(name_, "FrameBufferManager::disableDrawBuffer()",
            "Object [FrameBuffer] pointer not set.");

    state_machine_->bindFrameBuffer(frame_buffer);
    glDrawBuffer(GL_NONE);
}

void FrameBufferManager::disableReadBuffer(FrameBufferPtr frame_buffer)
{
    if (!frame_buffer)
        logErrorAndThrow(name_, "FrameBufferManager::disableReadBuffer()",
            "Object [FrameBuffer] pointer not set.");

    state_machine_->bindFrameBuffer(frame_buffer);
    glReadBuffer(GL_NONE);
}