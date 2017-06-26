//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_CAMERA_BOX_H
#define PUFFIN_CAMERA_BOX_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <memory>
#include <vector>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class CameraBox
    {
        friend class Camera;

    public:
        explicit CameraBox(std::string name = "");
        virtual ~CameraBox();

        std::string getName() const
        {
            return name_;
        }

        glm::vec3 getCenter() const
        {
            return center_;
        }

        GLfloat getWidth() const
        {
            return maxX_ - minX_;
        }

        GLfloat getHeight() const
        {
            return maxY_ - minY_;
        }

        GLfloat getLength() const
        {
            return maxZ_ - minZ_;
        }

        void update(GLfloat box_size);
        void recalculateSpace(const glm::mat4 &space_matrix);

    protected:
        glm::vec3 calculatePointPosition(const glm::vec3 &start,
            const glm::vec3 &direction, GLfloat width) const;
        void calculateBox(const glm::vec3 &center_near,
            const glm::vec3 &center_far);
        void findMinAndMax();
        void calculatePlanesSize();

        void setCameraParameters(GLfloat distance_near, GLfloat aspect,
            GLfloat fov);
        void setCameraMatrix(const glm::mat4 &view_matrix_inverted);
        void setCameraVectors(const glm::vec3 &direction, const glm::vec3 &up,
            const glm::vec3 &right);

        std::string name_{"unnamed_camera_box"};

        // Box
        bool box_updated_{false};

        GLfloat far_width_{0.0f};
        GLfloat far_height_{0.0f};
        GLfloat near_width_{0.0f};
        GLfloat near_height_{0.0f};

        GLfloat minX_{0.0f};
        GLfloat maxX_{0.0f};
        GLfloat minY_{0.0f};
        GLfloat maxY_{0.0f};
        GLfloat minZ_{0.0f};
        GLfloat maxZ_{0.0f};

        glm::vec3 center_{0.0f, 0.0f, 0.0f};

        std::vector<glm::vec3> box_points_;

        // Camera
        GLboolean recalculate_planes_size_{true};
        GLboolean camera_matrix_changed_{true};
        GLboolean camera_vectors_changed_{true};

        GLfloat fov_{0.0f};
        GLfloat aspect_{0.0f};
        GLfloat distance_near_{0.0f};
        GLfloat distance_far_{0.0f};
        glm::mat4 view_matrix_inverted_{1.0f};
        glm::vec3 forward_vector_{0.0f, 0.0f, -1.0f};
        glm::vec3 up_vector_{0.0f, 1.0f, 0.0f};
        glm::vec3 right_vector_{1.0f, 0.0f, 0.0f};
    };

    using CameraBoxPtr = std::shared_ptr<CameraBox>;
} // namespace puffin

#endif // PUFFIN_CAMERA_BOX_H