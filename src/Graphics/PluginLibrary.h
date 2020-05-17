#pragma once

#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Text/AbstractFont.h>

#include "Types.h"

using Corrade::PluginManager::Manager;

class PluginLibrary {
private:
	Manager<Magnum::Text::AbstractFont> fontManager_;
	static PluginLibrary* instance_;
	static PluginLibrary& getInstance();
public:
	static Manager<Magnum::Text::AbstractFont>& getFontManager();
	
};
