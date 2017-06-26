//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_LIGHT_MANAGER_H
#define PUFFIN_LIGHT_MANAGER_H

#include <vector>

#include "Puffin/Lighting/DirectionalLight.h"
#include "Puffin/Lighting/PointLight.h"
#include "Puffin/Manager/BaseManager.h"

namespace puffin
{
    class LightManager : public BaseManager
    {
    public:
        LightManager() : BaseManager("core_light_manager")
        {
            directional_light_.reset(new DirectionalLight());

            logDebug(name_, "LightManager::LightManager()",
                "Light manager created.");
        }

        virtual ~LightManager()
        {
            logDebug(name_, "LightManager::~LightManager()",
                "Light manager destroyed.");
        }

        void enableLighting(GLboolean state)
        {
            lighting_enabled_ = state;
        }

        GLboolean isLightingEnabled() const
        {
            return lighting_enabled_;
        }

        DirectionalLightPtr directionalLight() const
        {
            return directional_light_;
        }

        GLint getMaxPointLightsCount() const
        {
            return max_point_lights_count_;
        }

        const std::vector<PointLightPtr>& getPointLightContainer() const
        {
            return point_light_containter_;
        }

        PointLightPtr createPointLight(std::string point_light_name = "")
        {
            if (point_light_containter_.size() >= max_point_lights_count_)
                logErrorAndThrow(name_, "LightManager::createPointLight()",
                    "Cannot create point light. Max value reached [" +
                    std::to_string(max_point_lights_count_) + "].");

            PointLightPtr point_light(new PointLight(point_light_name));
            point_light_containter_.push_back(point_light);
            return point_light;
        }

        PointLightPtr getPointLight(GLuint index) const
        {
            if (index >= point_light_containter_.size())
                logErrorAndThrow(name_, "LightManager::getPointLight()",
                    "Point light index value out of range.");

            return point_light_containter_[index];
        }

        GLuint getPointLightsCount() const
        {
            return point_light_containter_.size();
        }

    protected:
        static constexpr GLint max_point_lights_count_{4};

        GLboolean lighting_enabled_{false};

        DirectionalLightPtr directional_light_{nullptr};
        std::vector<PointLightPtr> point_light_containter_;
    };

    using LightManagerPtr = std::shared_ptr<LightManager>;
} // namespace puffin

#endif // PUFFIN_LIGHT_MANAGER_H