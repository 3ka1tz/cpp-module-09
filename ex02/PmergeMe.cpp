#include "PmergeMe.hpp"

#include <climits>  // INT_MAX
#include <cstddef>  // size_t
#include <cstdlib>  // atoi, strtol
#include <ctime>    // CLOCKS_PER_SEC, clock, clock_t
#include <iostream> // cerr, cout

PmergeMe::PmergeMe()
{}

bool isValidArg(const std::string& arg)
{
    if (arg.empty())
        return false;

    char* end;
    const char* cstr = arg.c_str();

    long value = std::strtol(cstr, &end, 10);

    if (end == cstr || *end != '\0')
        return false;

    if (value < 0 || value > INT_MAX)
        return false;

    return true;
}

bool areValidArgs(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (!isValidArg(argv[i]))
        {
            std::cerr << "Error: bad input => " << argv[i] << '\n';
            return false;
        }
    }

    return true;
}

PmergeMe::PmergeMe(int argc, char* argv[])
{
    if (!areValidArgs(argc, argv))
        return;
    
    for (int i = 1; i < argc; ++i)
    {
        int n = std::atoi(argv[i]);
        deq.push_back(n);
        vec.push_back(n);
    }
}

PmergeMe::PmergeMe(const PmergeMe& other) : deq(other.deq), vec(other.vec)
{}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
    if (this != &other)
    {
        deq = other.deq;
        vec = other.vec;
    }

    return *this;
}

PmergeMe::~PmergeMe()
{}

template <typename Container>
bool isContainerSorted(const Container& c)
{
    typename Container::const_iterator curr = c.begin();
    typename Container::const_iterator next = curr;
    ++next;

    for (; next != c.end(); ++curr, ++next)
    {
        if (*curr > *next)
            return false;
    }

    return true;
}

template <typename Container>
void printContainer(const Container& c)
{
    typename Container::const_iterator curr = c.begin();
    typename Container::const_iterator next = curr;
    ++next;

    for (; curr != c.end(); ++curr, ++next)
    {
        std::cout << *curr;

        if (next != c.end())
            std::cout << ' ';
    }

    std::cout << '\n';
}

template <typename Container>
Container jacobsthalUpTo(int n)
{
    Container c;

    int prev = 0;
    int last = 1;

    c.push_back(prev);
    c.push_back(last);

    while (true)
    {
        int next = last + 2 * prev;
        if (next >= n)
            break;

        c.push_back(next);

        prev = last;
        last = next;
    }

    return c;
}

template <typename Container>
Container fordJohnsonAlgorithm(Container c)
{
    typename Container::iterator curr = c.begin();
    typename Container::iterator next = curr;
    ++next;

    if (c.size() <= 2)
    {
        if (c.size() == 2 && *curr > *next)
            std::swap(*curr, *next);

        return c;
    }

    Container winers;
    Container losers;

    while (next != c.end())
    {
        if (*curr > *next)
        {
            winers.push_back(*curr);
            losers.push_back(*next);
        }
        else
        {
            winers.push_back(*next);
            losers.push_back(*curr);
        }

        ++curr;
        if (curr == c.end())
            break;

        ++curr;
        if (curr == c.end())
            break;

        next = curr;
        ++next;
    }
    winers = fordJohnsonAlgorithm(winers);

    Container J = jacobsthalUpTo<Container>(static_cast<int>(losers.size()));

    for (size_t i = 0; i < J.size(); ++i)
    {
        size_t idx = J[i];
        if (idx < losers.size())
        {
            typename Container::iterator pos = std::lower_bound(winers.begin(), winers.end(), losers[idx]);
            winers.insert(pos, losers[idx]);
        }
    }

    for (size_t i = 0; i < losers.size(); ++i)
    {
        bool inJacob = false;
        for (size_t j = 0; j < J.size(); ++j)
        {
            if (J[j] == (int)i)
            {
                inJacob = true;
                break;
            }
        }
        if (!inJacob)
        {
            typename Container::iterator pos = std::lower_bound(winers.begin(), winers.end(), losers[i]);
            winers.insert(pos, losers[i]);
        }
    }

    int leftover = 0;
    bool hasLeftover = false;
    if (c.size() % 2)
    {
        leftover = c.back();
        hasLeftover = true;
    }

    if (hasLeftover)
    {
        typename Container::iterator pos = std::lower_bound(winers.begin(), winers.end(), leftover);
        winers.insert(pos, leftover);
    }

    return winers;
}

void PmergeMe::displayResult()
{
    if (deq.empty())
        return;

    if (isContainerSorted(deq))
    {
        std::cout << "The introduced sequence is already sorted.\n";
        return; 
    }

    std::cout << "Before:  ";
    printContainer(deq);

    std::clock_t start = std::clock();
    deq = fordJohnsonAlgorithm(deq);
    std::clock_t end = std::clock();
    double elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000;

    std::cout << "After:   ";
    printContainer(deq);

    std::cout << "Time to process a range of " << deq.size() << " elements with std::deque  : " << elapsed << " ms\n";

    start = std::clock();
    vec = fordJohnsonAlgorithm(vec);
    end = std::clock();
    elapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000;

    std::cout << "Time to process a range of " << vec.size() << " elements with std::vector : " << elapsed << " ms\n";
}
