#include "Observer.h"
#include <iostream>
#include <unistd.h>


void Observer::watchPrices(assetList_t trackedAssets) {
    StockPrice currentPrice { };

    while(true) {
        for (const auto& asset : trackedAssets) {
            try {
                currentPrice = getPrice(std::get<0>(asset));
                this->checkPrice(currentPrice, std::get<1>(asset), std::get<2>(asset));
            }
            catch (char* errorMessage) {
                std::cout << errorMessage << '\n';
                break;
            }
        }
        sleep(120);
    }
}

void Observer::checkPrice(StockPrice currentPrice, double lower, double upper) {
    if (currentPrice.price >= upper) {
        this->alert.ticker = currentPrice.name;
        this->alert.price = currentPrice.price;
        this->alert.time = currentPrice.time;
        this->alert.alertPrice = upper;
        this->alert.action = "sell";
        composeMail(alert, smtpSettings);
        sendEmail(smtpSettings);
    }
    else if (currentPrice.price <= lower) {
        this->alert.ticker = currentPrice.name;
        this->alert.price = currentPrice.price;
        this->alert.time = currentPrice.time;
        this->alert.alertPrice = lower;
        this->alert.action = "buy";
        composeMail(alert, smtpSettings);
        sendEmail(smtpSettings);
    } 
}
