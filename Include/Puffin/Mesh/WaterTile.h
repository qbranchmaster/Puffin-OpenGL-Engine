//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//--------------------------------------------------------------
#ifndef PUFFIN_WATER_TILE_H
#define PUFFIN_WATER_TILE_H

#include "Puffin/Mesh/BaseMesh.h"

namespace puffin
{
    class WaterTile : public BaseMesh
    {
        friend class MeshManager;
        friend class WaterRenderer;

    public:
        explicit WaterTile(std::string name = "") : BaseMesh(name)
        {
            logDebug(name_, "WaterTile::WaterTile()", "Water tile created.");
        }

        virtual ~WaterTile()
        {
            logDebug(name_, "WaterTile::~WaterTile()", "Water tile destroyed.");
        }

        void setWaterColor(const glm::vec3 &color)
        {
            water_color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getWaterColor() const
        {
            return water_color_;
        }

        void setWaveStrength(GLfloat strength)
        {
            if (strength < 0.0f)
                logErrorAndThrow(name_, "WaterTile::setWaveStrength()",
                    "Wave strength value out of range: {0.0 <= VALUE}.");

            wave_strength_ = strength;
        }

        GLfloat getWaveStrength() const
        {
            return wave_strength_;
        }

        void setWaveSpeed(GLfloat speed)
        {
            if (speed < 0.0f)
                logErrorAndThrow(name_, "WaterTile::setWaveSpeed()",
                    "Wave speed value out of range: {0.0 <= VALUE}.");

            wave_speed_ = speed;
        }

        GLfloat getWaveSpeed() const
        {
            return wave_speed_;
        }

        void setShininess(GLint shininess)
        {
            if (shininess < 0)
                logErrorAndThrow(name_, "WaterTile::setShininess()",
                    "Shininess value out of range: {0 <= VALUE}.");

            shininess_ = shininess;
        }

        GLint getShininess() const
        {
            return shininess_;
        }

        void setTextureTiling(GLint tiling)
        {
            if (tiling < 1)
                logErrorAndThrow(name_, "WaterTile::setTextureTiling()",
                    "Texture tiling value out of range: {1 <= VALUE}.");

            texture_tiling_ = tiling;
        }

        GLint getTextureTiling() const
        {
            return texture_tiling_;
        }

    protected:
        void draw(GLuint index = 0)
        {
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glm::vec3 water_color_{0.0f, 0.3f, 0.5f};
        GLfloat wave_strength_{0.04f};
        GLfloat wave_speed_{0.01f};
        GLint shininess_{10};

        GLfloat move_factor_{0.0f};

        GLint texture_tiling_{1};
        TexturePtr dudv_texture_{nullptr};
        TexturePtr normal_map_texture_{nullptr};
    };

    using WaterTilePtr = std::shared_ptr<WaterTile>;
} // namespace puffin

#endif // PUFFIN_WATER_TILE_H