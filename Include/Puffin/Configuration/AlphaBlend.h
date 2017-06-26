//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_ALPHA_BLEND_H
#define PUFFIN_ALPHA_BLEND_H

#include <GL/glew.h>

#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    enum class BlendFunction
    {
        ADDITIVE,
        NORMAL,
    };

    class AlphaBlend
    {
    public:
        AlphaBlend()
        {
            glGetBooleanv(GL_BLEND, &enabled_);

            GLint value = 0;
            glGetIntegerv(GL_BLEND_SRC_ALPHA, &value);
            switch (value)
            {
            case GL_ONE:
                blend_function_ = BlendFunction::ADDITIVE;
                break;
            case GL_ONE_MINUS_SRC_ALPHA:
                blend_function_ = BlendFunction::NORMAL;
                break;
            }

            logDebug(name_, "AlphaBlend::AlphaBlend()", "Alpha blend created.");
        }

        virtual ~AlphaBlend()
        {
            logDebug(name_, "AlphaBlend::~AlphaBlend()",
                "Alpha blend destroyed.");
        }

        std::string getName() const
        {
            return name_;
        }

        void enable(GLboolean state)
        {
            if (state == enabled_)
                return;

            if (state)
                glEnable(GL_BLEND);
            else
                glDisable(GL_BLEND);

            enabled_ = state;
        }

        GLboolean isEnabled() const
        {
            return enabled_;
        }

        void setBlendFunction(BlendFunction function)
        {
            if (function == blend_function_)
                return;

            switch (function)
            {
            case BlendFunction::ADDITIVE:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            case BlendFunction::NORMAL:
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
            }

            blend_function_ = function;
        }

    protected:
        std::string name_{"core_alpha_blend"};

        GLboolean enabled_{false};
        BlendFunction blend_function_{BlendFunction::NORMAL};
    };

    using AlphaBlendPtr = std::shared_ptr<AlphaBlend>;
} // namespace puffin

#endif // PUFFIN_ALPHA_BLEND_H