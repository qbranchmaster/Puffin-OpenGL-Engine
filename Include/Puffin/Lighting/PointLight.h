//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_POINT_LIGHT_H
#define PUFFIN_POINT_LIGHT_H

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class PointLight
    {
    public:
        explicit PointLight(std::string name = "")
        {
            if (!name.empty())
                name_ = name;

            logDebug(name_, "PointLight::PointLight()", "Point light created.");
        }

        virtual ~PointLight()
        {
            logDebug(name_, "PointLight::~PointLight()",
                "Point light destroyed.");
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

        void setAttenuation(GLfloat linear, GLfloat quadratic)
        {
            if (linear <= 0)
                logErrorAndThrow(name_, "PointLight::setAttenuation()",
                    "Linear factor value out of range: {0 < VALUE}.");

            if (quadratic <= 0)
                logErrorAndThrow(name_, "PointLight::setAttenuation()",
                    "Quadratic factor value out of range: {0 < VALUE}.");

            linear_factor_ = linear;
            quadratic_factor_ = quadratic;
        }

        GLfloat getLinearAttenuationFactor() const
        {
            return linear_factor_;
        }

        GLfloat getQuadraticAttenuationFactor() const
        {
            return quadratic_factor_;
        }

        void setPosition(const glm::vec3 &position)
        {
            position_ = position;
        }

        glm::vec3 getPosition() const
        {
            return position_;
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

    protected:
        std::string name_{"unnamed_point_light"};

        GLboolean enabled_{false};

        GLfloat linear_factor_{0.14f};
        GLfloat quadratic_factor_{0.07f};

        glm::vec3 color_{1.0f, 1.0f, 1.0f};
        glm::vec3 position_{0.0f, 0.0f, 0.0f};
    };

    using PointLightPtr = std::shared_ptr<PointLight>;
} // namespace puffin

#endif // PUFFIN_POINT_LIGHT_H