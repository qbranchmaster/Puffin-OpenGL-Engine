//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Manager/UiManager.h"

using namespace puffin;

UiManager::UiManager() : BaseManager("core_ui_manager")
{
    logDebug(name_, "UiManager::UiManager()", "UI manager created.");
}

UiManager::~UiManager()
{
    logDebug(name_, "UiManager::~UiManager()", "UI manager destroyed.");
}

TextPtr UiManager::createText(std::wstring text, GLint font_size,
    const glm::uvec2 &position, std::string font, std::string text_name)
{
    TextPtr text_obj(new Text(text, font_size, position, font, text_name));
    text_container_.push_back(text_obj);
    return text_obj;
}