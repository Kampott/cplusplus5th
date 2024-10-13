#include "main.h"

int main ( ) {
    setlocale ( LC_ALL , "Russian" );
    try {
        Calculator calculator;
        Operations& ops = Operations::getInstance ( );
        // Подключение плагинов через PluginLoader
        PluginLoader pluginLoader;

        // Загружаем все функции из динамических библиотек в папке "plugins"
        pluginLoader.loadAllPlugins ( ops );

        // Основной цикл ввода пользователя
        std::string inputExpression;
        std::cout << "Введите выражение для вычисления (введите ~ для выхода): " << std::endl;

        while (true) {
            std::getline ( std::cin , inputExpression );

            if (inputExpression == "~") { // Проверка на выход
                break;
            }

            // Вычисление результата
            double result = calculator.evaluate ( inputExpression );
            std::cout << "Результат: " << result << std::endl;
        }


    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what ( ) << std::endl;
    }
    return 0;
}