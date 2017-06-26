//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_FACE_CULL_H
#define PUFFIN_FACE_CULL_H

#include <GL/glew.h>

#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    enum class CulledSide
    {
        BACK,
        FRONT,
        FRONT_AND_BACK,
    };

    class FaceCull
    {
    public:
        FaceCull()
        {
            glGetBooleanv(GL_CULL_FACE, &enabled_);

            GLint value = 0;
            glGetIntegerv(GL_CULL_FACE_MODE, &value);
            switch (value)
            {
            case GL_BACK:
                culled_side_ = CulledSide::BACK;
                break;
            case GL_FRONT:
                culled_side_ = CulledSide::FRONT;
                break;
            case GL_FRONT_AND_BACK:
                culled_side_ = CulledSide::FRONT_AND_BACK;
                break;
            }

            logDebug(name_, "FaceCull::FaceCull()", "Face cull created.");
        }

        virtual ~FaceCull()
        {
            logDebug(name_, "FaceCull::~FaceCull()", "Face cull destroyed.");
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
                glEnable(GL_CULL_FACE);
            else
                glDisable(GL_CULL_FACE);

            enabled_ = state;
        }

        GLboolean isEnabled() const
        {
            return enabled_;
        }

        void setCulledSide(CulledSide culled_side)
        {
            if (culled_side == culled_side_)
                return;

            switch (culled_side)
            {
            case CulledSide::BACK:
                glCullFace(GL_BACK);
                break;
            case CulledSide::FRONT:
                glCullFace(GL_FRONT);
                break;
            case CulledSide::FRONT_AND_BACK:
                glCullFace(GL_FRONT_AND_BACK);
                break;
            }

            culled_side_ = culled_side;
        }

    protected:
        std::string name_{"core_face_cull"};

        GLboolean enabled_{false};
        CulledSide culled_side_{CulledSide::BACK};
    };

    using FaceCullPtr = std::shared_ptr<FaceCull>;
} // namespace puffin

#endif // PUFFIN_FACE_CULL_H