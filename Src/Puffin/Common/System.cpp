//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Common/System.h"

using namespace puffin;

System::System()
{
    initializeGLFW();

    logDebug(name_, "System::System()", "System created.");
}

System::~System()
{
    terminateGLFW();

    logDebug(name_, "System::~System()", "System destroyed.");
}

void System::initializeGLFW() const
{
    if (glfwInit() != GL_TRUE)
        logErrorAndThrow(name_, "System::initializeGLFW()",
            "Window manager initialization error.");

    logInfo(name_, "System::initializeGLFW()", "Window manager initialized.");
}

void System::terminateGLFW() const
{
    glfwTerminate();
    logInfo(name_, "System::terminateGLFW()", "Window manager terminated.");
}

void System::initializeGL() const
{
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        logErrorAndThrow(name_, "System::initializeGL()",
            "OpenGL core initialization error.");

    logInfo(name_, "System::initializeGL()", "OpenGL core initialized.");
}

std::pair<GLint, GLint> System::getMaxDisplaySize() const
{
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLint screen_width = mode->width;
    GLint screen_height = mode->height;

    return std::make_pair(screen_width, screen_height);
}

std::vector<GLint> System::getSupportedMsaaSamples() const
{
    // TODO: Read these values from GL or GLFW (is it possible?)
    return {1, 2, 4, 8};
}

std::string System::getGpuVendor() const
{
    const GLubyte *data = glGetString(GL_VENDOR);
    return std::string(reinterpret_cast<const char*>(data));
}

std::string System::getGpuName() const
{
    const GLubyte *data = glGetString(GL_RENDERER);
    return std::string(reinterpret_cast<const char*>(data));
}

std::string System::getGlVersion() const
{
    const GLubyte *data = glGetString(GL_VERSION);
    return std::string(reinterpret_cast<const char*>(data));
}

std::string System::getGlslVersion() const
{
    const GLubyte *data = glGetString(GL_SHADING_LANGUAGE_VERSION);
    return std::string(reinterpret_cast<const char*>(data));
}