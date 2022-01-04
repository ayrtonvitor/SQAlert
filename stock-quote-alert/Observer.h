#ifndef OBSERVER_H
#define OBSERVER_H

#include "dataRequest.h"
#include "email.h"
#include "settings.h"
#include <tuple>
#include <vector>

using assetTuple_t = std::tuple<std::string, double, double>;
using assetList_t = std::vector<assetTuple_t>;

class Observer {
public:
    Observer() {
        smtpSettings = getSettings();
    }
    void watchPrices(assetList_t trackedAssets);
private:
    PriceAlert alert;
    SMTPSettings smtpSettings;

    void checkPrice(StockPrice currentPrice, double lower, double upper);
};

#endif
