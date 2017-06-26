//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Renderer/FrameBuffer.h"

using namespace puffin;

FrameBuffer::FrameBuffer(std::string name)
{
    if (!name.empty())
        name_ = name;

    glGenFramebuffers(1, &handle_);

    logDebug(name_, "FrameBuffer::FrameBuffer()", "Frame buffer created.");
}

FrameBuffer::~FrameBuffer()
{
    if (depth_stencil_.handle)
        glDeleteRenderbuffers(1, &depth_stencil_.handle);

    glDeleteFramebuffers(1, &handle_);

    logDebug(name_, "FrameBuffer::~FrameBuffer()", "Frame buffer destroyed.");
}