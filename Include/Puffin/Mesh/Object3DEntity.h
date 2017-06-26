//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_OBJECT_3D_ENTITY_H
#define PUFFIN_OBJECT_3D_ENTITY_H

#include <GL/glew.h>

#include <memory>

#include "Puffin/Common/Logger.h"
#include "Puffin/Mesh/Material.h"

namespace puffin
{
    class Object3DEntity
    {
    public:
        explicit Object3DEntity(std::string name = "")
        {
            if (!name.empty())
                name_ = name;

            logDebug(name_, "Object3DEntity::Object3DEntity()",
                "Object 3D entity created.");
        }

        virtual ~Object3DEntity()
        {
            logDebug(name_, "Object3DEntity::~Object3DEntity()",
                "Object 3D entity destroyed.");
        }

        void setIndicesCount(GLint count)
        {
            if (count < 0)
                logErrorAndThrow(name_, "Object3DEntity::setIndicesCount()",
                    "Indices count value out of range: {0 <= VALUE}.");

            indices_count_ = count;
        }

        GLint getIndicesCount() const
        {
            return indices_count_;
        }

        void setStartingIndex(GLint index)
        {
            if (index < 0)
                logErrorAndThrow(name_, "Object3DEntity::setStartingIndex()",
                    "Starting index value out of range: {0 <= VALUE}.");

            starting_index_ = index;
        }

        GLint getStartingIndex() const
        {
            return starting_index_;
        }

        void setVerticesCount(GLint count)
        {
            if (count < 0)
                logErrorAndThrow(name_, "Object3DEntity::setVerticesCount()",
                    "Vertices count value out of range: {0 <= VALUE}.");

            vertices_count_ = count;
        }

        GLint getVerticesCount() const
        {
            return vertices_count_;
        }

        void setMaterial(MaterialPtr material)
        {
            if (!material)
                logErrorAndThrow(name_, "Object3DEntity::setMaterial()",
                    "Object [Material] pointer not set.");

            material_ = material;
        }

        MaterialPtr getMaterial() const
        {
            return material_;
        }

    protected:
        std::string name_{"unnamed_object3d_entity"};

        GLint starting_index_{0};
        GLint indices_count_{0};
        GLint vertices_count_{0};

        MaterialPtr material_{nullptr};
    };

    using Object3DEntityPtr = std::shared_ptr<Object3DEntity>;
} // namespace puffin

#endif // PUFFIN_OBJECT_3D_ENTITY_H