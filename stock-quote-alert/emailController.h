#ifndef EMAILCONTROLLER_H
#define EMAILCONTROLLER_H
//#include priceController.h
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
//const char* composeMail(PriceAlert stock, SMTPSettings smtpSettings);
//void sendEmail(const char* payload, SMTPSettings settings);
#endif
