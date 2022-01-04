#include "settings.h"
#include <fstream>
#include <iostream>
#include <sstream>

SMTPSettings getSettings(std::string settingsPath/* = "./keys/smtpSettings"*/){
    SMTPSettings settings {};
    std::ifstream settingsRaw { };

    try {
        settingsRaw.open(settingsPath, std::ios::in);
        if (!settingsRaw)
            throw "Could not open smtpSettings.\nThe process cannot proceed.";

        std::string ph;
        settingsRaw >> ph;
        getField(&settings.smtpServerURL, &ph);
        settingsRaw >> ph;
        getField(&settings.senderUserName, &ph);
        settingsRaw >> ph;
        getField(&settings.password, &ph);
        settingsRaw >> ph;
        getField(&settings.fromMail, &ph);
        settingsRaw >> ph;
        getField(&settings.toMail, &ph);
        settingsRaw >> ph;
        getField(&settings.pathCACertificate, &ph);

        settingsRaw.close();
    }
    catch(const char* errorMessage) {
        std::cout << errorMessage << std::endl;
        settingsRaw.close();
        std::exit(0); //not sure if previous local variables will be destructed
    }

    return settings;
}

void getField(std::string* field, std::string* content) {
    if (!content) {
        throw "Could not open smtpSettings. Settings field missing.\nThe process cannot proceed.";
    }
    *field = *content;
}
