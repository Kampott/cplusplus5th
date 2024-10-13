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

    // ��������� ����������� ����������
    loadedPlugins.push_back ( hModule );

    // ���� ������� "registerOperations" � �������
    RegisterFunc registerOperations = (RegisterFunc)GetProcAddress ( hModule , "registerOperations" );
    if (!registerOperations) {
        FreeLibrary ( hModule );
        loadedPlugins.pop_back ( );
        throw std::runtime_error ( "Failed to find registerOperations in plugin: " + pluginPath );
    }
    // �������� ������� ����������� ��������
    registerOperations ( ops );
}

void PluginLoader::loadAllPlugins ( Operations& ops ) {
    namespace fs = std::filesystem;

    // �������� ������������� ���� � ����� � ���������
    std::string pluginsPath = "plugins";  // ������������� ���� � ����� � ���������

    // ���������, ���������� �� ����� � ���������
    if (!fs::exists ( pluginsPath ) || !fs::is_directory ( pluginsPath )) {
        std::cerr << "Directory " << pluginsPath << " does not exist." << std::endl;
        return; // ��������� ����������, ���� ���������� �� �������
    }

    for (const auto& entry : fs::directory_iterator ( pluginsPath )) {
        if (entry.path ( ).extension ( ) == ".dll") {
            loadPlugin ( entry.path ( ).string ( ) , ops );
            std::cout << "�������� ������: " << entry.path ( ).string ( ) << std::endl;
        }
    }
}
