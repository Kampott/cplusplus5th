#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <string>
#include <vector>
#include <windows.h>
#include "Operations.h"

class PluginLoader {
public:
    void loadAllPlugins ( Operations& ops );
private:
    void loadPlugin ( const std::string& pluginPath , Operations& ops );
    std::vector<HMODULE> loadedPlugins;
};

#endif // PLUGIN_LOADER_H