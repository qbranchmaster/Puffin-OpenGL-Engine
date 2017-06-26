//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_OBJECT_3D_H
#define PUFFIN_OBJECT_3D_H

#include "Puffin/Mesh/BaseMesh.h"
#include "Puffin/Mesh/Object3DEntity.h"
#include "Puffin/Mesh/OutlineModifier.h"

namespace puffin
{
    class Object3D : public BaseMesh
    {
        friend class FontRenderer;
        friend class MasterRenderer;
        friend class MeshManager;
        friend class Object3DRenderer;
        friend class ParticleRenderer;

    public:
        explicit Object3D(std::string name = "");
        virtual ~Object3D();

        void addEntity(Object3DEntityPtr entity)
        {
            entities_.push_back(entity);
        }

        GLuint getEntitiesCount() const
        {
            return entities_.size();
        }

        Object3DEntityPtr getEntity(GLuint index) const
        {
            if (index >= entities_.size())
                logErrorAndThrow(name_, "Object3D::getEntity()",
                    "Entity index value out of range.");

            return entities_[index];
        }

        void useIndices(GLboolean state)
        {
            use_indices_ = state;
        }

        void addModifier(Object3DModifierPtr modifier);

        Object3DModifierPtr getModifier(Object3DModifierType modifier_type)
        {
            return modifiers_[modifier_type];
        }

    protected:
        void draw(GLuint index = 0)
        {
            if (index >= entities_.size())
                logErrorAndThrow(name_, "Object3D::draw()",
                    "Entity index value out of range.");

            auto entity = entities_[index];

            if (use_indices_)
                glDrawElements(GL_TRIANGLES, entity->getIndicesCount(),
                    GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(GLint) *
                        entity->getStartingIndex()));
            else
                glDrawArrays(GL_TRIANGLES, 0, entity->getVerticesCount());
        }

        GLboolean use_indices_{false};
        std::vector<Object3DEntityPtr> entities_;
        std::map<Object3DModifierType, Object3DModifierPtr> modifiers_;
    };

    using Object3DPtr = std::shared_ptr<Object3D>;
} // namespace puffin

#endif // PUFFIN_OBJECT_3D_H