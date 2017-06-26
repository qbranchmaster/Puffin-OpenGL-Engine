//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_INPUT_H
#define PUFFIN_INPUT_H

#include <map>
#include <memory>

#include "Puffin/Common/Logger.h"
#include "Puffin/Display/Display.h"
#include "Puffin/Input/KeyCodes.h"

namespace puffin
{
    class Input
    {
    public:
        explicit Input(DisplayPtr display);
        virtual ~Input();

        std::string getName() const
        {
            return name_;
        }

        GLboolean keyPressed(Key key, GLboolean sticky = true);
        GLboolean mouseKeyPressed(MouseButton button) const;

        void setCursorPosition(GLdouble x, GLdouble y) const
        {
            glfwSetCursorPos(display_->handle_, x, y);
        }

        void getCursorPosition(GLfloat &x_pos, GLfloat &y_pos) const
        {
            GLdouble cur_x = 0;
            GLdouble cur_y = 0;

            glfwGetCursorPos(display_->handle_, &cur_x, &cur_y);

            x_pos = static_cast<GLfloat>(cur_x);
            y_pos = static_cast<GLfloat>(cur_y);
        }

    protected:
        std::string name_{"core_input"};

        DisplayPtr display_{nullptr};

        std::map<Key, GLboolean> key_states_;
    };

    using InputPtr = std::shared_ptr<Input>;
} // namespace puffin

#endif // PUFFIN_INPUT_H