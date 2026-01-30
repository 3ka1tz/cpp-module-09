#include "BitcoinExchange.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path/to/file>\n";
        return 1;
    }

    try
    {
        BitcoinExchange original(argv[1]);
        BitcoinExchange copyConstructed(original);
        BitcoinExchange copyAssigned;
        copyAssigned = copyConstructed;

        copyAssigned.displayResult();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
