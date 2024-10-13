#include "main.h"

int main ( ) {
    setlocale ( LC_ALL , "Russian" );
    try {
        Calculator calculator;
        Operations& ops = Operations::getInstance ( );
        // ����������� �������� ����� PluginLoader
        PluginLoader pluginLoader;

        // ��������� ��� ������� �� ������������ ��������� � ����� "plugins"
        pluginLoader.loadAllPlugins ( ops );

        // �������� ���� ����� ������������
        std::string inputExpression;
        std::cout << "������� ��������� ��� ���������� (������� ~ ��� ������): " << std::endl;

        while (true) {
            std::getline ( std::cin , inputExpression );

            if (inputExpression == "~") { // �������� �� �����
                break;
            }

            // ���������� ����������
            double result = calculator.evaluate ( inputExpression );
            std::cout << "���������: " << result << std::endl;
        }


    }
    catch (const std::exception& e) {
        std::cerr << "������: " << e.what ( ) << std::endl;
    }
    return 0;
}