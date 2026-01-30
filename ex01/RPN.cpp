#include "RPN.hpp"

#include <cctype>       // isdigit, isspace
#include <cstddef>      // size_t
#include <sstream>      // istringstream
#include <stack>        // stack
#include <stdexcept>    // invalid_argument

RPN::RPN()
{}

RPN::RPN(const std::string& expression) : expression(expression)
{}

RPN::RPN(const RPN& other) : expression(other.expression)
{}

RPN& RPN::operator=(const RPN& other)
{
    if (this != &other)
        expression = other.expression;

    return *this;
}

RPN::~RPN()
{}

bool isOnlyWhitespace(const std::string& expression)
{
    for (size_t i = 0; i < expression.size(); ++i)
    {
        if (!std::isspace(static_cast<unsigned char>(expression[i])))
            return false;
    }

    return true;
}

bool isOperator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

double calc(double n1, double n2, char op)
{
    switch (op)
    {
        case '+': return n1 + n2;
        case '-': return n1 - n2;
        case '*': return n1 * n2;
        case '/':
            if (n2 == 0)
                throw std::invalid_argument("Math Error: division by zero");
            return n1 / n2;
        default:
            throw std::invalid_argument("Error: invalid operator");
    }
}

double RPN::displayResult()
{
    std::stack<double> stack;

    if (isOnlyWhitespace(expression))
        throw std::invalid_argument("Error: expression is empty");

    std::istringstream iss(expression);
    std::string token;

    while (iss >> token)
    {
        if (token.size() == 1 && isOperator(token[0]) && stack.empty())
            throw std::invalid_argument("Error: expression cannot start with an operator");

        if (token.size() == 1 && isOperator(token[0]))
        {
            if (stack.size() < 2)
                throw std::invalid_argument("Error: insufficient numbers");

            double n2 = stack.top(); stack.pop();
            double n1 = stack.top(); stack.pop();

            stack.push(calc(n1, n2, token[0]));
            continue;
        }

        if (token.size() == 1 && std::isdigit(static_cast<unsigned char>(token[0])))
        {
            stack.push(token[0] - '0');
            continue;
        }

        throw std::invalid_argument("Error: invalid token => " + token);
    }

    if (stack.size() != 1)
        throw std::invalid_argument("Error: missing operator");

    return stack.top();
}
