#include "PmergeMe.hpp"

#include <iostream> // cerr

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <positive integer sequence>\n";
        return 1;
    }

    PmergeMe original(argc, argv);
    PmergeMe copyConstructed(original);
    PmergeMe copyAssigned;
    copyAssigned = copyConstructed;

    copyAssigned.displayResult();
}
