//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_KEY_CODES_H
#define PUFFIN_KEY_CODES_H

namespace puffin
{
    enum class Key
    {
        NUM_0 = 48,
        NUM_1 = 49,
        NUM_2 = 50,
        A = 65,
        D = 68,
        S = 83,
        T = 84,
        W = 87,
        ESCAPE = 256,
        ARROW_RIGHT = 262,
        ARROW_LEFT = 263,
        ARROW_DOWN = 264,
        ARROW_UP = 265,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        LEFT_SHIFT = 340,
        LEFT_CONTROL = 341,
    };

    enum class MouseButton
    {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2,
    };
} // namespace puffin

#endif // PUFFIN_KEY_CODES_H