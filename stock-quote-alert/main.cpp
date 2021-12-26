#include "dataRequestController.h"
#include "emailController.h"
#include "settings.h"
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>


int main(int argc, char* argv[]) {
    std::string ticker { argv[1] };
    double lower, upper;
    std::stringstream convert{ argv[2] };
    convert >> upper;
    convert = static_cast<std::stringstream>(argv[3]);
    convert >> lower;
    
    PriceAlert alert {};
    SMTPSettings smtpSettings {getSettings()};

    ticker = validateTicker(ticker);

    std::unordered_set<std::string> tickerList;
    if (!(tickerSet(&tickerList)) && !(tickerList.find(ticker) == tickerList.end()))
        std::cout << "Asset code not supported.\n";
    else {
        // Keep monitoring until program stops
        StockPrice currentPrice { };
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
}


