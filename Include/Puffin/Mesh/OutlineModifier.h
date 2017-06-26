//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_OUTLINE_MODIFIER_H
#define PUFFIN_OUTLINE_MODIFIER_H

#include "Puffin/Mesh/Object3DModifier.h"

namespace puffin
{
    class Outline : public Object3DModifier
    {
    public:
        Outline(std::string name = "") : Object3DModifier(name)
        {
            modifier_type_ = Object3DModifierType::OUTLINE;

            logDebug(name_, "Outline::Outline()", "Outline modifier created.");
        }

        virtual ~Outline()
        {
            logDebug(name_, "Outline::~Outline()",
                "Outline modifier destroyed.");
        }

        void setAlwaysVisible(GLboolean state)
        {
            always_visible_ = state;
        }

        GLboolean isAlwaysVisible() const
        {
            return always_visible_;
        }

        void setColor(const glm::vec3 &color)
        {
            color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getColor() const
        {
            return color_;
        }

        void setScale(GLfloat scale)
        {
            scale_ = scale;
        }

        GLfloat getScale() const
        {
            return scale_;
        }

    protected:
        GLboolean always_visible_{false};
        GLfloat scale_{1.0f};
        glm::vec3 color_{1.0f, 0.0f, 0.0f};
    };

    using OutlinePtr = std::shared_ptr<Outline>;
} // namespace puffin

#endif // PUFFIN_OUTLINE_MODIFIER_H
