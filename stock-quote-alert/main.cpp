//#include "dataRequestController.h"
//#include "emailController.h"
//#include "monitoredAssetsDB.cpp"
#include "Options.h"
//#include "settings.h"
//#include <cstdlib>
#include <iostream>
//#include <sstream>
//#include <string>
//#include <unistd.h>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "What functionality do you want to perform?\n"
            << "Enter `SQAlert add [SYMBOL] [lower_price] [upper_price]` "
            << "to monitor new stock.\n";
    }
    else {
        Options opt(argv);
    }
    return 0;
}

 /*   double lower, upper;
    std::stringstream convert{ cLower };
    convert >> upper;
    convert = static_cast<std::stringstream>(cUpper);
    convert >> lower; 
    
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
    */

