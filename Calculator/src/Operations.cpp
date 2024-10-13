// Operations.cpp
#include "Operations.h"
#include <stdexcept>

Operations& Operations::getInstance() {
    static Operations instance;
    return instance;
}

Operations::Operations() {
    // Инициализация базовых операторов
    addBinaryOperation("+", [](double a, double b) { return a + b; }, 1);
    addBinaryOperation("-", [](double a, double b) { return a - b; }, 1);
    addBinaryOperation("*", [](double a, double b) { return a * b; }, 2);
    addBinaryOperation("/", [](double a, double b) {
        if (b == 0) throw std::runtime_error("Division by zero");
        return a / b;
        }, 2);
}

void Operations::addBinaryOperation(const std::string& name, BinaryOp operation, unsigned short priority) {
    binaryOps[name] = operation;
    priorityMap[name] = priority;
}

void Operations::addUnaryOperation(const std::string& name, UnaryOp operation, unsigned short priority) {
    unaryOps[name] = operation;
    priorityMap[name] = priority;
}

double Operations::applyBinary(const std::string& name, double a, double b) {
    if (binaryOps.find(name) != binaryOps.end()) {
        return binaryOps[name](a, b);
    }
    throw std::runtime_error("Binary operation not found: " + name);
}

double Operations::applyUnary(const std::string& name, double a) {
    if (unaryOps.find(name) != unaryOps.end()) {
        return unaryOps[name](a);
    }
    throw std::runtime_error("Unary operation not found: " + name);
}

bool Operations::isBinaryOperation(const std::string& name) {
    return binaryOps.find(name) != binaryOps.end();
}

bool Operations::isUnaryOperation(const std::string& name) {
    return unaryOps.find(name) != unaryOps.end();
}

unsigned short Operations::getPriority(const std::string& name) {
    if (priorityMap.find(name) != priorityMap.end()) {
        return priorityMap[name];
    }
    throw std::runtime_error("Operation not found: " + name);
}
