#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <stdexcept>

class Calculator {
public:
    // Конструктор
    Calculator();

    // Метод регистрации функций (унапр. sin, cos, pow) через плагины
    void registerFunction(const std::string& name, std::function<double(double)> func);

    // Основной метод для вычисления выражения
    double evaluate(const std::string& expression);

private:
    // Карта для хранения зарегистрированных функций
    std::unordered_map<std::string, std::function<double(double)>> functions;

    // Парсинг выражения и вычисление через RPN
    double parseExpression(const std::string& expr);

    // Токенизация строки выражения
    std::vector<std::string> tokenize(const std::string& expr);

    // Преобразование токенов в обратную польскую нотацию (RPN)
    std::vector<std::string> toRPN(const std::vector<std::string>& tokens);

    // Вычисление выражения в обратной польской нотации
    double evaluateRPN(const std::vector<std::string>& rpn);
};
class OutOfBoundsException : public std::runtime_error {
public:
    explicit OutOfBoundsException ( const std::string& message ) : std::runtime_error ( message ) {}
};
#endif // CALCULATOR_H