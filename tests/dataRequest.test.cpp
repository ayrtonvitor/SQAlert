#include "catchConf.h"
#include "../stock-quote-alert/dataRequestController.h"
#include "../stock-quote-alert/dataRequestController.cpp"
#include <cstdlib>
#include <fstream>
#include <string>
#include <unordered_set>

TEST_CASE("makePriceRequest", "[HTTP], [API]") {
    SECTION("valid api key") { std::ifstream file { "../stock-quote-alert/keys/api.key" };
        std::string key {};
        file >> key;
        json resp;
        
        SECTION("valid tickers"){
            // Requiring only the ticker is enough, since we can't predict
            // the price and the time the api will return
            resp = makePriceRequest("EGIE3", key);
            REQUIRE( resp["symbol"] == "EGIE3" );

            resp = makePriceRequest("HGLG11", key);
            REQUIRE( resp["symbol"] == "HGLG11" );
        }
        SECTION("invalid tickers"){
            resp = makePriceRequest("ITTB3", key);
            REQUIRE( resp["error"] == true );

            resp = makePriceRequest("HGBW11", key);
            REQUIRE( resp["error"] == true );
        }
    }
    /* not implemented yet
    SECTION("invalid api key") {    
    }*/
}

TEST_CASE("validateTicker", "[ticker]"){
    REQUIRE( validateTicker("abev3") == "ABEV3" );
    REQUIRE( validateTicker("egie3") == "EGIE3");
    REQUIRE( validateTicker("WEGE3") == "WEGE3" );
    REQUIRE( validateTicker("GogL34") == "GOGL34");
    REQUIRE( validateTicker("MDIA") == "" ); // This shall be changed into an error
    REQUIRE( validateTicker("ITUB456") == "" ); // This shall be changed into an error

}


TEST_CASE("getPrice", "[STOCK], [PRICE]") {
    StockPrice resp { getPrice("EZTC3") };
    REQUIRE( resp.name == "EZTC3" );

    resp = getPrice("LREN3");
    REQUIRE( resp.name == "LREN3" );

    resp = getPrice("KNRI11");
    REQUIRE( resp.name == "KNRI11");

}

TEST_CASE("tickerSet") {
    std::unordered_set<std::string> tickerList;
    REQUIRE(tickerSet(&tickerList));
    
    std::system("mv ../stock-quote-alert/lib/assets ../stock-quote-alert/lib/assets1");
    bool result;
    try {
        result = !tickerSet(&tickerList);
    }
    catch (char*){
        ;
    }
    REQUIRE(result == false);
    
    std::system("mv ../stock-quote-alert/lib/assets1 ../stock-quote-alert/lib/assets");
}
