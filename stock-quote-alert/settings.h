#ifndef SETTINGS_H
#define SETTINGS_H
#include "email.h"
#include <string>

SMTPSettings getSettings(std::string settingsPath = "./keys/smtpSettings");
void getField(std::string *field, std::string *content);
#endif
