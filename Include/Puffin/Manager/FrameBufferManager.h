//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_FRAME_BUFFER_MANAGER_H
#define PUFFIN_FRAME_BUFFER_MANAGER_H

#include <vector>

#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Display/DisplayConfiguration.h"
#include "Puffin/Manager/BaseManager.h"
#include "Puffin/Manager/TextureManager.h"
#include "Puffin/Renderer/FrameBuffer.h"

namespace puffin
{
    enum class TextureBufferType
    {
        DEPTH_BUFFER,
        RGB_BUFFER,
    };

    enum class RenderBufferType
    {
        DEPTH_STENCIL_BUFFER,
    };

    class FrameBufferManager : public BaseManager
    {
    public:
        FrameBufferManager(StateMachinePtr state_machine, 
            DisplayConfigurationPtr display_configuration,
            TextureManagerPtr texture_manager);
        virtual ~FrameBufferManager();

        FrameBufferPtr createFrameBuffer(std::string buffer_name = "");

        void addTextureBuffer(FrameBufferPtr frame_buffer,
            TextureBufferType type, GLint width, GLint height, 
            GLboolean mulitsampled);
        void addRenderBuffer(FrameBufferPtr frame_buffer, RenderBufferType type,
            GLint width, GLint height, GLboolean multisampled);
        void addCubeMapDepthBuffer(FrameBufferPtr frame_buffer, GLint size);

        void disableDrawBuffer(FrameBufferPtr frame_buffer);
        void disableReadBuffer(FrameBufferPtr frame_buffer);

    protected:
        DisplayConfigurationPtr display_configuration_{nullptr};
        StateMachinePtr state_machine_{nullptr};
        TextureManagerPtr texture_manager_{nullptr};

        std::vector<FrameBufferPtr> frame_buffer_container_;
    };

    using FrameBufferManagerPtr = std::shared_ptr<FrameBufferManager>;
} // namespace puffin

#endif // PUFFIN_FRAME_BUFFER_MANAGER_H