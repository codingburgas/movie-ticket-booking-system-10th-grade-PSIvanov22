#include "DatabaseManager.h"
#include <iostream>
#include <sqlite3.h> // Make sure this line is here!
#include <sstream>
#include "SeatManager.h"

using namespace std;

namespace DBManager {

    sqlite3* db;

    static int callback(void* data, int argc, char** argv, char** azColName) {
        return 0;
    }

    bool initDB(const string& dbPath) {
        int rc = sqlite3_open(dbPath.c_str(), &db);
        if (rc) {
            cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
            return false;
        }
        else {
            cout << "Opened database successfully: " << dbPath << endl;
        }

        const char* createSeatsTableSQL =
            "CREATE TABLE IF NOT EXISTS seats ("
            "movie_id INTEGER NOT NULL,"
            "row INTEGER NOT NULL,"
            "col INTEGER NOT NULL,"
            "status INTEGER NOT NULL,"
            "PRIMARY KEY (movie_id, row, col));";

        const char* createFoodSalesTableSQL =
            "CREATE TABLE IF NOT EXISTS food_sales ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "movie_id INTEGER NOT NULL,"
            "item_type TEXT NOT NULL,"
            "quantity INTEGER NOT NULL,"
            "price_per_unit INTEGER NOT NULL);";

        char* errMsg = 0;
        rc = sqlite3_exec(db, createSeatsTableSQL, callback, 0, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL error (create seats): " << errMsg << endl;
            sqlite3_free(errMsg);
            return false;
        }

        rc = sqlite3_exec(db, createFoodSalesTableSQL, callback, 0, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL error (create food sales): " << errMsg << endl;
            sqlite3_free(errMsg);
            return false;
        }

        cout << "Tables checked/created successfully.\n";
        return true;
    }

    void closeDB() {
        sqlite3_close(db);
        cout << "Database closed.\n";
    }

    bool saveSeatState(int movieIndex, int row, int col, int status) {
        stringstream ss;
        ss << "INSERT OR REPLACE INTO seats (movie_id, row, col, status) VALUES ("
            << movieIndex << ", " << row << ", " << col << ", " << status << ");";

        char* errMsg = 0;
        int rc = sqlite3_exec(db, ss.str().c_str(), callback, 0, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL error (save seat): " << errMsg << endl;
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }

    bool loadSeatsForMovie(int movieIndex, int** seats) {
        stringstream ss;
        ss << "SELECT row, col, status FROM seats WHERE movie_id = " << movieIndex << ";";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            cerr << "SQL error (prepare load seats): " << sqlite3_errmsg(db) << endl;
            return false;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int row = sqlite3_column_int(stmt, 0);
            int col = sqlite3_column_int(stmt, 1);
            int status = sqlite3_column_int(stmt, 2);
            if (row >= 0 && row <= ROWS && col >= 0 && col <= COLS) {
                seats[row][col] = status;
            }
        }
        sqlite3_finalize(stmt);
        return true;
    }

    bool saveFoodSale(int movieIndex, const string& itemType, int quantity, int pricePerUnit) {
        stringstream ss;
        ss << "INSERT INTO food_sales (movie_id, item_type, quantity, price_per_unit) VALUES ("
            << movieIndex << ", '" << itemType << "', " << quantity << ", " << pricePerUnit << ");";

        char* errMsg = 0;
        int rc = sqlite3_exec(db, ss.str().c_str(), callback, 0, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL error (save food sale): " << errMsg << endl;
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }

    void getSeatCounts(int movieIndex, int& vip, int& premium, int& common) {
        vip = 0; premium = 0; common = 0;

        stringstream ss;
        ss << "SELECT row FROM seats WHERE movie_id = " << movieIndex << " AND status = 1;";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            cerr << "SQL error (prepare get seat counts): " << sqlite3_errmsg(db) << endl;
            return;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int row = sqlite3_column_int(stmt, 0);
            if (row >= 1 && row <= 3) { // VIP rows
                vip++;
            }
            else if (row >= 4 && row <= 7) { // Premium rows
                premium++;
            }
            else if (row >= 8 && row <= 10) { // Common rows
                common++;
            }
        }
        sqlite3_finalize(stmt);
    }

    map<string, int> getFoodSales(int movieIndex) {
        map<string, int> sales;
        stringstream ss;
        ss << "SELECT item_type, SUM(quantity) FROM food_sales WHERE movie_id = " << movieIndex << " GROUP BY item_type;";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            cerr << "SQL error (prepare get food sales): " << sqlite3_errmsg(db) << endl;
            return sales;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string itemType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int quantity = sqlite3_column_int(stmt, 1);
            sales[itemType] = quantity;
        }
        sqlite3_finalize(stmt);
        return sales;
    }

    int calculateTotalRevenue(int movieIndex) {
        int revenue = 0;

        stringstream seatRevenueSS;
        seatRevenueSS << "SELECT row FROM seats WHERE movie_id = " << movieIndex << " AND status = 1;";
        sqlite3_stmt* seatStmt;
        int rc = sqlite3_prepare_v2(db, seatRevenueSS.str().c_str(), -1, &seatStmt, 0);
        if (rc == SQLITE_OK) {
            while (sqlite3_step(seatStmt) == SQLITE_ROW) {
                int row = sqlite3_column_int(seatStmt, 0);
                if (row >= 1 && row <= 3) revenue += 50;
                else if (row >= 4 && row <= 7) revenue += 30;
                else if (row >= 8 && row <= 10) revenue += 15;
            }
            sqlite3_finalize(seatStmt);
        }
        else {
            cerr << "SQL error (calc seat revenue): " << sqlite3_errmsg(db) << endl;
        }

        stringstream foodRevenueSS;
        foodRevenueSS << "SELECT SUM(quantity * price_per_unit) FROM food_sales WHERE movie_id = " << movieIndex << ";";
        sqlite3_stmt* foodStmt;
        rc = sqlite3_prepare_v2(db, foodRevenueSS.str().c_str(), -1, &foodStmt, 0);
        if (rc == SQLITE_OK) {
            if (sqlite3_step(foodStmt) == SQLITE_ROW) {
                revenue += sqlite3_column_int(foodStmt, 0);
            }
            sqlite3_finalize(foodStmt);
        }
        else {
            cerr << "SQL error (calc food revenue): " << sqlite3_errmsg(db) << endl;
        }

        return revenue;
    }

}