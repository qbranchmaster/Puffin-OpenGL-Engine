//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_FOG_H
#define PUFFIN_FOG_H

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class Fog
    {
    public:
        Fog()
        {
            logDebug(name_, "Fog::Fog()", "Fog created.");
        }

        virtual ~Fog()
        {
            logDebug(name_, "Fog::~Fog()", "Fog destroyed.");
        }

        std::string getName() const
        {
            return name_;
        }

        void enable(GLboolean state)
        {
            enabled_ = state;
        }

        GLboolean isEnabled() const
        {
            return enabled_;
        }

        void setDensity(GLfloat density)
        {
            if (density < 0.0f || density > 1.0f)
                logErrorAndThrow(name_, "Fog::setDensity()",
                    "Fog density value out of range: {0.0 <= VALUE <= 1.0}.");

            density_ = density;
        }

        GLfloat getDensity() const
        {
            return density_;
        }

        void setColor(const glm::vec3 &color)
        {
            color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getColor() const
        {
            return color_;
        }

        void setSkyboxFog(GLfloat overall_density, GLfloat transition_power,
            GLfloat height)
        {
            if (overall_density < 0.0f || overall_density > 1.0f)
                logErrorAndThrow(name_, "Fog::setSkyboxFog()",
                    "Skybox fog overall density value out of range: "
                    "{0.0 <= VALUE <= 1.0}.");

            if (transition_power < 0.0f)
                logErrorAndThrow(name_, "Fog::setSkyboxFog()",
                    "Skybox fog transition power value out of range: "
                    "{0.0 <= VALUE}.");

            if (height < 0.0f || height > 1.0f)
                logErrorAndThrow(name_, "Fog::setSkyboxFog()",
                    "Skybox fog height value out of range: "
                    "{0.0 <= VALUE <= 1.0}.");

            skybox_fog_overall_density_ = overall_density;
            skybox_fog_transition_power_ = transition_power;
            skybox_fog_height_ = height;
        }

        GLfloat getSkyboxFogOverallDensity() const
        {
            return skybox_fog_overall_density_;
        }

        GLfloat getSkyboxFogTransitionPower() const
        {
            return skybox_fog_transition_power_;
        }

        GLfloat getSkyboxFogHeight() const
        {
            return skybox_fog_height_;
        }

    protected:
        std::string name_{"core_fog"};

        GLboolean enabled_{false};

        GLfloat density_{0.05f};
        glm::vec3 color_{0.5f, 0.5f, 0.5f};

        GLfloat skybox_fog_overall_density_{0.0f};
        GLfloat skybox_fog_transition_power_{1.0f};
        GLfloat skybox_fog_height_{0.6f};
    };

    using FogPtr = std::shared_ptr<Fog>;
} // namespace puffin

#endif // PUFFIN_FOG_H