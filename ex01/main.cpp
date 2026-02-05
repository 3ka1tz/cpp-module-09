#include "RPN.hpp"

#include <exception>    // exception
#include <iostream>     // cerr, cout

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " \"<expression>\"\n";
        return 1;
    }

    RPN original(argv[1]);
    RPN copyConstructed(original);
    RPN copyAssigned;
    copyAssigned = copyConstructed;

    try
    {
        std::cout << copyAssigned.displayResult() << '\n';
    } 
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
