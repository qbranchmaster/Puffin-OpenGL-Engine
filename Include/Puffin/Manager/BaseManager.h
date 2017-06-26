//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_BASE_MANAGER_H
#define PUFFIN_BASE_MANAGER_H

#include <GL/glew.h>

#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class BaseManager
    {
    public:
        explicit BaseManager(std::string name)
        {
            if (!name.empty())
                name_ = name;

            logDebug(name_, "BaseManager::BaseManager()",
                "Base manager created.");
        }

        virtual ~BaseManager()
        {
            logDebug(name_, "BaseManager::~BaseManager()",
                "Base manager destroyed.");
        }

        std::string getName() const
        {
            return name_;
        }

    protected:
        std::string name_{"base_manager"};
    };

    using BaseManagerPtr = std::shared_ptr<BaseManager>;
} // namespace puffin

#endif // PUFFIN_BASE_MANAGER_H