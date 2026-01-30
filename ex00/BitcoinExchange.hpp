#pragma once

#include <map>
#include <string>

class BitcoinExchange
{
    public:
        BitcoinExchange();
        BitcoinExchange(const std::string& inputFilePath);
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);
        ~BitcoinExchange();

        void displayResult();

    private:
        std::string inputFilePath;
        std::map<std::string, double> btcPriceHistory;

        void loadPriceHistory();
        void printConversion(const std::string& date, double value);
};
