#pragma once

#include <string>   // string

class RPN
{
    public:
        RPN();
        RPN(const std::string& expression);
        RPN(const RPN& other);
        RPN& operator=(const RPN& other);
        ~RPN();

        double displayResult();

    private:
        std::string expression;
};
