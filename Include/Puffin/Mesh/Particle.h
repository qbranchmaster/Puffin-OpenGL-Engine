//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_PARTICLE_H
#define PUFFIN_PARTICLE_H

#include <GL/glew.h>    

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <string>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class Particle
    {
        friend class ParticleSystem;

    public:
        Particle(const glm::vec3 &position, const glm::vec3 &velocity,
            GLfloat life_length, GLfloat gravity, GLfloat gravity_effect = 1.0f,
            GLfloat rotation = 0.0f, GLfloat scale = 1.0f);

        glm::vec3 getPosition() const
        {
            return position_;
        }

        void setVelocity(const glm::vec3 &velocity)
        {
            velocity_ = velocity;
        }

        glm::vec3 getVelocity() const
        {
            return velocity_;
        }

        void setRotation(GLfloat rotation)
        {
            rotation_ = rotation;
        }

        GLfloat getRotation() const
        {
            return rotation_;
        }

        void setScale(GLfloat scale)
        {
            if (scale <= 0.0f)
                logErrorAndThrow(name_, "Particle::setScale()",
                    "Particle scale value out of range: {0.0 < VALUE}.");

            scale_ = scale;
        }

        GLfloat getScale() const
        {
            return scale_;
        }

        GLfloat getMaxLifeLenght() const
        {
            return life_length_;
        }

        GLfloat getCurrentLifeLenght() const
        {
            return current_life_length_;
        }

        void setTextureIndex(GLint index)
        {
            // Texture index is used to select one particle texture from
            // several textures contained in texture atlas

            if (index < 0)
                logErrorAndThrow(name_, "Particle::setTextureIndex()",
                    "Particle texture index value out of range: {0 <= VALUE}.");

            texture_index_ = index;
        }

        GLint getTextureIndex() const
        {
            return texture_index_;
        }

    protected:
        GLboolean update(GLfloat delta_time)
        {
            velocity_.y += gravity_ * gravity_effect_ * delta_time;

            glm::vec3 distance = velocity_ * delta_time;
            position_ += distance;

            current_life_length_ += delta_time;

            // Returns 'true' if particle is still alive, and returns 'false' 
            // when particle is already dead
            return (current_life_length_ < life_length_);
        }

        std::string name_{"unnamed_particle"};

        glm::vec3 position_{0.0f, 0.0f, 0.0f};
        glm::vec3 velocity_{0.0f, 0.0f, 0.0f};

        GLfloat gravity_{-15.0f};
        GLfloat gravity_effect_{1.0f};
        GLfloat current_life_length_{0.0f};
        GLfloat life_length_{0.0f};
        GLfloat rotation_{0.0f};
        GLfloat scale_{1.0f};

        GLint texture_index_{0};

        GLfloat distance_{0.0f};
    };

    using ParticlePtr = std::shared_ptr<Particle>;
} // namespace puffin

#endif // PUFFIN_PARTICLE_H