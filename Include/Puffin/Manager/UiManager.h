//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_UI_MANAGER_H
#define PUFFIN_UI_MANAGER_H

#include <vector>

#include "Puffin/Manager/BaseManager.h"
#include "Puffin/UI/Text.h"

namespace puffin
{
    class UiManager : public BaseManager
    {
    public:
        UiManager();
        virtual ~UiManager();

        TextPtr createText(std::wstring text, GLint font_size,
            const glm::uvec2 &position, std::string font = "",
            std::string text_name = "");

        std::vector<TextPtr> getTextContainer() const
        {
            return text_container_;
        }

    protected:
        std::vector<TextPtr> text_container_;
    };

    using UiManagerPtr = std::shared_ptr<UiManager>;
} // namespace puffin

#endif // PUFFIN_UI_MANAGER_H