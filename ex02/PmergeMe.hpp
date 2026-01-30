#pragma once

#include <deque>    // deque
#include <vector>   // vector

class PmergeMe
{
    public:
        PmergeMe();
        PmergeMe(int argc, char* argv[]);
        PmergeMe(const PmergeMe& other);
        PmergeMe& operator=(const PmergeMe& other);
        ~PmergeMe();

        void displayResult();

    private:
        std::deque<int> deq;
        std::vector<int> vec;
};
