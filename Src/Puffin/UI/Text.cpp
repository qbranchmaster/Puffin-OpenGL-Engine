//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/UI/Text.h"

using namespace puffin;

Text::Text(std::wstring text, GLint font_size, const glm::uvec2 &position,
    std::string font, std::string name) : UiComponent(name)
{
    setText(text);
    setFontSize(font_size);
    setPosition(position);
    setFont(font);

    logDebug(name_, "Text::Text()", "Text created.");
}

Text::~Text()
{
    logDebug(name_, "Text::~Text()", "Text destroyed.");
}

void Text::setFontSize(GLint font_size)
{
    if (font_size <= 0)
        logErrorAndThrow(name_, "Text::setFontSize()",
            "Font size value out of range: {0 < VALUE}.");

    font_size_ = font_size;
}

void Text::setFont(std::string font)
{
    if (font.empty())
    {
        logWarning(name_, "Text::setFont()",
            "Font not set. Default will be chosen.");
        font = "Data/Fonts/unispace/unispace.ttf";
    }

    font_ = font;
}

void Text::setHorizontalSpacing(GLint spacing)
{
    horizontal_spacing_ = spacing;
}

GLint Text::getHorizontalSpacing() const
{
    return horizontal_spacing_;
}

void Text::setVerticalSpacing(GLint spacing)
{
    vertical_spacing_ = spacing;
}

GLint Text::getVerticalSpacing() const
{
    return vertical_spacing_;
}