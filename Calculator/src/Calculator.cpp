#include "Calculator.h"
#include "Operations.h"
#include "PluginLoader.h"
#include <sstream>
#include <cctype>
#include <stack>
#include <vector>
#include <algorithm>
#include <regex>

// Конструктор
Calculator::Calculator ( ) {}

// Регистрация функции из плагина
void Calculator::registerFunction ( const std::string& name , std::function<double ( double )> func ) {
    functions[name] = std::move ( func );
}

// Основной метод для вычисления выражения
double Calculator::evaluate ( const std::string& expression ) {
    std::string expr = expression; // Локальная копия
    std::smatch match;
    std::regex functionRegex ( R"((\w+)\(([^)]+)\))" );

    Operations& ops = Operations::getInstance ( );
    try {
        while (std::regex_search ( expr , match , functionRegex )) {
            const std::string& funcName = match[1];  // Используем ссылку
            const std::string& args = match[2];      // Используем ссылку

            if (ops.isUnaryOperation ( funcName )) {
                // Унарная операция
                double arg = std::stod ( args );
                double result = ops.applyUnary ( funcName , arg );
                expr = std::regex_replace ( expr , functionRegex , std::to_string ( result ) , std::regex_constants::format_first_only );
            }
            else if (ops.isBinaryOperation ( funcName )) {
                // Бинарная операция
                std::stringstream ss ( args );
                std::string arg1 , arg2;
                std::getline ( ss , arg1 , ',' );
                std::getline ( ss , arg2 , ',' );

                double a = std::stod ( arg1 );
                double b = std::stod ( arg2 );
                double result = ops.applyBinary ( funcName , a , b );
                expr = std::regex_replace ( expr , functionRegex , std::to_string ( result ) , std::regex_constants::format_first_only );
            }
            else {
                throw std::runtime_error ( "Unknown function: " + funcName );
            }
        }
        return parseExpression ( expr );
    }
    catch (const OutOfBoundsException& e) {
        throw std::runtime_error ( "OutOfBoundsException: " + std::string ( e.what ( ) ) );
    }
    catch (const std::exception& e) {
        throw std::runtime_error ( "Error: " + std::string ( e.what ( ) ) );
    }
}

// Парсинг и вычисление выражения
double Calculator::parseExpression ( const std::string& expr ) {
    auto tokens = tokenize ( expr );         // Разбиваем выражение на токены
    auto rpn = toRPN ( tokens );             // Преобразуем токены в обратную польскую нотацию
    return evaluateRPN ( rpn );              // Вычисляем выражение в ОПН
}

// Токенизация строки выражения
std::vector<std::string> Calculator::tokenize ( const std::string& expr ) {
    std::vector<std::string> tokens;
    std::string number;
    std::string func;

    size_t i = 0;
    while (i < expr.length ( )) {
        char ch = expr[i];

        if (std::isspace ( ch )) {
            ++i;
            continue;
        }

        // Число (включая десятичные точки)
        if (std::isdigit ( ch ) || ch == ',') {
            number.clear ( );
            while (i < expr.length ( ) && ( std::isdigit ( expr[i] ) || expr[i] == ',' )) {
                number += expr[i++];
            }
            tokens.push_back ( std::move ( number ) ); // Используем move для минимизации копий
            continue;
        }

        // Функция или переменная (буквы)
        if (std::isalpha ( ch )) {
            func.clear ( );
            while (i < expr.length ( ) && std::isalpha ( expr[i] )) {
                func += expr[i++];
            }
            tokens.push_back ( std::move ( func ) ); // Используем move для минимизации копий
            continue;
        }

        // Операторы и скобки
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')') {
            tokens.emplace_back ( 1 , ch );
            ++i;
            continue;
        }

        // Некорректный символ
        throw std::runtime_error ( std::string ( "Invalid character in expression: " ) + ch );
    }

    return tokens;
}

// Преобразование токенов в обратную польскую нотацию (RPN)
std::vector<std::string> Calculator::toRPN ( const std::vector<std::string>& tokens ) {
    std::vector<std::string> output;
    std::stack<std::string> opStack;
    Operations& ops = Operations::getInstance ( );

    for (size_t i = 0; i < tokens.size ( ); ++i) {
        const std::string& token = tokens[i];

        // Если токен число
        if (std::isdigit ( token[0] ) || ( token.length ( ) > 1 && token.find ( '.' ) != std::string::npos )) {
            output.push_back ( token );
        }
        // Если токен оператор
        else if (ops.isBinaryOperation ( token ) || ops.isUnaryOperation ( token )) {
            bool isUnary = false;
            if (token == "-" && ( i == 0 || tokens[i - 1] == "(" || ops.isBinaryOperation ( tokens[i - 1] ) )) {
                isUnary = true;
            }

            std::string op = isUnary ? "u" + token : token; // Префикс 'u' для унарных операций

            while (!opStack.empty ( )) {
                const std::string& top = opStack.top ( );
                if (( ops.isBinaryOperation ( top ) || ops.isUnaryOperation ( top ) ) && ops.getPriority ( top ) >= ops.getPriority ( op )) {
                    output.push_back ( top );
                    opStack.pop ( );
                }
                else {
                    break;
                }
            }
            opStack.push ( op );
        }
        // Если токен открывающая скобка
        else if (token == "(") {
            opStack.push ( token );
        }
        // Если токен закрывающая скобка
        else if (token == ")") {
            while (!opStack.empty ( ) && opStack.top ( ) != "(") {
                output.push_back ( opStack.top ( ) );
                opStack.pop ( );
            }
            if (opStack.empty ( )) {
                throw std::runtime_error ( "Mismatched parentheses" );
            }
            opStack.pop ( ); // Удаляем '('
        }
        else {
            throw std::runtime_error ( "Unknown token: " + token );
        }
    }

    // Перемещаем оставшиеся операторы в output
    while (!opStack.empty ( )) {
        const std::string& top = opStack.top ( );
        if (top == "(" || top == ")") {
            throw std::runtime_error ( "Mismatched parentheses" );
        }
        output.push_back ( top );
        opStack.pop ( );
    }

    return output;
}

// Вычисление выражения в обратной польской нотации
double Calculator::evaluateRPN ( const std::vector<std::string>& rpn ) {
    std::stack<double> evalStack;
    Operations& ops = Operations::getInstance ( );

    for (const auto& token : rpn) {
        // Если токен число
        if (std::isdigit ( token[0] ) || ( token.length ( ) > 1 && token.find ( '.' ) != std::string::npos )) {
            evalStack.push ( std::stod ( token ) );
        }
        // Если токен унарная операция
        else if (token[0] == 'u' && ops.isUnaryOperation ( token.substr ( 1 ) )) {
            if (evalStack.empty ( )) {
                throw std::runtime_error ( "Insufficient operands for unary operator: " + token );
            }
            double a = evalStack.top ( ); evalStack.pop ( );


            double result = ops.applyUnary ( token.substr ( 1 ) , a );
            evalStack.push ( result );
        }
        // Если токен бинарная операция
        else if (ops.isBinaryOperation ( token )) {
            if (evalStack.size ( ) < 2) {
                throw std::runtime_error ( "Insufficient operands for binary operator: " + token );
            }
            double a = evalStack.top ( ); evalStack.pop ( );
            double b = evalStack.top ( ); evalStack.pop ( );
            double result = ops.applyBinary ( token , b , a ); // b - a для правильного порядка
            evalStack.push ( result );
        }
        else {
            throw std::runtime_error ( "Unknown token in RPN: " + token );
        }
    }

    if (evalStack.size ( ) != 1) {
        throw std::runtime_error ( "Invalid expression" );
    }

    return evalStack.top ( );
}