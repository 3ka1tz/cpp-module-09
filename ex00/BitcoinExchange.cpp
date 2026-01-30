#include "BitcoinExchange.hpp"

#include <cctype>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

BitcoinExchange::BitcoinExchange()
{}

BitcoinExchange::BitcoinExchange(const std::string& inputFilePath) : inputFilePath(inputFilePath)
{
    loadPriceHistory();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : inputFilePath(other.inputFilePath), btcPriceHistory(other.btcPriceHistory)
{}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
    if (this != &other)
    {
        inputFilePath = other.inputFilePath;
        btcPriceHistory = other.btcPriceHistory;
    }

    return *this;
}

BitcoinExchange::~BitcoinExchange()
{}

bool stringToDouble(const std::string& s, double& out)
{
    std::istringstream iss(s);

    if (!(iss >> out))
        return false;

    iss >> std::ws;
    return iss.eof();
}

void BitcoinExchange::loadPriceHistory()
{
    const std::string databaseFilePath = "data.csv";

    std::ifstream databaseFile(databaseFilePath.c_str());
    if (!databaseFile.is_open())
        throw std::runtime_error("Error: could not open database file: " + databaseFilePath);

    std::string line;
    std::getline(databaseFile, line);
    if (line != "date,exchange_rate")
        throw std::runtime_error("Error: invalid format in database file: " + databaseFilePath);

    while (std::getline(databaseFile, line))
    {
        std::istringstream lineStream(line);

        std::string date;
        std::getline(lineStream, date, ',');

        std::string exchangeRateString;
        std::getline(lineStream, exchangeRateString);
        double exchangeRate;
        stringToDouble(exchangeRateString, exchangeRate);

        btcPriceHistory[date] = exchangeRate;
    }

    databaseFile.close();
}

int getActualYear()
{
    std::time_t now = std::time(0);
    std::tm *lt = std::localtime(&now);
    return lt->tm_year + 1900;
}

bool isLeapYear(const int& year)
{
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int getActualMonth()
{
    std::time_t now = std::time(0);
    std::tm *lt = std::localtime(&now);
    return lt->tm_mon + 1;
}

int getActualDay()
{
    std::time_t now = std::time(0);
    std::tm *lt = std::localtime(&now);
    return lt->tm_mday;
}

bool isValidDate(const std::string& date)
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;

    for (int i = 0; i < 10; ++i)
    {
        if (i == 4 || i == 7)
            continue;

        if (!std::isdigit(static_cast<unsigned char>(date[i])))
            return false;
    }

    const int y = std::atoi(date.substr(0, 4).c_str());
    const int m = std::atoi(date.substr(5, 2).c_str());
    const int d = std::atoi(date.substr(8, 2).c_str());

    if (y < 2009 || y > getActualYear())
        return false;

    if (m < 1 || m > 12)
        return false;

    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    if (isLeapYear(y))
        daysInMonth[1] = 29;

    if (d < 1 || d > daysInMonth[m - 1])
        return false;

    if (y == getActualYear())
    {
        if (m > getActualMonth() || (m == getActualMonth() && d > getActualDay()))
            return false;
    }

    return true;
}

bool isValidValue(const double& value)
{
    if (value < 0.0)
    {
        std::cerr << "Error: not a positive number.\n";
        return false;
    }

    if (value > 1000.0)
    {
        std::cerr << "Error: too large a number.\n";
        return false;
    }

    return true;
}

void BitcoinExchange::printConversion(const std::string& date, double value)
{
    std::map<std::string, double>::const_iterator it = btcPriceHistory.find(date);

    if (it == btcPriceHistory.end())
    {
        it = btcPriceHistory.lower_bound(date);
        if (it != btcPriceHistory.begin())
            --it;
    }

    std::cout << date << " => " << value << " = " << value * it->second << '\n';
}

void BitcoinExchange::displayResult()
{
    std::ifstream inputFile(inputFilePath.c_str());
    if (!inputFile.is_open())
    {
        std::cerr << "Error: could not open input file: " << inputFilePath << '\n';
        return;
    }

    std::string line;
    std::getline(inputFile, line);
    if (line != "date | value")
    {
        std::cerr << "Error: invalid format in input file: " << inputFilePath << '\n';
        return;
    }

    while (std::getline(inputFile, line))
    {
        std::istringstream lineStream(line);

        std::string date;
        std::getline(lineStream, date, ' ');
        if (!isValidDate(date))
        {
            std::cerr << "Error: bad input => " << date << '\n';
            continue;
        }

        std::string valueString;
        std::getline(lineStream, valueString, ' ');
        std::getline(lineStream, valueString);
        double value;
        if (!stringToDouble(valueString, value))
        {
            std::cerr << "Error: bad input value => " << valueString << '\n';
            continue;
        }
        if (!isValidValue(value))
            continue;

        printConversion(date, value);
    }

    inputFile.close();
}
