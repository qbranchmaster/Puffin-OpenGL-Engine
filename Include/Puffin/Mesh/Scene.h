//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_SCENE_H
#define PUFFIN_SCENE_H

#include <GL/glew.h>

#include <memory>
#include <vector>

#include "Puffin/Common/Logger.h"
#include "Puffin/Mesh/Object3D.h"
#include "Puffin/Mesh/ParticleSystem.h"
#include "Puffin/Mesh/Skybox.h"
#include "Puffin/Mesh/WaterTile.h"
#include "Puffin/UI/Text.h"

namespace puffin
{
    class Scene
    {
    public:
        explicit Scene(std::string name);
        virtual ~Scene();

        std::string getName() const
        {
            return name_;
        }

        void enable(GLboolean state);

        GLboolean isEnabled() const
        {
            return enabled_;
        }

        void addObject3D(Object3DPtr src_object);
        void addParticleSystem(ParticleSystemPtr particle_system_src);
        void addSkybox(SkyboxPtr src_skybox, GLboolean activate = true);
        void addText(TextPtr text_src);
        void addWaterTile(WaterTilePtr water_tile_src);

        std::vector<Object3DPtr> getObject3DContainer() const
        {
            return object3d_container_;
        }

        std::vector<ParticleSystemPtr> getParticleSystemContainer() const
        {
            return particle_system_container_;
        }

        std::vector<SkyboxPtr> getSkyboxContainer() const
        {
            return skybox_container_;
        }

        std::vector<TextPtr> getTextContainer() const
        {
            return text_container_;
        }

        std::vector<WaterTilePtr> getWaterTileContainer() const
        {
            return water_tile_container_;
        }

        SkyboxPtr getActiveSkybox() const
        {
            return active_skybox_;
        }

    protected:
        std::string name_{"unnamed_scene"};

        GLboolean enabled_{true};

        SkyboxPtr active_skybox_{nullptr};

        std::vector<Object3DPtr> object3d_container_;
        std::vector<ParticleSystemPtr> particle_system_container_;
        std::vector<SkyboxPtr> skybox_container_;
        std::vector<TextPtr> text_container_;
        std::vector<WaterTilePtr> water_tile_container_;
    };

    using ScenePtr = std::shared_ptr<Scene>;
} // namespace puffin

#endif // PUFFIN_SCENE_H