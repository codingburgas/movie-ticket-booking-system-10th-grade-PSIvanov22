#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <vector>
#include <map>

struct sqlite3; // Forward declaration for sqlite3 pointer

namespace DBManager {

    bool initDB(const std::string& dbPath);

    void closeDB();

    bool saveSeatState(int movieIndex, int row, int col, int status);

    bool loadSeatsForMovie(int movieIndex, int** seats);

    bool saveFoodSale(int movieIndex, const std::string& itemType, int quantity, int pricePerUnit);

    void getSeatCounts(int movieIndex, int& vip, int& premium, int& common);

    std::map<std::string, int> getFoodSales(int movieIndex);

    int calculateTotalRevenue(int movieIndex);

}

#endif