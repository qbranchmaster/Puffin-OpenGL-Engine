//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_DIRECTIONAL_LIGHT_H
#define PUFFIN_DIRECTIONAL_LIGHT_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class DirectionalLight
    {
        friend class Object3DRenderer;
        friend class ShadowMapRenderer;

    public:
        DirectionalLight();
        virtual ~DirectionalLight();

        void enable(GLboolean state)
        {
            enabled_ = state;
        }

        GLboolean isEnabled() const
        {
            return enabled_;
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

        void setDirection(const glm::vec3 &direction)
        {
            direction_ = glm::normalize(direction);
        }

        glm::vec3 getDirection() const
        {
            return direction_;
        }

    protected:
        glm::mat4 getProjectionViewMatrix()
        {
            if (matrices_changed_)
            {
                pv_matrix_ = projection_matrix_ * view_matrix_;
                matrices_changed_ = false;
            }

            return pv_matrix_;
        }

        void calculateViewMatrix(const glm::vec3 &center_point)
        {
            view_matrix_ = glm::lookAt(center_point, center_point + direction_,
                glm::vec3(0.0f, 1.0f, 0.0f));

            matrices_changed_ = true;
        }

        glm::mat4 getViewMatrix() const
        {
            return view_matrix_;
        }

        void calculateProjectionMatrix(const glm::vec3 &ortho_box)
        {
            projection_matrix_ = glm::ortho(-ortho_box.x, ortho_box.x,
                -ortho_box.y, ortho_box.y, -ortho_box.z, ortho_box.z);

            matrices_changed_ = true;
        }

        glm::mat4 getProjectionMatrix() const
        {
            return projection_matrix_;
        }

        std::string name_{"core_directional_light"};
        GLboolean enabled_{false};

        glm::vec3 color_{1.0f, 1.0f, 1.0f};
        glm::vec3 direction_{-1.0f, -1.0f, -1.0f};

        GLboolean matrices_changed_{false};
        glm::mat4 pv_matrix_{1.0f};
        glm::mat4 view_matrix_{1.0f};
        glm::mat4 projection_matrix_{1.0f};
    };

    using DirectionalLightPtr = std::shared_ptr<DirectionalLight>;
} // namespace puffin

#endif // PUFFIN_DIRECTIONAL_LIGHT_H