#include <iostream>
#include <sqlite3.h>

int main() {
    sqlite3* db;
    int code{};
    char* errorMsg{};
    const char path[] { "./stock_quote_alert.db" };

    code = sqlite3_open(path, &db); //open (create) database

    if (code){
        std::cout << "Could not locate the database in " << path << '\n';
        std::cout << sqlite3_errmsg(db);
    }
    else {
        std::cout << "Database opened successfully\n";
    }

    char sqlReq[] {"CREATE TABLE assets(" \
        "ticker         TEXT PRIMARY KEY    NOT NULL," \
        "date           DATE                NOT NULL," \
        "buy_price      NUMERIC             NOT NULL," \
        "sell_price     NUMERIC             NOT NULL);"};
    
    code = sqlite3_exec(db, sqlReq, 
                [](void*, int, char**, char**) { return 0;}, //mandatory callback
                0, &errorMsg); //first argument to the callback, error message address

    if(code != SQLITE_OK) {
        std::cout << "SQL error: " << errorMsg;
        sqlite3_free(errorMsg);
    }
    else {
        std::cout << "Table created successfully.\n";
    }

    sqlite3_close(db);
    return 0;
}
