//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_POLYGON_MODE_H
#define PUFFIN_POLYGON_MODE_H

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class PolygonMode
    {
    public:
        PolygonMode()
        {
            GLint mode = 0;
            glGetIntegerv(GL_POLYGON_MODE, &mode);
            switch (mode)
            {
            case GL_LINE:
                enabled_ = true;
                break;
            case GL_FILL:
                enabled_ = false;
                break;
            }

            logDebug(name_, "PolygonMode::PolygonMode()",
                "Polygon mode created.");
        }

        virtual ~PolygonMode()
        {
            logDebug(name_, "PolygonMode::~PolygonMode()",
                "Polygon mode destroyed.");
        }

        void enable(GLboolean state)
        {
            if (state == enabled_)
                return;

            glPolygonMode(GL_FRONT_AND_BACK, state ? GL_LINE : GL_FILL);
            enabled_ = state;
        }

        GLboolean isEnabled() const
        {
            return enabled_;
        }

        void setLinesColor(const glm::vec3 &color)
        {
            lines_color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getLinesColor() const
        {
            return lines_color_;
        }

        void usePipeline(GLboolean state)
        {
            use_pipeline_ = state;
        }

        GLboolean isUsingPipeline() const
        {
            return use_pipeline_;
        }

    protected:
        std::string name_{"core_polygon_mode"};

        GLboolean enabled_{false};
        GLboolean use_pipeline_{false};
        glm::vec3 lines_color_{1.0f, 0.0f, 0.0f};
    };

    using PolygonModePtr = std::shared_ptr<PolygonMode>;
} // namespace puffin

#endif // PUFFIN_POLYGON_MODE_H