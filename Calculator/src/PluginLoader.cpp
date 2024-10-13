#include "PluginLoader.h"
#include <iostream>
#include <stdexcept>
#include <filesystem>

typedef void (*RegisterFunc)(Operations& ops);

void PluginLoader::loadPlugin ( const std::string& pluginPath , Operations& ops ) {
    HMODULE hModule = LoadLibraryA ( pluginPath.c_str ( ) );
    if (!hModule) {
        throw std::runtime_error ( "Failed to load plugin: " + pluginPath );
    }

    // Сохраняем загруженную библиотеку
    loadedPlugins.push_back ( hModule );

    // Ищем функцию "registerOperations" в плагине
    RegisterFunc registerOperations = (RegisterFunc)GetProcAddress ( hModule , "registerOperations" );
    if (!registerOperations) {
        FreeLibrary ( hModule );
        loadedPlugins.pop_back ( );
        throw std::runtime_error ( "Failed to find registerOperations in plugin: " + pluginPath );
    }
    // Вызываем функцию регистрации операций
    registerOperations ( ops );
}

void PluginLoader::loadAllPlugins ( Operations& ops ) {
    namespace fs = std::filesystem;

    // Получаем относительный путь к папке с плагинами
    std::string pluginsPath = "plugins";  // Относительный путь к папке с плагинами

    // Проверяем, существует ли папка с плагинами
    if (!fs::exists ( pluginsPath ) || !fs::is_directory ( pluginsPath )) {
        std::cerr << "Directory " << pluginsPath << " does not exist." << std::endl;
        return; // Прерываем выполнение, если директория не найдена
    }

    for (const auto& entry : fs::directory_iterator ( pluginsPath )) {
        if (entry.path ( ).extension ( ) == ".dll") {
            loadPlugin ( entry.path ( ).string ( ) , ops );
            std::cout << "Загружен плагин: " << entry.path ( ).string ( ) << std::endl;
        }
    }
}
