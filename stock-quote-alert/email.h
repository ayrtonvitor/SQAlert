#ifndef EMAIL_H
#define EMAIL_H
#include <string>
struct SMTPSettings {
    std::string smtpServerURL {};
    std::string senderUserName {};
    std::string password {};
    std::string fromMail {};
    std::string toMail {};
    std::string pathCACertificate {"./keys/certificate.pem"};
};
struct PriceAlert {
    std::string ticker {};
    double price {};
    std::string time {};
    double alertPrice {};
    std::string action {};
};
void composeMail(PriceAlert stock, SMTPSettings smtpSettings);
void sendEmail(SMTPSettings settings);
#endif
