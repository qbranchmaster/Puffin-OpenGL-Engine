//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_ENGINE_CORE_H
#define PUFFIN_ENGINE_CORE_H

#include "Puffin/Common/RandomRealGenerator.h"
#include "Puffin/Common/System.h"
#include "Puffin/Common/Timer.h"
#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Display/Display.h"
#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Input/Input.h"
#include "Puffin/Manager/MasterManager.h"
#include "Puffin/Renderer/MasterRenderer.h"

namespace puffin
{
    class EngineCore
    {
    public:
        void initialize();
        void createDisplay(std::string window_caption = "");

        void start();
        void stop();

        CameraPtr mainCamera() const
        {
            return main_camera_;
        }

        DisplayPtr display() const
        {
            return display_;
        }

        DisplayConfigurationPtr displayConfiguration() const
        {
            return display_configuration_;
        }

        InputPtr input() const
        {
            return input_;
        }

        MasterRendererPtr masterRenderer() const
        {
            return master_renderer_;
        }

        MeshManagerPtr meshManager() const
        {
            return master_manager_->meshManager();
        }

        SceneManagerPtr sceneManager() const
        {
            return master_manager_->sceneManager();
        }

        UiManagerPtr uiManager() const
        {
            return master_manager_->uiManager();
        }

        TextureManagerPtr textureManager() const
        {
            return master_manager_->textureManager();
        }

    protected:
        CameraPtr main_camera_{nullptr};
        DisplayPtr display_{nullptr};
        DisplayConfigurationPtr display_configuration_{nullptr};
        InputPtr input_{nullptr};
        MasterManagerPtr master_manager_{nullptr};
        MasterRendererPtr master_renderer_{nullptr};
        StateMachinePtr state_machine_{nullptr};
        SystemPtr system_{nullptr};
    };
} // namespace puffin

#endif // PUFFIN_ENGINE_CORE_H