#include "catchConf.h"
#include "../stock-quote-alert/email.h"
#include "../stock-quote-alert/settings.cpp"
#include "../stock-quote-alert/settings.h"
#include <string>


TEST_CASE("getField") {
    std::string testField{};
    std::string testContent{"thisshouldwork"};
    getField(&testField, &testContent);
    REQUIRE(testField == testContent);

    testField = "x";
    
    REQUIRE(testField != testContent);
}

TEST_CASE("getSettings", "[SMTP]"){
    SMTPSettings settings{};
    settings = getSettings("./smtpSettingsExample");
    REQUIRE(settings.smtpServerURL == "smtp.gmail.com");
    REQUIRE(settings.senderUserName == "user");
    REQUIRE(settings.password == "senhaqualquer");
    REQUIRE(settings.fromMail == "seu@email.com");
    REQUIRE(settings.toMail == "destinatario@mail.com");
    REQUIRE("certificate.pem");
}
