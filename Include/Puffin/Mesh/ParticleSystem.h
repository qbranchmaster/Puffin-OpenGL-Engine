//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_PARTICLE_SYSTEM_H
#define PUFFIN_PARTICLE_SYSTEM_H

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <algorithm>
#include <functional>
#include <list>
#include <memory>
#include <vector>

#include "Puffin/Camera/Camera.h"
#include "Puffin/Common/Logger.h"
#include "Puffin/Configuration/AlphaBlend.h"
#include "Puffin/Manager/TextureManager.h"
#include "Puffin/Mesh/Particle.h"

namespace puffin
{
    class ParticleSystem
    {
        friend class ParticleRenderer;

    public:
        ParticleSystem(TextureManagerPtr texture_manager,
            const glm::vec3 &position, std::string texture_path,
            GLint atlas_size, GLboolean animate, GLint pps);
        virtual ~ParticleSystem();

        void setPosition(const glm::vec3 &position)
        {
            position_ = position;
        }

        glm::vec3 getPosition() const
        {
            return position_;
        }

        TexturePtr getParticlesTexture() const
        {
            return particles_texture_;
        }

        GLint getTextureAtlasSize() const
        {
            return texture_atlas_size_;
        }

        void setBlendingType(BlendFunction blend)
        {
            blend_func_ = blend;
        }

        BlendFunction getBlendingType() const
        {
            return blend_func_;
        }

        void sortParticles(GLboolean enabled)
        {
            sort_particles_ = enabled;
        }

        void setParticleGenerationFunction(std::function<ParticlePtr()>);

    protected:
        void generateParticles(GLfloat time_delta);
        void updateParticles(GLfloat time_delta, CameraPtr camera);

        std::list<ParticlePtr> getParticlesList() const
        {
            return particles_list_;
        }

        std::string name_{"unnamed_particle_system"};

        glm::vec3 position_{0.0f, 0.0f, 0.0f};
        GLint pps_{0};
        GLint texture_atlas_size_{1};
        GLfloat elapsed_time_{0.0f};

        GLboolean sort_particles_{true};
        GLboolean animate_texture_{true};

        TexturePtr particles_texture_{nullptr};
        TextureManagerPtr texture_manager_{nullptr};

        std::list<ParticlePtr> particles_list_;
        std::function<ParticlePtr()> generate_function_{nullptr};
        BlendFunction blend_func_{BlendFunction::NORMAL};
    };

    using ParticleSystemPtr = std::shared_ptr<ParticleSystem>;
} // namespace puffin

#endif // PUFFIN_PARTICLE_SYSTEM_H