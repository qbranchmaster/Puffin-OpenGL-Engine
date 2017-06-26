//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_SHADOW_MAP_CONFIGURATION_H
#define PUFFIN_SHADOW_MAP_CONFIGURATION_H

#include <GL/glew.h>

#include <memory>
#include <string>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class ShadowMapConfiguration
    {
    public:
        ShadowMapConfiguration()
        {
            logDebug(name_, "ShadowMapConfiguration::ShadowMapConfiguration()",
                "Shadow map configuration created.");
        }

        virtual ~ShadowMapConfiguration()
        {
            logDebug(name_, "ShadowMapConfiguration::~ShadowMapConfiguration()",
                "Shadow map configuration destroyed.");
        }

        void enableShadows(bool state)
        {
            shadows_enabled_ = state;
        }

        GLboolean isShadowsEnabled() const
        {
            return shadows_enabled_;
        }

        void setShadowMapSizeDirectionalLight(GLint size)
        {
            if (size <= 0)
                logErrorAndThrow(name_,
                    "ShadowMapConfiguration::setShadowMapSizeDirectionalLight()",
                    "Shadow map size value out of range: {0 < VALUE}.");

            shadow_map_size_dir_light_ = size;
        }

        GLint getShadowMapSizeDirectionalLight() const
        {
            return shadow_map_size_dir_light_;
        }

        void setShadowMapSizePointLight(GLint size)
        {
            if (size <= 0)
                logErrorAndThrow(name_,
                    "ShadowMapConfiguration::setShadowMapSizePointLight()",
                    "Shadow map size value out of range: {0 < VALUE}.");

            shadow_map_size_point_light_ = size;
        }

        GLint getShadowMapSizePointLight() const
        {
            return shadow_map_size_point_light_;
        }

        void setShadowDistance(GLfloat distance)
        {
            if (distance <= 0.0f)
                logErrorAndThrow(name_,
                    "ShadowMapConfiguration::setShadowDistance()",
                    "Shadow distance value out of range: {0.0 < VALUE}.");

            shadow_distance_ = distance;
        }

        GLfloat getShadowDistance() const
        {
            return shadow_distance_;
        }

        void setShadowTransitionDistance(GLfloat distance)
        {
            if (distance <= 0.0f)
                logErrorAndThrow(name_,
                    "ShadowMapConfiguration::setShadowTransitionDistance()",
                    "Shadow transition distance value out of range: "
                    "{0.0 < VALUE}.");

            shadow_transition_distance_ = distance;
        }

        GLfloat getShadowTransitionDistance() const
        {
            return shadow_transition_distance_;
        }

        void setPcfSamplesCount(GLint value)
        {
            if (value <= 0)
                logErrorAndThrow(name_,
                    "ShadowMapConfiguration::setPcfSamplesCount()",
                    "PCF samples count value out of range: {0 < VALUE}.");

            pcf_samples_count_ = value;
        }

        GLint getPcfSamplesCount() const
        {
            return pcf_samples_count_;
        }

    protected:
        std::string name_{"core_shadow_map_configuration"};

        GLboolean shadows_enabled_{false};
        GLint shadow_map_size_dir_light_{1024};
        GLint shadow_map_size_point_light_{1024};
        GLfloat shadow_distance_{10.0f};
        GLfloat shadow_transition_distance_{5.0f};

        GLint pcf_samples_count_{1};
    };

    using ShadowMapConfigurationPtr = std::shared_ptr<ShadowMapConfiguration>;
} // namespace puffin

#endif // PUFFIN_SHADOW_MAP_CONFIGURATION_H