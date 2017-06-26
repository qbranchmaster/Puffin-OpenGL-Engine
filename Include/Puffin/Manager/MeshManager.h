//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_MESH_MANAGER_H
#define PUFFIN_MESH_MANAGER_H

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <array>
#include <map>
#include <vector>

#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Manager/BaseManager.h"
#include "Puffin/Manager/TextureManager.h"
#include "Puffin/Mesh/Skybox.h"
#include "Puffin/Mesh/Object3D.h"
#include "Puffin/Mesh/ParticleSystem.h"
#include "Puffin/Mesh/WaterTile.h"

namespace puffin
{
    class MeshManager : public BaseManager
    {
    public:
        MeshManager(StateMachinePtr state_machine,
            TextureManagerPtr texture_manager);
        virtual ~MeshManager();

        Object3DPtr createObject3D(std::string object3d_name = "");
        Object3DPtr loadObject3D(std::string file_path,
            std::string object3d_name = "");
        SkyboxPtr createSkybox(std::array<std::string, 6> textures,
            std::string skybox_name = "");
        WaterTilePtr createWaterTile(std::string dudv_texture = "",
            std::string normal_map_texture = "", std::string tile_name = "");
        ParticleSystemPtr createParticleSystem(const glm::vec3 &position,
            std::string texture_path, GLint atlas_size, GLboolean animate,
            GLint pps);

        void setMeshData(BaseMeshPtr mesh, std::vector<GLfloat> data,
            VertexDataType vertex_data_type, GLboolean dynamic_draw);
        void setMeshIndices(BaseMeshPtr mesh, std::vector<GLuint> data);

    protected:
        std::string processTexturePath(std::string model_file_path,
            const aiString &texture_path);

        std::vector<Object3DPtr> object_3d_container_;
        std::vector<ParticleSystemPtr> particle_system_container_;
        std::vector<SkyboxPtr> skybox_container_;
        std::vector<WaterTilePtr> water_tile_container_;

        StateMachinePtr state_machine_{nullptr};
        TextureManagerPtr texture_manager_{nullptr};
    };

    using MeshManagerPtr = std::shared_ptr<MeshManager>;
} // namespace puffin

#endif // PUFFIN_MESH_MANAGER_H