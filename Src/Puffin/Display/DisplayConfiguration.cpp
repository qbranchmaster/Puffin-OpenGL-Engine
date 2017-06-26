//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Display/DisplayConfiguration.h"

using namespace puffin;

DisplayConfiguration::DisplayConfiguration(SystemPtr system)
{
    if (!system)
        logErrorAndThrow(name_, "DisplayConfiguration::DisplayConfiguration()",
            "Object [System] pointer not set.");

    system_ = system;

    logDebug(name_, "DisplayConfiguration::DisplayConfiguration()",
        "Display configuration created.");
}

DisplayConfiguration::~DisplayConfiguration()
{
    logDebug(name_, "DisplayConfiguration::~DisplayConfiguration()",
        "Display configuration destroyed.");
}

void DisplayConfiguration::configure(GLint width, GLint height,
    GLint msaa_samples, GLboolean fullscreen)
{
    if (blocked_)
    {
        logWarning(name_, "DisplayConfiguration::configure()",
            "Cannot change display configuration at runtime.");
        return;
    }

    constexpr GLint min_width = 320;
    GLint max_width = system_->getMaxDisplaySize().first;

    if (checkIntValue(width, min_width, max_width))
        width_ = width;
    else
        logErrorAndThrow(name_, "DisplayConfiguration::configure()",
            "Display width value out of range: {" + std::to_string(min_width) +
            " <= VALUE <= " + std::to_string(max_width) + "}.");

    constexpr GLint min_height = 240;
    GLint max_height = system_->getMaxDisplaySize().second;

    if (checkIntValue(height, min_height, max_height))
        height_ = height;
    else
        logErrorAndThrow(name_, "DisplayConfiguration::configure()",
            "Display height value out of range: {" +
            std::to_string(min_height) + " <= VALUE <= " +
            std::to_string(max_height) + "}.");

    if (checkIntValue(msaa_samples, system_->getSupportedMsaaSamples()))
        msaa_samples_ = msaa_samples;
    else
        // TODO: Create this message from system_->getSupportedMsaaSamples(), 
        // now msaa samples are hardcoded in this string
        logErrorAndThrow(name_, "DisplayConfiguration::configure()",
            "MSAA samples value out of range: {VALUE = [1, 2, 4, 8]}.");

    fullscreen_ = fullscreen;
}

GLboolean DisplayConfiguration::checkIntValue(GLint value,
    std::vector<GLint> allowed_values) const
{
    for (const auto &v : allowed_values)
        if (v == value)
            return true;

    return false;
}

GLboolean DisplayConfiguration::checkIntValue(GLint value, GLint min,
    GLint max) const
{
    if (value >= min && value <= max)
        return true;

    return false;
}