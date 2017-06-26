//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Display/Display.h"

using namespace puffin;

Display::Display(DisplayConfigurationPtr display_configuration, SystemPtr system)
{
    if (!display_configuration)
        logErrorAndThrow(name_, "Display::Display()",
            "Object [DisplayConfiguration] pointer not set.");

    if (!system)
        logErrorAndThrow(name_, "Display::Display()",
            "Object [System] pointer not set.");

    width_ = display_configuration->getWidth();
    height_ = display_configuration->getHeight();
    msaa_samples_ = display_configuration->getMsaaSamples();
    fullscreen_ = display_configuration->isFullscreen();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, msaa_samples_);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    handle_ = glfwCreateWindow(width_, height_, caption_.c_str(),
        fullscreen_ ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    if (!handle_)
        logErrorAndThrow(name_, "Display::Display()",
            "Creating display error.");

    glfwMakeContextCurrent(handle_);
    system->initializeGL();

    logInfo(name_, "Display::Display()", "GPU Vendor: " +
        system->getGpuVendor());
    logInfo(name_, "Display::Display()", "GPU Name: " +
        system->getGpuName());
    logInfo(name_, "Display::Display()", "OpenGL Version: " +
        system->getGlVersion());
    logInfo(name_, "Display::Display()", "GLSL Version: " +
        system->getGlslVersion());

    // Do not allow to change engine settings at runtime
    display_configuration->blocked_ = true;

    logDebug(name_, "Display::Display()", "Display created.");
}

Display::~Display()
{
    if (handle_)
    {
        glfwDestroyWindow(handle_);
        handle_ = nullptr;
        logDebug(name_, "Display::~Display()", "Display destroyed.");
    }
}