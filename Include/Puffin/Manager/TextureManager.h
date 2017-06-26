//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_TEXTURE_MANAGER_H
#define PUFFIN_TEXTURE_MANAGER_H

#include <SOIL/SOIL.h>

#include <array>
#include <map>
#include <vector>

#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Manager/BaseManager.h"
#include "Puffin/Texture/Texture.h"
#include "Puffin/Texture/TextureFilter.h"
#include "Puffin/Texture/TextureWrap.h"

namespace puffin
{
    class TextureManager : public BaseManager
    {
        friend class FontRenderer;
        friend class MasterRenderer;
        friend class Object3DRenderer;
        friend class ParticleRenderer;
        friend class SkyboxRenderer;
        friend class WaterRenderer;

    public:
        TextureManager(StateMachinePtr state_machine,
            DisplayConfigurationPtr display_configuration);
        virtual ~TextureManager();

        // Texture order in array: right, left, up, down, back, front
        TexturePtr loadTextureCube(std::array<std::string, 6> textures,
            std::string texture_name = "");
        TexturePtr loadTexture2D(std::string path, GLboolean auto_free = true,
            std::string texture_name = "");

        TexturePtr createTextureCube(GLint size, std::string texture_name = "");
        TexturePtr createTexture2D(std::string texture_name = "");
        TexturePtr createTextureDepthBuffer(GLint width, GLint height,
            std::string texture_name = "");
        TexturePtr createTextureRgbBuffer(GLint width, GLint height,
            GLboolean multisample, std::string texture_name = "");

        void setTexture2DData(TexturePtr texture, GLubyte *data, GLint width,
            GLint height, GLint color_channels);

        void setTexture2DBorderColor(TexturePtr texture,
            const glm::vec4 &color) const;
        void freeTexture2DRawData(TexturePtr texture) const;
        void setTextureFilter(TexturePtr texture,
            TextureFilter texture_filter) const;
        void setTextureWrap(TexturePtr texture, TextureWrap wrap_mode) const;

    protected:
        void setDefaultTextureFilter();
        void generateTextureMipmap(TexturePtr texture) const;
        void setTextureSlot(GLint slot_index);
        void setUnpackPixelAlignment(GLint alignment);

        std::map<TextureType, TextureFilter> default_texture_filter_;
        std::vector<TexturePtr> texture_container_;

        DisplayConfigurationPtr display_configuration_{nullptr};
        StateMachinePtr state_machine_{nullptr};
    };

    using TextureManagerPtr = std::shared_ptr<TextureManager>;
} // namespace puffin

#endif // PUFFIN_TEXTURE_MANAGER_H