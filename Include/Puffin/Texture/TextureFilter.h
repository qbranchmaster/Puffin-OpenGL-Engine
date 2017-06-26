//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_TEXTURE_FILTER_H
#define PUFFIN_TEXTURE_FILTER_H

namespace puffin
{
    enum class TextureFilter
    {
        NEAREST,
        BILINEAR,
        BILINEAR_WITH_MIPMAPS,
        TRILINEAR,
    };
} // namespace puffin

#endif // PUFFIN_TEXTURE_FILTER_H