//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Manager/MeshManager.h"

using namespace puffin;

MeshManager::MeshManager(StateMachinePtr state_machine,
    TextureManagerPtr texture_manager) : BaseManager("core_mesh_manager")
{
    if (!state_machine)
        logErrorAndThrow(name_, "MeshManager::MeshManager()",
            "Object [StateMachine] pointer not set.");

    if (!texture_manager)
        logErrorAndThrow(name_, "MeshManager::MeshManager()",
            "Object [TextureManager] pointer not set.");

    state_machine_ = state_machine;
    texture_manager_ = texture_manager;

    logDebug(name_, "MeshManager::MeshManager()", "Mesh manager created.");
}

MeshManager::~MeshManager()
{
    logDebug(name_, "MeshManager::~MeshManager()", "Mesh manager destroyed.");
}

Object3DPtr MeshManager::createObject3D(std::string object3d_name)
{
    Object3DPtr object(new Object3D(object3d_name));
    object_3d_container_.push_back(object);
    return object;
}

SkyboxPtr MeshManager::createSkybox(std::array<std::string, 6> textures,
    std::string skybox_name)
{
    TexturePtr cube_texture = texture_manager_->loadTextureCube(textures,
        skybox_name + "_cube_texture");
    texture_manager_->setTextureFilter(cube_texture, TextureFilter::BILINEAR);

    SkyboxPtr skybox(new Skybox(skybox_name));
    skybox->setTexture(cube_texture);

    std::vector<GLfloat> data = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f,  1.0f, 1.0f,
        1.0f,  1.0f, 1.0f,
        1.0f,  1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f,  1.0f,
        1.0f, 1.0f,  1.0f,
        -1.0f, 1.0f,  1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    setMeshData(skybox, data, VertexDataType::POSITION, false);

    skybox_container_.push_back(skybox);
    return skybox;
}

Object3DPtr MeshManager::loadObject3D(std::string file_path,
    std::string object3d_name)
{
    if (file_path.empty())
        logErrorAndThrow(name_, "MeshManager::loadObject3D()",
            "Empty object 3D file path.");

    // TODO: Check if object already exists

    Assimp::Importer mesh_importer;
    const aiScene *scene = mesh_importer.ReadFile(file_path.c_str(),
        aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);

    if (!scene)
    {
        logError(name_, "MeshManager::loadObject3D()", "Importer message: " +
            std::string(mesh_importer.GetErrorString()) + ".");
        logErrorAndThrow(name_, "MeshManager::loadObject3D()", "File [" +
            file_path + "] loading error.");
    }

    GLint vertices_count = 0;
    GLint indices_count = 0;

    std::vector<GLfloat> v_positions_;
    std::vector<GLfloat> v_normals_;
    std::vector<GLfloat> v_tex_coords_;
    std::vector<GLfloat> v_tangents_;
    std::vector<GLfloat> v_bitangents_;
    std::vector<GLuint> v_indices_;

    std::vector<Object3DEntityPtr> entities_;

    for (GLuint m = 0; m < scene->mNumMeshes; m++)
    {
        aiMesh *mesh = scene->mMeshes[m];

        Object3DEntityPtr entity(new Object3DEntity());

        for (GLuint v = 0; v < mesh->mNumVertices; v++)
        {
            aiVector3D vp(0.0f, 0.0f, 0.0f);
            aiVector3D vn(0.0f, 1.0f, 0.0f);
            aiVector3D vt(0.0f, 0.0f, 0.0f);
            aiVector3D tangent(0.0f, 0.0f, 0.0f);
            aiVector3D bitangent(0.0f, 0.0f, 0.0f);

            if (mesh->HasPositions())
                vp = mesh->mVertices[v];

            if (mesh->HasNormals())
                vn = mesh->mNormals[v];

            if (mesh->HasTextureCoords(0))
                vt = mesh->mTextureCoords[0][v];

            if (mesh->HasTangentsAndBitangents())
            {
                tangent = mesh->mTangents[v];
                bitangent = mesh->mBitangents[v];
            }

            v_positions_.push_back(vp.x);
            v_positions_.push_back(vp.y);
            v_positions_.push_back(vp.z);

            v_normals_.push_back(vn.x);
            v_normals_.push_back(vn.y);
            v_normals_.push_back(vn.z);

            v_tex_coords_.push_back(vt.x);
            v_tex_coords_.push_back(vt.y);

            v_tangents_.push_back(tangent.x);
            v_tangents_.push_back(tangent.y);
            v_tangents_.push_back(tangent.z);

            v_bitangents_.push_back(bitangent.x);
            v_bitangents_.push_back(bitangent.y);
            v_bitangents_.push_back(bitangent.z);
        }

        for (GLuint f = 0; f < mesh->mNumFaces; f++)
        {
            aiFace *face = &mesh->mFaces[f];

            for (GLuint i = 0; i < face->mNumIndices; i++)
            {
                v_indices_.push_back(face->mIndices[i] + vertices_count);
                entity->setIndicesCount(entity->getIndicesCount() + 1);
            }
        }

        vertices_count += mesh->mNumVertices;
        entity->setStartingIndex(indices_count);
        indices_count += entity->getIndicesCount();

        // Material loading
        if (scene->HasMaterials())
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            MaterialPtr mat(new Material(name_ + "_material_" +
                std::to_string(mesh->mMaterialIndex)));

            aiString texture_path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path)
                == AI_SUCCESS)
            {
                TexturePtr diffuse_tex = texture_manager_->loadTexture2D(
                    processTexturePath(file_path, texture_path), true, name_ +
                    "_texture_diffuse_" + std::to_string(mesh->mMaterialIndex));
                texture_manager_->setTextureFilter(diffuse_tex,
                    TextureFilter::TRILINEAR);
                mat->setDiffuseTexture(diffuse_tex);
            }

            if (material->GetTexture(aiTextureType_HEIGHT, 0, &texture_path) ==
                AI_SUCCESS)
            {
                TexturePtr normalmap_tex = texture_manager_->loadTexture2D(
                    processTexturePath(file_path, texture_path), true, name_ +
                    "_texture_normalmap_" +
                    std::to_string(mesh->mMaterialIndex));
                texture_manager_->setTextureFilter(normalmap_tex,
                    TextureFilter::TRILINEAR);
                mat->setNormalMapTexture(normalmap_tex);
            }

            aiColor3D kd;
            if (material->Get(AI_MATKEY_COLOR_DIFFUSE, kd) == AI_SUCCESS)
                mat->setKd(glm::vec3(kd.r, kd.g, kd.b));

            aiColor3D ka;
            if (material->Get(AI_MATKEY_COLOR_AMBIENT, ka) == AI_SUCCESS)
                mat->setKa(glm::vec3(ka.r, ka.g, ka.b));

            aiColor3D ks;
            if (material->Get(AI_MATKEY_COLOR_SPECULAR, ks) == AI_SUCCESS)
                mat->setKs(glm::vec3(ks.r, ks.g, ks.b));

            // Shininess needs to be divided by 4, because Assimp
            // multiplies it by 4
            GLfloat shininess = 0;
            if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
                mat->setShininess(static_cast<int>(shininess) / 4);

            GLfloat reflectivity = 0;
            if (material->Get(AI_MATKEY_REFLECTIVITY, reflectivity) ==
                AI_SUCCESS)
                mat->setReflectivity(reflectivity);

            entity->setMaterial(mat);
        }

        entities_.push_back(entity);
    }

    logInfo(name_, "MeshManager::loadObject3D()", "Model 3D information:\n"
        "Meshes count: " + std::to_string(scene->mNumMeshes) + "\n"
        "Vertices count: " + std::to_string(vertices_count));

    Object3DPtr model(new Object3D(object3d_name));
    model->entities_ = entities_;
    state_machine_->bindMesh(model);
    setMeshData(model, v_positions_, VertexDataType::POSITION, false);
    setMeshData(model, v_tex_coords_, VertexDataType::TEXTURE_COORD, false);
    setMeshData(model, v_normals_, VertexDataType::NORMAL_VECTOR, false);
    setMeshData(model, v_tangents_, VertexDataType::TANGENT, false);
    setMeshData(model, v_bitangents_, VertexDataType::BITANGET, false);
    setMeshIndices(model, v_indices_);
    model->useIndices(true);

    object_3d_container_.push_back(model);

    return model;
}

std::string MeshManager::processTexturePath(std::string model_file_path,
    const aiString &texture_path)
{
    std::size_t slash_pos = model_file_path.find_last_of("/\\");
    std::string path = model_file_path.substr(0, slash_pos);
    std::string name = texture_path.C_Str();
    if (name[0] == '/')
        name.erase(0, 1);

    std::string file_path = path + "/" + name;
    return file_path;
}

WaterTilePtr MeshManager::createWaterTile(std::string dudv_texture,
    std::string normal_map_texture, std::string tile_name)
{
    if (dudv_texture.empty())
    {
        dudv_texture = "Data/Textures/WaterDuDv.png";
        logWarning(name_, "MeshManager::createWaterTile()",
            "DuDv texture not set. Default will be used.");
    }

    if (normal_map_texture.empty())
    {
        normal_map_texture = "Data/Textures/WaterNormalMap.png";
        logWarning(name_, "MeshManager::createWaterTile()",
            "Normal map texture not set. Default will be used.");
    }

    TexturePtr dudv = texture_manager_->loadTexture2D(dudv_texture);
    TexturePtr normal_map = texture_manager_->loadTexture2D(normal_map_texture);

    WaterTilePtr tile(new WaterTile(tile_name));
    tile->dudv_texture_ = dudv;
    tile->normal_map_texture_ = normal_map;

    std::vector<GLfloat> positions = {
        -1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f,  1.0f,
        1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
        -1.0f, 0.0f,  1.0f,
        1.0f, 0.0f,  1.0f
    };

    setMeshData(tile, positions, VertexDataType::POSITION, false);
    water_tile_container_.push_back(tile);

    return tile;
}

void MeshManager::setMeshData(BaseMeshPtr mesh, std::vector<GLfloat> data,
    VertexDataType vertex_data_type, GLboolean dynamic_draw)
{
    state_machine_->bindMesh(mesh);

    bool created = false;
    if (mesh->data_buffers_[vertex_data_type] == 0)
    {
        glGenBuffers(1, &mesh->data_buffers_[vertex_data_type]);
        created = true;
    }

    if (vertex_data_type != VertexDataType::INDEX)
        glBindBuffer(GL_ARRAY_BUFFER, mesh->data_buffers_[vertex_data_type]);

    switch (vertex_data_type)
    {
    case VertexDataType::POSITION:
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat),
            data.data(), dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        if (created)
        {
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(0);
        }
        break;
    case VertexDataType::TEXTURE_COORD:
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat),
            data.data(), dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        if (created)
        {
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(2);
        }
        break;
    case VertexDataType::NORMAL_VECTOR:
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat),
            data.data(), dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        if (created)
        {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(1);
        }
        break;
    case VertexDataType::TANGENT:
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat),
            data.data(), dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        if (created)
        {
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(3);
        }
        break;
    case VertexDataType::BITANGET:
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat),
            data.data(), dynamic_draw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        if (created)
        {
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(4);
        }
        break;
    }
}

void MeshManager::setMeshIndices(BaseMeshPtr mesh, std::vector<GLuint> data)
{
    state_machine_->bindMesh(mesh);

    if (mesh->data_buffers_[VertexDataType::INDEX] == 0)
        glGenBuffers(1, &mesh->data_buffers_[VertexDataType::INDEX]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->data_buffers_[VertexDataType::
        INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint),
        data.data(), GL_STATIC_DRAW);
}

ParticleSystemPtr MeshManager::createParticleSystem(const glm::vec3 &position,
    std::string texture_path, GLint atlas_size, GLboolean animate, GLint pps)
{
    ParticleSystemPtr system(new ParticleSystem(texture_manager_, position,
        texture_path, atlas_size, animate, pps));
    particle_system_container_.push_back(system);
    return system;
}