//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_MASTER_MANAGER_H
#define PUFFIN_MASTER_MANAGER_H

#include <memory>

#include "Puffin/Common/Logger.h"
#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Manager/FrameBufferManager.h"
#include "Puffin/Manager/LightManager.h"
#include "Puffin/Manager/MeshManager.h"
#include "Puffin/Manager/SceneManager.h"
#include "Puffin/Manager/ShaderManager.h"
#include "Puffin/Manager/TextureManager.h"
#include "Puffin/Manager/UiManager.h"

namespace puffin
{
    class MasterManager : public BaseManager
    {
    public:
        MasterManager(DisplayConfigurationPtr display_configuration,
            StateMachinePtr state_machine);
        virtual ~MasterManager();

        FrameBufferManagerPtr frameBufferManager() const
        {
            return frame_buffer_manager_;
        }

        LightManagerPtr lightManager() const
        {
            return light_manager_;
        }

        MeshManagerPtr meshManager() const
        {
            return mesh_manager_;
        }

        SceneManagerPtr sceneManager() const
        {
            return scene_manager_;
        }

        ShaderManagerPtr shaderManager() const
        {
            return shader_manager_;
        }

        TextureManagerPtr textureManager() const
        {
            return texture_manager_;
        }

        UiManagerPtr uiManager() const
        {
            return ui_manager_;
        }

    protected:
        FrameBufferManagerPtr frame_buffer_manager_{nullptr};
        LightManagerPtr light_manager_{nullptr};
        MeshManagerPtr mesh_manager_{nullptr};
        SceneManagerPtr scene_manager_{nullptr};
        ShaderManagerPtr shader_manager_{nullptr};
        TextureManagerPtr texture_manager_{nullptr};
        UiManagerPtr ui_manager_{nullptr};

        DisplayConfigurationPtr display_configuration_{nullptr};
        StateMachinePtr state_machine_{nullptr};
    };

    using MasterManagerPtr = std::shared_ptr<MasterManager>;
} // namespace puffin

#endif // PUFFIN_MASTER_MANAGER_H