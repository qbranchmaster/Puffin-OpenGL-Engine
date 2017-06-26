//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/EngineCore.h"

using namespace puffin;

void EngineCore::initialize()
{
    system_.reset(new System());
    display_configuration_.reset(new DisplayConfiguration(system_));
}

void EngineCore::createDisplay(std::string window_caption)
{
    display_.reset(new Display(display_configuration_, system_));
    display_->setCaption(window_caption);
    input_.reset(new Input(display_));

    state_machine_.reset(new StateMachine());
    state_machine_->depthTest()->enable(true);
    state_machine_->depthTest()->enableDepthMask(true);
    state_machine_->faceCulling()->enable(true);

    master_manager_.reset(new MasterManager(display_configuration_,
        state_machine_));
    main_camera_.reset(new Camera("main_camera"));
    master_renderer_.reset(new MasterRenderer(master_manager_, state_machine_,
        display_, display_configuration_));
}

void EngineCore::start()
{
    master_renderer_->start();
}

void EngineCore::stop()
{
    master_renderer_->stop();
}