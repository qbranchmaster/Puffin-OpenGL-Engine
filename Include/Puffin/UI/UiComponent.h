//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_UI_COMPONENT_H
#define PUFFIN_UI_COMPONENT_H

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class UiComponent
    {
    public:
        explicit UiComponent(std::string name = "")
        {
            if (!name.empty())
                name_ = name;

            logDebug(name_, "UiComponent::UiComponent()",
                "UI component created.");
        }

        virtual ~UiComponent()
        {
            logDebug(name_, "UiComponent::~UiComponent()",
                "UI component destroyed.");
        }

        std::string getName() const
        {
            return name_;
        }

        void setPosition(const glm::uvec2 &position)
        {
            position_ = position;
        }

        void setPosition(GLint x, GLint y)
        {
            position_.x = x;
            position_.y = y;
        }

        glm::uvec2 getPosition() const
        {
            return position_;
        }

    protected:
        std::string name_{"unnamed_ui_component"};

        glm::uvec2 position_{0, 0};
    };

    using UiComponentPtr = std::shared_ptr<UiComponent>;
} // namespace puffin

#endif // PUFFIN_UI_COMPONENT_H