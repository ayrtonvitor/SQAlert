#ifndef OPTIONS_H
#define OPTIONS_H
#include "monitoredAssetsDB.cpp"
#include <string>

class Options {
public:
    Options(char* argv[]);
private:
    std::string time;
    MonitoredAssetsDB assetsDB{};

    void addNewTracker(char*, char*, char*);
    void start();
    void checkAsset(char*);
    void updateAsset(char*, char*, char*);
    bool checkTicker(std::string);
};

#endif
