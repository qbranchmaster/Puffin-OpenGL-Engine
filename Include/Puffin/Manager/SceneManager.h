//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_SCENE_MANAGER_H
#define PUFFIN_SCENE_MANAGER_H

#include <vector>

#include "Puffin/Manager/BaseManager.h"
#include "Puffin/Mesh/Scene.h"

namespace puffin
{
    class SceneManager : public BaseManager
    {
    public:
        SceneManager();
        virtual ~SceneManager();

        ScenePtr createScene(std::string scene_name = "");

        std::vector<ScenePtr> getSceneContainer() const
        {
            return scene_container_;
        }

    protected:
        std::vector<ScenePtr> scene_container_;
    };

    using SceneManagerPtr = std::shared_ptr<SceneManager>;
} // namespace puffin

#endif // PUFFIN_SCENE_MANAGER_H