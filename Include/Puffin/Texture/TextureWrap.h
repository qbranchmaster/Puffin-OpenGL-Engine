//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_TEXTURE_WRAP_H
#define PUFFIN_TEXTURE_WRAP_H

namespace puffin
{
    enum class TextureWrap
    {
        REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
    };
} // namespace puffin

#endif // PUFFIN_TEXTURE_WRAP_H