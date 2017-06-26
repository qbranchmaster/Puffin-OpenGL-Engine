//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_OBJECT_3D_MODIFIER_H
#define PUFFIN_OBJECT_3D_MODIFIER_H

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    enum class Object3DModifierType
    {
        NONE,
        OUTLINE,
    };

    class Object3DModifier
    {
    public:
        Object3DModifier(std::string name = "")
        {
            if (!name.empty())
                name_ = name;

            modifier_type_ = Object3DModifierType::NONE;

            logDebug(name_, "Object3DModifier::Object3DModifier()",
                "Object 3D modifier created.");
        }

        virtual ~Object3DModifier()
        {
            logDebug(name_, "Object3DModifier::~Object3DModifier()",
                "Object 3D modifier destroyed.");
        }

        std::string getName() const
        {
            return name_;
        }

        void enable(GLboolean state)
        {
            enabled_ = state;
        }

        GLboolean isEnabled() const
        {
            return enabled_;
        }

        Object3DModifierType getType() const
        {
            return modifier_type_;
        }

    protected:
        std::string name_{"unnamed_modifier"};

        GLboolean enabled_{false};
        Object3DModifierType modifier_type_{Object3DModifierType::NONE};
    };

    using Object3DModifierPtr = std::shared_ptr<Object3DModifier>;
} // namespace puffin

#endif // PUFFIN_OBJECT_3D_MODIFIER_H