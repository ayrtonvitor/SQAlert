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

    // Keep monitoring until program stops
    while(true) {
        StockPrice currentPrice { getPrice(ticker) };

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


