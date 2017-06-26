//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Mesh/Object3D.h"

using namespace puffin;

Object3D::Object3D(std::string name) : BaseMesh(name)
{
    logDebug(name_, "Object3D::Object3D()", "Object 3D created.");
}

Object3D::~Object3D()
{
    logDebug(name_, "Object3D::~Object3D()", "Object 3D destroyed.");
}

void Object3D::addModifier(Object3DModifierPtr modifier)
{
    if (!modifier)
        logErrorAndThrow(name_, "Object3D::Object3D()",
            "Object [Object3DModifier] pointer not set.");

    modifiers_[modifier->getType()] = modifier;
}