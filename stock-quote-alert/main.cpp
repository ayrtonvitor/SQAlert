#include "dataRequestController.h"
#include "emailController.h"
#include "settings.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

void priceMonitor(const std::string& ticker, double lower, double upper) {
    StockPrice currentPrice { };
    PriceAlert alert {};
    SMTPSettings smtpSettings {getSettings()};

    while(true) {
        try {
            currentPrice = getPrice(ticker);
        }
        catch (char* errorMessage) {
            std::cout << errorMessage << '\n';
            break;
        }

        if (currentPrice.price >= upper) {
            alert.ticker = currentPrice.name;
            alert.price = currentPrice.price;
            alert.time = currentPrice.time;
            alert.alertPrice = upper;
            alert.action = "sell";
            composeMail(alert, smtpSettings);
            sendEmail(smtpSettings);
        }
        else if (currentPrice.price <= lower) {
            alert.ticker = currentPrice.name;
            alert.price = currentPrice.price;
            alert.time = currentPrice.time;
            alert.alertPrice = lower;
            alert.action = "buy";
            composeMail(alert, smtpSettings);
            sendEmail(smtpSettings);
        } 

        sleep(120);
    }
}

int main(int argc, char* argv[]) {

    std::string ticker { argv[1] };
    double lower, upper;
    std::stringstream convert{ argv[2] };
    convert >> upper;
    convert = static_cast<std::stringstream>(argv[3]);
    convert >> lower;

    ticker = validateTicker(ticker);

    std::unordered_set<std::string> tickerList;
    if (!(tickerSet(&tickerList)) && !(tickerList.find(ticker) == tickerList.end()))
        std::cout << "Asset code not supported.\n";
    else {
        // Keep monitoring until program stops
        priceMonitor(ticker, lower, upper);
    }

    return 0;
}


