/* Implement options so that the user can better interact with the program.
 * You can track a new asset with `newTracker`, check if and with what price
 * limits a certain asset is being tracked using `query`, update the prices
 * limits of a tracker using update, remove an asset from the list with `remove`
 * and start the price tracking with `start`.
 * */
#include "Observer.h"
#include "Options.h"
#include <ctime>
#include <iostream>


Options::Options(char* argv[]) {
    std::time_t timeNow {std::time(0)};
    std::tm *ptm {std::localtime(&timeNow)};
    char timeStr[22];
    std::strftime(timeStr, 22, "%H:%M:%S (%d.%m.%Y)", ptm);
    this->time = timeStr;

    char mode {argv[1][0]};
    switch (mode){
        case 'a':
            this->addNewTracker(argv[2], argv[3], argv[4]);
            break;
        case 'c':
            this->checkAsset(argv[2]);
            break;
        case 'r':
            this->assetsDB.stopTrackingAsset(argv[2]);
            break;
        case 'u':
            this->updateAsset(argv[2], argv[3], argv[4]);
            break;
        case 's':
            this->start();
            break;
        default:
            std::cout << "Cannot identify option " << argv[1] << "\nTry"
                << "`sqalert add [ticker] [lower] [upper]` to track new asset.\n"
                << "`sqalert start` to begin tracking assets.\n"
                << "`sqalert update [ticker] [lower] [upper]` to update price "
                << "limits for a already tracked asset.\n"
                << "`sqalert check [ticker]` to see if an asset is being tracked"
                << " or to check an older price limit."
                << "`sqalert remove [ticker]` to remove an asset from the list.\n";
            break;
    }
}


void Options::start() {
    /* Get list of tickers and, for each of them, check their prices*/
    assetList_t trackedAssets{ assetsDB.getAssetList() };
    assetsDB.closeDataBase();

    Observer tracker;
    tracker.watchPrices(trackedAssets);
}

void Options::addNewTracker(char* ticker, char* lower, char* upper){
    /* Validates ticker and adds it to the database if valid.*/

    const std::string vTicker{ validateTicker(ticker).c_str() };
    if (this->checkTicker(vTicker)) { //for some reason, the program is unable to implicitly convert char*
           // to std::string properly, so I am converting them manually
        const std::string vLower { lower };
        const std::string vUpper { upper };
        this->assetsDB.trackNewAsset(vTicker, this->time, vLower, vUpper);
    }
}

void Options::checkAsset(char* ticker) {
    const std::string vTicker{ validateTicker(ticker).c_str() };
    if (this->checkTicker(vTicker))
        this->assetsDB.checkTracker(vTicker);
}

void Options::updateAsset(char* ticker, char* lower, char* upper){
    const std::string vTicker{ validateTicker(ticker).c_str() };
    if (this->checkTicker(vTicker))
        this->assetsDB.updateTrackedAsset(ticker, this->time, lower, upper);
}

bool Options::checkTicker(std::string ticker) {
    std::unordered_set<std::string> tickerList;
    tickerSet(&tickerList);
    if (tickerList.find(ticker) == tickerList.end()){
        std::cout << "Asset code not supported.\n";
        this->assetsDB.closeDataBase();
        return false;
    }
    else return true;
}
