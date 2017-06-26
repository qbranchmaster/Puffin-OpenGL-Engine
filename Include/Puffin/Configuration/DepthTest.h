//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_DEPTH_TEST_H
#define PUFFIN_DEPTH_TEST_H

#include <GL/glew.h>

#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class DepthTest
    {
    public:
        DepthTest()
        {
            glGetBooleanv(GL_DEPTH_TEST, &enabled_);
            glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_mask_enabled_);

            logDebug(name_, "DepthTest::DepthTest()", "Depth test created.");
        }

        virtual ~DepthTest()
        {
            logDebug(name_, "DepthTest::~DepthTest()", "Depth test destroyed.");
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
                glEnable(GL_DEPTH_TEST);
            else
                glDisable(GL_DEPTH_TEST);

            enabled_ = state;
        }

        GLboolean isEnabled() const
        {
            return enabled_;
        }

        void enableDepthMask(GLboolean state)
        {
            if (state == depth_mask_enabled_)
                return;

            glDepthMask(state ? GL_TRUE : GL_FALSE);
            depth_mask_enabled_ = state;
        }

        GLboolean isDepthMaskEnabled() const
        {
            return depth_mask_enabled_;
        }

    protected:
        std::string name_{"core_depth_test"};

        GLboolean enabled_{false};
        GLboolean depth_mask_enabled_{false};
    };

    using DepthTestPtr = std::shared_ptr<DepthTest>;
} // namespace puffin

#endif // PUFFIN_DEPTH_TEST_H