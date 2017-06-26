//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_TEXT_H
#define PUFFIN_TEXT_H

#include "Puffin/Ui/UiComponent.h"

namespace puffin
{
    class Text : public UiComponent
    {
    public:
        Text(std::wstring text, GLint font_size, const glm::uvec2 &position,
            std::string font = "", std::string name = "");
        virtual ~Text();

        void setFontSize(GLint font_size);

        GLint getFontSize() const
        {
            return font_size_;
        }

        void setFont(std::string font);

        std::string getFont() const
        {
            return font_;
        }

        void setText(std::wstring text)
        {
            text_ = text;
        }

        std::wstring getText() const
        {
            return text_;
        }

        void setFontColor(const glm::vec3 &color)
        {
            font_color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getFontColor() const
        {
            return font_color_;
        }

        void setOutlineColor(const glm::vec3 &color)
        {
            outline_color_ = glm::vec3(glm::clamp(color.r, 0.0f, 1.0f),
                glm::clamp(color.g, 0.0f, 1.0f),
                glm::clamp(color.b, 0.0f, 1.0f));
        }

        glm::vec3 getOutlineColor() const
        {
            return outline_color_;
        }

        void setOutlineSize(GLint size)
        {
            if (size < 0)
                logErrorAndThrow(name_, "Text::setOutlineSize()",
                    "Outline size value out of range: {0 <= VALUE}.");

            outline_size_ = size;
        }

        GLint getOutlineSize() const
        {
            return outline_size_;
        }

        void setVerticalSpacing(GLint spacing);
        GLint getVerticalSpacing() const;
        void setHorizontalSpacing(GLint spacing);
        GLint getHorizontalSpacing() const;

    protected:
        std::wstring text_{L""};
        std::string font_{""};
        GLint font_size_{16};
        glm::vec3 font_color_{1.0f, 1.0f, 1.0f};

        GLint outline_size_{0};
        glm::vec3 outline_color_{0.0f, 0.0f, 0.0f};

        GLint vertical_spacing_{0};
        GLint horizontal_spacing_{0};
    };

    using TextPtr = std::shared_ptr<Text>;
} // namespace puffin

#endif // PUFFIN_TEXT_H