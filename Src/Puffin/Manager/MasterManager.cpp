//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Manager/MasterManager.h"

using namespace puffin;

MasterManager::MasterManager(DisplayConfigurationPtr display_configuration,
    StateMachinePtr state_machine) : BaseManager("core_master_manager")
{
    if (!display_configuration)
        logErrorAndThrow(name_, "MasterManager::MasterManager()",
            "Object [DisplayConfiguration] pointer not set.");

    if (!state_machine)
        logErrorAndThrow(name_, "MasterManager::MasterManager()",
            "Object [StateMachine] pointer not set.");

    display_configuration_ = display_configuration;
    state_machine_ = state_machine;

    light_manager_.reset(new LightManager());
    scene_manager_.reset(new SceneManager());
    shader_manager_.reset(new ShaderManager(state_machine_));
    texture_manager_.reset(new TextureManager(state_machine_,
        display_configuration_));
    ui_manager_.reset(new UiManager());
    frame_buffer_manager_.reset(new FrameBufferManager(state_machine_,
        display_configuration_, texture_manager_));

    mesh_manager_.reset(new MeshManager(state_machine_, texture_manager_));

    logDebug(name_, "MasterManager::MasterManager()", "Master manager created.");
}

MasterManager::~MasterManager()
{
    logDebug(name_, "MasterManager::MasterManager()",
        "Master manager destroyed.");
}