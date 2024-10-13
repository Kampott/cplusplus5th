// Operations.h
#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>
#include <map>
#include <functional>

class Operations {
public:
    using BinaryOp = std::function<double(double, double)>;
    using UnaryOp = std::function<double(double)>;

    // Singleton
    static Operations& getInstance();

    // Добавление унарных и бинарных операций
    void addBinaryOperation(const std::string& name, BinaryOp operation, unsigned short priority);
    void addUnaryOperation(const std::string& name, UnaryOp operation, unsigned short priority);

    // Вычисление операций
    double applyBinary(const std::string& name, double a, double b);
    double applyUnary(const std::string& name, double a);

    // Проверка наличия операторов
    bool isBinaryOperation(const std::string& name);
    bool isUnaryOperation(const std::string& name);

    // Получение приоритета операции
    unsigned short getPriority(const std::string& name);

private:
    Operations(); // Приватный конструктор для Singleton

    std::map<std::string, BinaryOp> binaryOps;
    std::map<std::string, UnaryOp> unaryOps;
    std::map<std::string, unsigned short> priorityMap;
};

#endif // OPERATIONS_H
