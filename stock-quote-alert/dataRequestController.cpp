/*
 * Use libcurl to get stockprice from HG Brasil API
 *
 * Based on: https://curl.se/libcurl/c/getinmemory.html
 *****************************************************************************/

#include "dataRequestController.h"
#include "./lib/json.hpp"
#include <algorithm>
#include <curl/curl.h>
#include <cctype>                       //isdigit
#include <cstdlib>                      //malloc, realloc
#include <cstring>                      //memcpy
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
//using json = nlohmann::json;  //clause in dataRequestController.h


// Struct to handle the API response
// Memory points to the returned JSON copied in memory
struct MemoryStruct {
    char *memory {};
    std::size_t size {};
};

static std::size_t writeMemoryCallback( void *contents, std::size_t size,
                                            std::size_t nmemb, void *userp ){
    /* Callback function to handle HTTP response using struct MemoryStruct
     * This function is an option for curl_easy_setopt() with
     * CURLOPT_WRITEFUNCTION option.
     *
     * Parameters
     * ----------
     *  contents: pointer to the data curl retrieved
     *  size: size_of(&contents)
     *  nmemb: size of the reponse curl got in each data
     *  userp: pointer to a MemoryStruct holding the full response
     * */
    std::size_t realsize { size * nmemb };
    MemoryStruct *mem { (MemoryStruct *)userp };

    // More memory to fit incoming data    ///why realloc, though?
    char *ptr { static_cast<char*> (std::realloc(mem->memory,
                                                mem->size + realsize + 1)) };
    if (!ptr){
        std::cout << "Not enough memory (failled to realoc memory)\n";
        return 0;
    }

    mem->memory = ptr;
    std::memcpy(&(mem->memory[mem->size]), contents, realsize); // more efficient than strcpy
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

json makePriceRequest( std::string ticker, std::string apiKey ) {
    /* Gets data on user specified stock price from HGBrasil API
     *
     * RETURNS: nlohmann::json object with the API response.
     * */
    CURL *curl_handle;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    std::string url { "https://api.hgbrasil.com/finance/stock_price?key="
                        + apiKey + "&symbol=" + ticker };
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeMemoryCallback);

    // Store data in the variable apiResponse
    MemoryStruct apiResponse;
    apiResponse.memory = static_cast<char*>(std::malloc(1));
    apiResponse.size = 0;
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&apiResponse);
    json respJSON;

    try {
        res = curl_easy_perform(curl_handle);

        if (res != CURLE_OK) {
            throw -1;
        }
        
        respJSON = json::parse(apiResponse.memory);
        std::free(apiResponse.memory);

        if (!respJSON["valid_key"]) {
            throw "Invalid API key.";
        }
        return respJSON["results"][ticker];
    }
    catch (int){
        std::cout << "curl_easy_perform() failed: "
                  << curl_easy_strerror(res) << '\n';
        return json::parse("{}");
}

}

std::string validateTicker(std::string ticker) {
    /* Filters (some) invalid tickers and make sure they are uppercase.
     * Prevents bad API requests (limited resources).
     * Used only when list of available assets cannot be loaded.
     *
     * RETURNS: upper case supposedly valid ticker or "" if invalid*/

    std::string validTicker (ticker.length(), ' '); //constructor with correct length
    if (ticker.length() == 5 
            && std::isdigit(static_cast<unsigned char>(ticker[4]))){
        for (int i { 0 }; i < 5; i++)
            validTicker[i] = std::toupper(ticker[i]);
    }
    else if (ticker.length() == 6 
                && std::isdigit(static_cast<unsigned char>(ticker[4]))
                && std::isdigit(static_cast<unsigned char>(ticker[5]))){
        for (int i { 0 }; i < 6; i++)
            validTicker[i] = std::toupper(ticker[i]);
    }
    else validTicker = "";
    
    return validTicker;
}

bool tickerSet(std::unordered_set<std::string>* tickers) {
    /* Gets the set of tickers which the API supports.
     * Returns `true` if could load file and input the elements in the set.
     * */

    std::ifstream assets{};
    try {
        assets.open("lib/assets");
        if (!assets)
            throw "Could not find list of assets.";
        while (assets) {
            std::string ticker;
            assets >> ticker;
            (*tickers).insert(ticker);
        }
    }
    catch (const char* errorMessage) {
        std::cout << errorMessage << '\n';
        return false;
    }

    return true;
}

StockPrice getPrice(std::string ticker) {
    /* Gets stock price for a specified ticker*/
    std::ifstream file { "keys/api.key" }; //should raise error if not able to load key
    std::string key;
    file >> key;
    
    /*for some reason, there is a problem with c++ specs and braces 
    initialization will cause the json response to get back as an array of
    some sort. Use `=` initialization, if needed*/
    json resp = makePriceRequest(ticker, key);


    try {
        if (resp["error"]) {
            throw "API couldn't respond properly. Check if ticker is correct or supported";
        }
    }
    catch (json::exception) { ; }

    return {
        resp["symbol"],
        resp["updated_at"],
        resp["price"]
    };
}

