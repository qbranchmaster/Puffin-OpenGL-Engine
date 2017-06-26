//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Mesh/Skybox.h"

using namespace puffin;

Skybox::Skybox(std::string name) : BaseMesh(name)
{
    logDebug(name_, "Skybox::Skybox()", "Skybox created.");
}

Skybox::~Skybox()
{
    logDebug(name_, "Skybox::~Skybox()", "Skybox destroyed.");
}

void Skybox::setTexture(TexturePtr texture)
{
    if (texture->getType() != TextureType::TEXTURE_CUBE)
        logErrorAndThrow(name_, "Skybox::setTexture()",
            "Invalid texture type.");

    texture_ = texture;
}