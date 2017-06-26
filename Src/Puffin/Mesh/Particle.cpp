//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Mesh/Particle.h"

using namespace puffin;

Particle::Particle(const glm::vec3 &position, const glm::vec3 &velocity,
    GLfloat life_length, GLfloat gravity, GLfloat gravity_effect,
    GLfloat rotation, GLfloat scale)
{
    if (life_length <= 0.0f)
        logErrorAndThrow(name_, "Particle::Particle()", "Particle life length "
            "value out of range: {0.0 < VALUE}.");

    if (gravity_effect < 0.0f)
        logErrorAndThrow(name_, "Particle::Particle()", "Particle gravity "
            "effect value out of range: {0.0 <= VALUE}.");

    if (scale <= 0.0f)
        logErrorAndThrow(name_, "Particle::Particle()", "Particle scale value "
            "out of range: {0.0 < VALUE}.");

    position_ = position;
    velocity_ = velocity;

    gravity_ = gravity;
    gravity_effect_ = gravity_effect;
    life_length_ = life_length;

    setRotation(rotation);
    setScale(scale);
}