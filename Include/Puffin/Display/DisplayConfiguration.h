//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_DISPLAY_CONFIGURATION_H
#define PUFFIN_DISPLAY_CONFIGURATION_H

#include <memory>

#include "Puffin/Common/Logger.h"
#include "Puffin/Common/System.h"

namespace puffin
{
    class DisplayConfiguration
    {
        friend class Display;

    public:
        explicit DisplayConfiguration(SystemPtr system);
        virtual ~DisplayConfiguration();

        std::string getName() const
        {
            return name_;
        }

        void configure(GLint width, GLint height, GLint msaa_samples,
            GLboolean fullscreen);

        GLint getWidth() const
        {
            return width_;
        }

        GLint getHeight() const
        {
            return height_;
        }

        GLint getMsaaSamples() const
        {
            return msaa_samples_;
        }

        GLboolean isFullscreen() const
        {
            return fullscreen_;
        }

    protected:
        GLboolean checkIntValue(GLint value,
            std::vector<GLint> allowed_values) const;
        GLboolean checkIntValue(GLint value, GLint min, GLint max) const;

        std::string name_{"core_display_configuration"};

        GLboolean blocked_{false};

        GLint width_{320};
        GLint height_{240};
        GLint msaa_samples_{4};
        GLboolean fullscreen_{false};

        SystemPtr system_{nullptr};
    };

    using DisplayConfigurationPtr = std::shared_ptr<DisplayConfiguration>;
} // namespace puffin

#endif // PUFFIN_DISPLAY_CONFIGURATION_H