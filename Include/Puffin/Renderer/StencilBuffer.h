//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_STENCIL_BUFFER_H
#define PUFFIN_STENCIL_BUFFER_H

#include <GL/glew.h>

#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    enum class StencilBufferAction
    {
        REPLACE,
        KEEP,
    };

    class StencilBuffer
    {
    public:
        StencilBuffer()
        {
            glGetBooleanv(GL_STENCIL_TEST, &enabled_);

            GLint value = 0;
            glGetIntegerv(GL_STENCIL_WRITEMASK, &value);
            if (static_cast<GLushort>(value) != 0)
                drawing_enabled_ = true;
            else
                drawing_enabled_ = false;

            logDebug(name_, "StencilBuffer::StencilBuffer()",
                "Stencil buffer created.");
        }

        virtual ~StencilBuffer()
        {
            logDebug(name_, "StencilBuffer::~StencilBuffer()",
                "Stencil buffer destroyed.");
        }

        void enable(GLboolean state)
        {
            if (state == enabled_)
                return;

            if (state)
                glEnable(GL_STENCIL_TEST);
            else
                glDisable(GL_STENCIL_TEST);

            enabled_ = state;
        }

        GLboolean isEnabled() const
        {
            return enabled_;
        }

        void enableDrawing(GLboolean state)
        {
            glStencilMask(state ? 0xFF : 0x00);
            drawing_enabled_ = state;
        }

        GLboolean isDrawingEnabled() const
        {
            return drawing_enabled_;
        }

        void setAction(StencilBufferAction action) const
        {
            switch (action)
            {
            case StencilBufferAction::REPLACE:
                glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                break;
            case StencilBufferAction::KEEP:
                glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            }
        }

        void passesAlways(GLint value) const
        {
            glStencilFunc(GL_ALWAYS, value, 0xFF);
        }

        void passesNotEqual(GLint value) const
        {
            glStencilFunc(GL_NOTEQUAL, value, 0xFF);
        }

    protected:
        std::string name_{"core_stencil_buffer"};

        GLboolean drawing_enabled_{false};
        GLboolean enabled_{false};
    };

    using StencilBufferPtr = std::shared_ptr<StencilBuffer>;
} // namespace puffin

#endif // PUFFIN_STENCIL_BUFFER_H