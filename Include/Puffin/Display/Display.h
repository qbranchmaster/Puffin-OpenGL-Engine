//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_DISPLAY_H
#define PUFFIN_DISPLAY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <memory>

#include "Puffin/Common/Logger.h"
#include "Puffin/Common/System.h"
#include "Puffin/Display/DisplayConfiguration.h"

namespace puffin
{
    class Display
    {
        friend class Input;
        friend class MasterRenderer;

    public:
        Display(DisplayConfigurationPtr display_configuration, SystemPtr system);
        virtual ~Display();

        std::string getName() const
        {
            return name_;
        }

        GLboolean isFullscreen() const
        {
            return fullscreen_;
        }

        GLint getWidth() const
        {
            return width_;
        }

        GLint getHeight() const
        {
            return height_;
        }

        GLint getMSAASamples() const
        {
            return msaa_samples_;
        }

        GLfloat getAspect() const
        {
            return static_cast<GLfloat>(width_) / static_cast<GLfloat>(height_);
        }

        void setCaption(std::string caption)
        {
            caption_ = caption;
            glfwSetWindowTitle(handle_, caption_.c_str());
        }

        std::string getCaption() const
        {
            return caption_;
        }

        void setBackgroundColor(const glm::vec3 &color)
        {
            background_color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getBackgroundColor() const
        {
            return background_color_;
        }

    protected:
        GLboolean isClosing() const
        {
            if (glfwWindowShouldClose(handle_))
                return true;

            return false;
        }

        void pollEvents() const
        {
            glfwPollEvents();
        }

        void swapBuffers() const
        {
            glfwSwapBuffers(handle_);
        }

        std::string name_{"core_display"};
        std::string caption_{"Puffin Engine Window"};

        GLFWwindow *handle_{nullptr};

        GLboolean fullscreen_{false};
        GLint height_{0};
        GLint width_{0};
        GLint msaa_samples_{0};
        glm::vec3 background_color_{0.0f, 0.0f, 0.0f};
    };

    using DisplayPtr = std::shared_ptr<Display>;
} // namespace puffin

#endif // PUFFIN_DISPLAY_H