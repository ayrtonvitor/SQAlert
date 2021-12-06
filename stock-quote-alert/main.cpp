#include "dataRequestController.cpp"
#include "dataRequestController.h"
#include "emailController.h"
#include "emailController.cpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

SMTPSettings getSettings();

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
    while(1) {
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
SMTPSettings getSettings(){
    std::ifstream settingsRaw { "./keys/smtpSettings"};
    SMTPSettings settings {};

    std::string ph;
    settingsRaw >> ph;
    settings.smtpServerURL = ph;
    settingsRaw >> ph;
    settings.senderUserName = ph;
    settingsRaw >> ph;
    settings.password = ph;
    settingsRaw >> ph;
    settings.fromMail = ph;
    settingsRaw >> ph;
    settings.toMail = ph;
    settingsRaw >> ph;
    settings.pathCACertificate = ph;

    settingsRaw.close();
    return settings;
}

