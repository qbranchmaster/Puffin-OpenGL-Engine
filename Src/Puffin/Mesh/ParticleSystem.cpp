//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Mesh/ParticleSystem.h"

using namespace puffin;

ParticleSystem::ParticleSystem(TextureManagerPtr texture_manager,
    const glm::vec3 &position, std::string texture_path, GLint atlas_size,
    GLboolean animate, GLint pps)
{
    if (!texture_manager)
        logErrorAndThrow(name_, "ParticleSystem::ParticleSystem()",
            "Object [TextureManager] pointer not set.");

    texture_manager_ = texture_manager;

    // PPS = Particles Per Second
    if (pps <= 0)
        logErrorAndThrow(name_, "ParticleSystem::ParticleSystem()",
            "Particles per second value out of range: {0 < VALUE}.");

    if (atlas_size <= 0)
        logErrorAndThrow(name_, "ParticleSystem::ParticleSystem()",
            "Texture atlas size value out of range: {0 < VALUE}.");

    position_ = position;
    pps_ = pps;
    texture_atlas_size_ = atlas_size;

    // Animate texture option uses texture atlas to change particle texture
    // in time
    animate_texture_ = animate;
    particles_texture_ = texture_manager_->loadTexture2D(texture_path);

    logDebug(name_, "ParticleSystem::ParticleSystem()",
        "Particle system created.");
}

ParticleSystem::~ParticleSystem()
{
    logDebug(name_, "ParticleSystem::~ParticleSystem()",
        "Particle system destroyed.");
}

void ParticleSystem::setParticleGenerationFunction(
    std::function<ParticlePtr()> function)
{
    if (!function)
        logErrorAndThrow(name_,
            "ParticleSystem::setParticleGenerationFunction()",
            "Function pointer not set.");

    generate_function_ = function;
}

void ParticleSystem::generateParticles(GLfloat time_delta)
{
    // Time between two following particles
    GLfloat pps_rate = 1.0f / pps_;
    elapsed_time_ += time_delta;

    if (elapsed_time_ >= pps_rate)
    {
        GLint particles_to_generate = static_cast<GLint>(elapsed_time_ /
            pps_rate);

        for (GLint i = 0; i < particles_to_generate; i++)
        {
            auto particle = generate_function_();
            particles_list_.push_back(particle);
        }

        elapsed_time_ = 0.0f;
    }
}

void ParticleSystem::updateParticles(GLfloat time_delta, CameraPtr camera)
{
    generateParticles(time_delta);

    // Update and remove particles
    auto it = particles_list_.begin();
    while (it != particles_list_.end())
    {
        bool alive = (*it)->update(time_delta);
        if (!alive)
            particles_list_.erase(it++);
        else
        {
            // Calculate particle distance from camera
            auto distance = glm::length(camera->getPosition() -
                (*it)->getPosition());
            (*it)->distance_ = distance;
            ++it;
        }
    }

    if (sort_particles_)
    {
        particles_list_.sort([](const ParticlePtr &a, const ParticlePtr &b)
        { return a->distance_ > b->distance_; });
    }
}