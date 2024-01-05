#include "PluginLibrary.h"

PluginLibrary* PluginLibrary::instance_ = nullptr;

PluginLibrary& PluginLibrary::getInstance() {
    if (!instance_) {
        instance_ = new PluginLibrary;
    }
    return *instance_;
}

Manager<Magnum::Text::AbstractFont>& PluginLibrary::getFontManager() {
    return getInstance().fontManager_;
}
