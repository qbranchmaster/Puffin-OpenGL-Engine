//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_TEXTURE_TYPE_H
#define PUFFIN_TEXTURE_TYPE_H

namespace puffin
{
    enum class TextureType
    {
        TEXTURE_UNDEFINED,
        TEXTURE_CUBE,
        TEXTURE_2D,
        TEXTURE_2D_MULTISAMPLED,
    };
} // namespace puffin

#endif // PUFFIN_TEXTURE_TYPE_H