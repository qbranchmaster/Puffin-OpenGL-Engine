//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_SKYBOX_H
#define PUFFIN_SKYBOX_H

#include "Puffin/Mesh/BaseMesh.h"
#include "Puffin/Texture/Texture.h"

namespace puffin
{
    class Skybox : public BaseMesh
    {
        friend class MeshManager;
        friend class SkyboxRenderer;

    public:
        explicit Skybox(std::string name = "");
        virtual ~Skybox();

        void setTexture(TexturePtr texture);

        TexturePtr getTexture() const
        {
            return texture_;
        }

        void setFilterColor(const glm::vec3 &color)
        {
            filter_color_ = color;
        }

        glm::vec3 getFilterColor() const
        {
            return filter_color_;
        }

        void rotate(GLfloat angle)
        {
            BaseMesh::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
            rotation_angle_ += angle;
        }

        void setRotationAngle(GLfloat angle)
        {
            BaseMesh::setRotationAngle(angle, glm::vec3(0.0f, 1.0f, 0.0f));
            rotation_angle_ = angle;
        }

        GLfloat getRotationAngle() const
        {
            return rotation_angle_;
        }

    protected:
        void draw(GLuint index = 0)
        {
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glm::vec3 filter_color_{1.0f, 1.0f, 1.0f};
        GLfloat rotation_angle_{0.0f};

        TexturePtr texture_{nullptr};
    };

    using SkyboxPtr = std::shared_ptr<Skybox>;
} // namespace puffin

#endif // PUFFIN_SKYBOX_H