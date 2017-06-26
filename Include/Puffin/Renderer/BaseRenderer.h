//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_BASE_RENDERER_H
#define PUFFIN_BASE_RENDERER_H

#include <GL/glew.h>

#include <memory>

#include "Puffin/Camera/Camera.h"
#include "Puffin/Common/Logger.h"
#include "Puffin/Mesh/Scene.h"

namespace puffin
{
    class BaseRenderer
    {
    public:
        explicit BaseRenderer(std::string name)
        {
            if (!name.empty())
                name_ = name;

            logDebug(name_, "BaseRenderer::BaseRenderer()",
                "Base renderer created.");
        }

        virtual ~BaseRenderer()
        {
            logDebug(name_, "BaseRenderer::~BaseRenderer()",
                "Base renderer destroyed.");
        }

        std::string getName() const
        {
            return name_;
        }

        void useCamera(CameraPtr camera)
        {
            if (!camera)
                logErrorAndThrow(name_, "BaseRenderer::useCamera()",
                    "Object [Camera] pointer not set.");

            active_camera_ = camera;
        }

    protected:
        virtual void render(ScenePtr scene) = 0;

        std::string name_{"core_base_renderer"};

        CameraPtr active_camera_{nullptr};
    };

    using BaseRendererPtr = std::shared_ptr<BaseRenderer>;
} // namespace puffin

#endif // PUFFIN_BASE_RENDERER_H