//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_MATERIAL_H
#define PUFFIN_MATERIAL_H

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "Puffin/Common/Logger.h"
#include "Puffin/Texture/Texture.h"

namespace puffin
{
    class Material
    {
    public:
        explicit Material(std::string name = "")
        {
            if (!name.empty())
                name_ = name;

            logDebug(name_, "Material::Material()", "Material created.");
        }

        virtual ~Material()
        {
            logDebug(name_, "Material::~Material()", "Material destroyed.");
        }

        std::string getName() const
        {
            return name_;
        }

        void setDiffuseTexture(TexturePtr texture)
        {
            if (!texture)
                logErrorAndThrow(name_, "Material::setDiffuseTexture()",
                    "Object [Texture] pointer not set.");

            diffuse_texture_ = texture;
        }

        TexturePtr getDiffuseTexture() const
        {
            return diffuse_texture_;
        }

        void setNormalMapTexture(TexturePtr texture)
        {
            if (!texture)
                logErrorAndThrow(name_, "Material::setNormalMapTexture()",
                    "Object [Texture] pointer not set.");

            normalmap_texture_ = texture;
        }

        TexturePtr getNormalMapTexture() const
        {
            return normalmap_texture_;
        }

        void setKa(const glm::vec3 &ka)
        {
            ka_ = glm::vec3(glm::clamp(ka.r, 0.0f, 1.0f),
                glm::clamp(ka.g, 0.0f, 1.0f),
                glm::clamp(ka.b, 0.0f, 1.0f));
        }

        glm::vec3 getKa() const
        {
            return ka_;
        }

        void setKd(const glm::vec3 &kd)
        {
            kd_ = glm::vec3(glm::clamp(kd.r, 0.0f, 1.0f),
                glm::clamp(kd.g, 0.0f, 1.0f),
                glm::clamp(kd.b, 0.0f, 1.0f));
        }

        glm::vec3 getKd() const
        {
            return kd_;
        }

        void setKs(const glm::vec3 &ks)
        {
            ks_ = glm::vec3(glm::clamp(ks.r, 0.0f, 1.0f),
                glm::clamp(ks.g, 0.0f, 1.0f),
                glm::clamp(ks.b, 0.0f, 1.0f));
        }

        glm::vec3 getKs() const
        {
            return ks_;
        }

        void setReflectivity(GLfloat value)
        {
            if (value < 0.0f)
                logErrorAndThrow(name_, "Material::setReflectivity()",
                    "Reflectivity value out of range: {0.0 <= VALUE}.");

            reflectivity_ = value;
        }

        GLfloat getReflectivity() const
        {
            return reflectivity_;
        }

        void setShininess(GLint value)
        {
            if (value < 0)
                logErrorAndThrow(name_, "Material::setShininess()",
                    "Shininess value out of range: {0 <= VALUE}.");

            shininess_ = value;
        }

        GLint getShininess() const
        {
            return shininess_;
        }

    protected:
        std::string name_{"unnamed_material"};

        GLfloat reflectivity_{0.0f};
        GLint shininess_{0};
        glm::vec3 ka_{0.0f, 0.0f, 0.0f};
        glm::vec3 kd_{0.0f, 0.0f, 0.0f};
        glm::vec3 ks_{0.0f, 0.0f, 0.0f};

        TexturePtr diffuse_texture_{nullptr};
        TexturePtr normalmap_texture_{nullptr};
    };

    using MaterialPtr = std::shared_ptr<Material>;
} // namespace puffin

#endif // PUFFIN_MATERIAL_H