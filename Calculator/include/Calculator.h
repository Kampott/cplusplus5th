#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <stdexcept>

class Calculator {
public:
    // �����������
    Calculator();

    // ����� ����������� ������� (�����. sin, cos, pow) ����� �������
    void registerFunction(const std::string& name, std::function<double(double)> func);

    // �������� ����� ��� ���������� ���������
    double evaluate(const std::string& expression);

private:
    // ����� ��� �������� ������������������ �������
    std::unordered_map<std::string, std::function<double(double)>> functions;

    // ������� ��������� � ���������� ����� RPN
    double parseExpression(const std::string& expr);

    // ����������� ������ ���������
    std::vector<std::string> tokenize(const std::string& expr);

    // �������������� ������� � �������� �������� ������� (RPN)
    std::vector<std::string> toRPN(const std::vector<std::string>& tokens);

    // ���������� ��������� � �������� �������� �������
    double evaluateRPN(const std::vector<std::string>& rpn);
};
class OutOfBoundsException : public std::runtime_error {
public:
    explicit OutOfBoundsException ( const std::string& message ) : std::runtime_error ( message ) {}
};
#endif // CALCULATOR_H