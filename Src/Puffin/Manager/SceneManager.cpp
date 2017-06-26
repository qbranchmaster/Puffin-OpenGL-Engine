//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Manager/SceneManager.h"

using namespace puffin;

SceneManager::SceneManager() : BaseManager("core_scene_manager")
{
    logDebug(name_, "SceneManager::SceneManager()", "Scene manager created.");
}

SceneManager::~SceneManager()
{
    logDebug(name_, "SceneManager::~SceneManager()",
        "Scene manager destroyed.");
}

ScenePtr SceneManager::createScene(std::string scene_name)
{
    ScenePtr scene(new Scene(scene_name));
    scene_container_.push_back(scene);
    return scene;
}