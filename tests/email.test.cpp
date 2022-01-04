/*
 * I don't know how to test this except for doing the whole thing and manually
 * checking the email.
 *
 * IMPORTANT: unless an scheme to use absolute paths or something more robust
 * such as checking for existing directories and files is implemented, this
 * file must be compiled in the /stock-quote-alert/ directory as it needs 
 * resources from that directory.
 *
 * Don't forget to compile with `-l curl`*/
#include "./email.h"
#include "./email.cpp"
#include <fstream>


int main() {
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

    PriceAlert alert {
        "ITUB4",
        22.39,
        "15:00:00",
        22.30,
        "sell"
    };

    composeMail(alert, settings);
    sendEmail(settings);
    return 0;
}

