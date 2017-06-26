//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Lighting/DirectionalLight.h"

using namespace puffin;

DirectionalLight::DirectionalLight()
{
    logInfo(name_, "DirectionalLight::DirectionalLight()",
        "Directional light created.");
}

DirectionalLight::~DirectionalLight()
{
    logInfo(name_, "DirectionalLight::~DirectionalLight()",
        "Directional light destroyed.");
}