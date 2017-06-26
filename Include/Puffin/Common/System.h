//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_SYSTEM_H
#define PUFFIN_SYSTEM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <vector>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class System
    {
        friend class Display;

    public:
        System();
        virtual ~System();

        std::string getName() const
        {
            return name_;
        }

        std::pair<GLint, GLint> getMaxDisplaySize() const;
        std::vector<GLint> getSupportedMsaaSamples() const;

        std::string getGpuVendor() const;
        std::string getGpuName() const;
        std::string getGlVersion() const;
        std::string getGlslVersion() const;

    protected:
        void initializeGLFW() const;
        void terminateGLFW() const;
        void initializeGL() const;

        std::string name_{"core_system"};
    };

    using SystemPtr = std::shared_ptr<System>;
} // namespace puffin

#endif // PUFFIN_SYSTEM_H