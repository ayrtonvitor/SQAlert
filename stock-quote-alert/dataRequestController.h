#ifndef DATAREQUEST_H
#define DATAREQUEST_H

#include "./lib/json.hpp"
#include <string>
#include <unordered_set>
using json = nlohmann::json;

struct StockPrice {
    std::string name {};
    std::string time {};
    double price {};
};

json makePriceRequest( std::string ticker, std::string apiKey );

std::string validateTicker(std::string ticker);

StockPrice getPrice(std::string inputTicker);

bool tickerSet(std::unordered_set<std::string>* tickers);

#endif
