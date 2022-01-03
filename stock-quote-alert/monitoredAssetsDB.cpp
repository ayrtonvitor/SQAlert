#ifndef MONITOREDASSETSDB_CPP
#define MONITOREDASSETSDB_CPP
#include <iostream>
#include <sqlite3.h>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using assetTuple_t = std::tuple<std::string, double, double>;
using assetList_t = std::vector<assetTuple_t>;

static assetList_t assetVector; //this should be a member variable, but I don't know
                        //how to call member functions through pointers, as
                        //they need to be static, and static functions can't
                        //modify non static member variables

class MonitoredAssetsDB {
public:
    MonitoredAssetsDB() {
        /*Opens database*/
        const char path[] { "dataBase/stock_quote_alert.db" };
        reqCode = sqlite3_open(path, &db);

        if (reqCode) {
            std::cout << "Could not open database in " << path << '\n';
            throw sqlite3_errmsg(db);
        }
    }
    
    ~MonitoredAssetsDB() {
        if (!errorMsg) sqlite3_free(errorMsg);
        sqlite3_close(db);
    }

    void checkTracker(const std::string& ticker) {
        /*Check if a given asset is being tracked and/or its price limits.*/
        std::string sqlReq{ "SELECT * FROM assets WHERE ticker = \'"
                            + ticker + "\';" };
        try {
            makeSQLReq(sqlReq, selectCallback);
            std::cout << std::get<0>(assetVector[0]) << ' '
                << std::get<1>(assetVector[0]) << ' '
                << std::get<2>(assetVector[0]) << '\n';
        }
        catch (const char* errorMessage) {
            std::cout << "Could not execute query.\n";
            std::cout << errorMessage << '\n';
            sqlite3_free(errorMsg);
        }
    }

    void trackNewAsset(const std::string& ticker, const std::string& date,
                const std::string& lower, const std::string& upper){
        /* Insert new asset to be tracked to the list.
         * Possible problem with SQL injection*/

        std::string sqlReq{ "INSERT INTO assets (ticker, date, buy_price, sell_price) " \
                    "VALUES ( \'" + ticker + "\', \'" + date + "\', "
                    + lower + ", " + upper +  " );"};
        try {
            makeSQLReq(sqlReq, defaultCallback);
        }
        catch (int) {
            std::cout << "Could not add asset to the list.\n";
            std::cout << errorMsg << '\n';
            sqlite3_free(errorMsg);
        }
    }

    const assetList_t getAssetList() {
        /* Retrieve data from all assets being tracked. 
         * Used by the main engine of the program.
         * 
         * Returns vector of tuples with ticker name, lower and upper price limits
         * */

        std::string sqlReq{ "SELECT * FROM assets;"};
        try {
            makeSQLReq(sqlReq.c_str(), selectCallback);
        }
        catch (const char* errorMessage) {
            std::cout << "Could not execute query.\n";
            std::cout << errorMessage << '\n';
            sqlite3_free(errorMsg);
        }
        assetList_t toReturn{ assetVector };
        assetVector.clear();
        return toReturn;
    }

    void stopTrackingAsset(const std::string& ticker) {
        /* Removes asset from the data base and it won't be tracked anymore.*/
        std::string sqlReq{ "DELETE FROM assets WHERE ticker = \'" + ticker + "\';"};
        std::cout << sqlReq;
        try {
            makeSQLReq(sqlReq, defaultCallback);
        }
        catch (const char* errorMessage) {
            std::cout << errorMessage << '\n';
            sqlite3_free(errorMsg);
        }

    }

    void updateTrackedAsset(const std::string& ticker,const std::string& date,
                const std::string& lower,const std::string& upper){
        /* Update value bounds for tracked asset.*/
        std::string sqlReq{ "UPDATE assets SET date = \'" + date
                    + "\', buy_price = " + lower + ", sell_price = "
                    + upper + " WHERE ticker = \'" + ticker + "\'; "};
        try {
            makeSQLReq(sqlReq, defaultCallback);
        }
        catch (const char* errorMessage) {
            std::cout << "Could not update values.\n";
            std::cout << errorMessage << '\n';
            sqlite3_free(errorMsg);
        }
        std::cout << "Updated ";
        this->checkTracker(ticker);
    }

    void closeDataBase(){
        sqlite3_free(errorMsg);
        sqlite3_close(db);
    }

private:
    sqlite3* db;
    int reqCode{};
    char* errorMsg{ nullptr };


    static int selectCallback(void* data, int argc, char** argv, char** colNames){
        double lower, upper;
        std::stringstream convert{ argv[2] };
        convert >> lower;
        convert = static_cast<std::stringstream>(argv[3]);
        convert >> upper;

        assetTuple_t asset{ argv[0], lower, upper };
        assetVector.push_back(asset);
        return 0;
    }

    static int defaultCallback(void*, int, char**, char**) {return 0;} //does nothing

    void makeSQLReq( std::string sqlReq, 
                     int (*callback)(void*, int, char**, char**) ) {

        try{
            reqCode = sqlite3_exec(db, sqlReq.c_str(), callback, 0, &errorMsg);
        }
        catch (int){
            std::cout << errorMsg << '\n';
        }
        
        if (reqCode != SQLITE_OK) {
            throw errorMsg;
        }
    }
};

#endif
