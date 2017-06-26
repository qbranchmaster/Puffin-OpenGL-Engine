//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_BASE_MESH_H
#define PUFFIN_BASE_MESH_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <memory>
#include <vector>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    enum class VertexDataType
    {
        POSITION,
        NORMAL_VECTOR,
        TEXTURE_COORD,
        TANGENT,
        BITANGET,
        INDEX,
    };

    class BaseMesh
    {
        friend class MeshManager;
        friend class StateMachine;

    public:
        explicit BaseMesh(std::string name = "");
        virtual ~BaseMesh();

        std::string getName() const
        {
            return name_;
        }

        glm::mat4 getRotationMatrix() const
        {
            return rotation_matrix_;
        }

        glm::mat4 getScaleMatrix() const
        {
            return scale_matrix_;
        }

        glm::mat4 getTranslationMatrix() const
        {
            return translation_matrix_;
        }

        glm::mat4 getModelMatrix()
        {
            if (model_matrix_changed_)
            {
                model_matrix_ = translation_matrix_ * rotation_matrix_ *
                    scale_matrix_;
                model_matrix_changed_ = false;
            }

            return model_matrix_;
        }

        void setScale(const glm::vec3 &scale)
        {
            scale_ = scale;
            scale_matrix_ = glm::scale(glm::mat4(1.0f), scale_);

            model_matrix_changed_ = true;
        }

        glm::vec3 getScale() const
        {
            return scale_;
        }

        void setPosition(const glm::vec3 &position)
        {
            translation_matrix_ = glm::mat4(1.0f);
            position_ = glm::vec3(0.0f, 0.0f, 0.0f);
            translate(position);
        }

        void translate(const glm::vec3 &translation)
        {
            translation_matrix_ = glm::translate(translation_matrix_,
                translation);
            position_ += translation;
            model_matrix_changed_ = true;
        }

        void zeroTranslation()
        {
            translation_matrix_ = glm::mat4(1.0f);
            model_matrix_changed_ = true;
        }

        glm::vec3 getPosition() const
        {
            return position_;
        }

        void rotate(GLfloat angle, const glm::vec3 &axis)
        {
            rotation_matrix_ = glm::rotate(rotation_matrix_, angle, axis);
            model_matrix_changed_ = true;
        }

        void setRotationAngle(GLfloat angle, const glm::vec3 &axis)
        {
            rotation_matrix_ = glm::mat4(1.0f);
            rotate(angle, axis);
        }

        void zeroRotation()
        {
            rotation_matrix_ = glm::mat4(1.0f);
            model_matrix_changed_ = true;
        }

    protected:
        virtual void draw(GLuint index = 0) = 0;

        std::string name_{"unnamed_base_mesh"};

        GLuint handle_{0};
        std::map<VertexDataType, GLuint> data_buffers_;

        GLboolean model_matrix_changed_{false};
        glm::mat4 model_matrix_{1.0f};
        glm::mat4 rotation_matrix_{1.0f};
        glm::mat4 scale_matrix_{1.0f};
        glm::mat4 translation_matrix_{1.0f};
        glm::vec3 position_{0.0f, 0.0f, 0.0f};
        glm::vec3 scale_{1.0f, 1.0f, 1.0f};
    };

    using BaseMeshPtr = std::shared_ptr<BaseMesh>;
} // namespace puffin

#endif // PUFFIN_BASE_MESH_H