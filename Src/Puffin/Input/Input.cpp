//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Input/Input.h"

using namespace puffin;

Input::Input(DisplayPtr display)
{
    if (!display)
        logErrorAndThrow(name_, "Input::Input()",
            "Object [Display] pointer not set.");

    display_ = display;

    logDebug(name_, "Input::Input()", "Input created.");
}

Input::~Input()
{
    logInfo(name_, "Input::~Input()", "Input destroyed.");
}

GLboolean Input::keyPressed(Key key, GLboolean sticky)
{
    GLboolean result = false;

    if (sticky)
    {
        if (glfwGetKey(display_->handle_, static_cast<GLint>(key)))
            result = true;
    }
    else
    {
        auto pressed = glfwGetKey(display_->handle_, static_cast<GLint>(key));

        if (pressed && key_states_[key] == false)
        {
            result = true;
            key_states_[key] = true;
        }
        else if (!pressed)
            key_states_[key] = false;
    }

    return result;
}

GLboolean Input::mouseKeyPressed(MouseButton button) const
{
    if (glfwGetMouseButton(display_->handle_, static_cast<GLint>(button)))
        return true;
    else
        return false;
}