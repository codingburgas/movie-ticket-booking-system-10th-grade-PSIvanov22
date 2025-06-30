#include <iostream>
#include <iomanip> // For formatting output (though not explicitly used for fixed/precision here)
#include <string>
#include <algorithm> // For tolower, max
#include <cctype> // For tolower
#include <map> // For std::map used in food sales
#include "SeatManager.h"
#include "DatabaseManager.h"

using namespace std;

extern const char* movieTitles[MOVIE_COUNT];
extern const char* movieTimes[MOVIE_COUNT];

void reserveSeat(int*** allSeats, int movieIndex, int& row, int& col) // row and col passed by reference
{
    clearScreen();
    printLogo();

    int** seats = allSeats[movieIndex]; // Get seats for the current movie

    cout << "Movie: " << movieTitles[movieIndex] << " at " << movieTimes[movieIndex] << "\n";
    displaySeatingMap(seats); // Show the seating map for the selected movie
    cout << "\n============PRICES===========" << endl;
    cout << "Rows 1 to 3: $50.00 (VIP)\nRows 4 to 7: $30.00 (Premium)\nRows 8 to 10: $15.00 (Common)";
    cout << "\n\nWhich row would you like to reserve?: ";
    cin >> row; // Get row input
    cout << "Which column would you like to reserve?: ";
    cin >> col; // Get column input

    if (row >= 1 && row <= ROWS &&
        col >= 1 && col <= COLS) // Check if input is within valid range
    {
        if (seats[row][col] == 0) // Check if seat is available (0 for available)
        {
            seats[row][col] = 1; // Mark seat as reserved (1 for reserved)
            DBManager::saveSeatState(movieIndex, row, col, 1); // Save seat state to DB

            clearScreen();
            printLogo();
            cout << "============RESERVATION===========" << endl;
            cout << "\nSeat successfully reserved!\n\n";

            char buyFoodChoice;
            cout << "\nWould you like to purchase any food/drinks? (y/n): ";
            cin >> buyFoodChoice;
            cin.ignore(); // Consume the newline character left by cin >> buyFoodChoice

            if (tolower(buyFoodChoice) == 'y') {
                char addMoreFood = 'y';
                while (tolower(addMoreFood) == 'y') {
                    clearScreen();
                    printLogo();
                    cout << "\n============ FOOD OPTIONS ============\n";
                    cout << "1. Popcorn ($10)\n";
                    cout << "2. Soda ($5)\n";
                    cout << "3. Candy ($7)\n";
                    cout << "Select an item: ";

                    int itemChoice;
                    cin >> itemChoice; // Get food item choice

                    int foodPrice = 0;
                    string foodName;
                    int quantity = 0;

                    switch (itemChoice) {
                    case 1:
                        foodPrice = 10;
                        foodName = "Popcorn";
                        break;
                    case 2:
                        foodPrice = 5;
                        foodName = "Soda";
                        break;
                    case 3:
                        foodPrice = 7;
                        foodName = "Candy";
                        break;
                    default:
                        cout << "Invalid food choice. No food added for this selection.\n";
                        foodPrice = 0; // Reset price if invalid choice
                    }

                    if (foodPrice > 0) { // If a valid food item was chosen
                        cout << "How many portions of " << foodName << " would you like?: ";
                        cin >> quantity; // Get quantity
                        if (quantity > 0) {
                            DBManager::saveFoodSale(movieIndex, foodName, quantity, foodPrice); // Save food sale to DB
                            cout << quantity << " " << foodName << "(s) added. Cost: $" << (foodPrice * quantity) << ".\n";
                        }
                        else {
                            cout << "Invalid quantity. No food added for this selection.\n";
                        }
                    }

                    cout << "\nWould you like to add more food/drinks? (y/n): ";
                    cin >> addMoreFood;
                    cin.ignore(); // Consume the newline
                }
            }
        }
        else // Seat is already taken
        {
            clearScreen();
            printLogo();
            cout << "Seat is already taken!\n";
        }
    }
    else // Invalid coordinates
    {
        clearScreen();
        printLogo();
        cout << "Invalid seat coordinates!\n";
    }

    cout << "\nPress ENTER to continue...";
    cin.get(); // Wait for user to press enter
}

void showRevenue(int movieIndex)
{
    clearScreen();
    printLogo();

    const int CONSOLE_WIDTH = 80;
    string reportTitle = "============ REVENUE REPORT ============";
    int titlePadding = max(0, (CONSOLE_WIDTH - (int)reportTitle.length()) / 2);

    cout << "\n";
    cout << string(titlePadding, ' ') << reportTitle << "\n";
    cout << "\n";

    string movieInfo = "Movie: " + string(movieTitles[movieIndex]) + " at " + string(movieTimes[movieIndex]);
    int movieInfoPadding = max(0, (CONSOLE_WIDTH - (int)movieInfo.length()) / 2);
    cout << string(movieInfoPadding, ' ') << movieInfo << "\n\n";

    int categoryIndent = 25;

    int vipSeats, premiumSeats, commonSeats;
    DBManager::getSeatCounts(movieIndex, vipSeats, premiumSeats, commonSeats); // Get seat counts from DB

    cout << string(categoryIndent, ' ') << "VIP seats reserved: " << vipSeats << endl;
    cout << string(categoryIndent, ' ') << "Premium seats reserved: " << premiumSeats << endl;
    cout << string(categoryIndent, ' ') << "Common seats reserved: " << commonSeats << endl;

    cout << "\n";

    string foodHeader = "--- Food and Drinks Sales ---";
    int foodHeaderPadding = max(0, (CONSOLE_WIDTH - (int)foodHeader.length()) / 2);
    cout << string(foodHeaderPadding, ' ') << foodHeader << "\n";

    map<string, int> foodSales = DBManager::getFoodSales(movieIndex); // Get food sales from DB

    bool anyFoodSold = false;
    // Display food sales if any
    if (foodSales.count("Popcorn") > 0 && foodSales["Popcorn"] > 0) {
        cout << string(categoryIndent, ' ') << "Popcorn: " << foodSales["Popcorn"] << " x $10 = $" << (foodSales["Popcorn"] * 10) << endl;
        anyFoodSold = true;
    }
    if (foodSales.count("Soda") > 0 && foodSales["Soda"] > 0) {
        cout << string(categoryIndent, ' ') << "Soda: " << foodSales["Soda"] << " x $5 = $" << (foodSales["Soda"] * 5) << endl;
        anyFoodSold = true;
    }
    if (foodSales.count("Candy") > 0 && foodSales["Candy"] > 0) {
        cout << string(categoryIndent, ' ') << "Candy: " << foodSales["Candy"] << " x $7 = $" << (foodSales["Candy"] * 7) << endl;
        anyFoodSold = true;
    }
    if (!anyFoodSold) {
        cout << string(categoryIndent, ' ') << "No food or drinks sold for this movie.\n";
    }

    cout << "\n";
    int totalRev = DBManager::calculateTotalRevenue(movieIndex); // Calculate total revenue from DB
    string totalRevenueText = "Total revenue: $" + to_string(totalRev);
    int totalRevenuePadding = max(0, (CONSOLE_WIDTH - (int)totalRevenueText.length()) / 2);
    cout << string(totalRevenuePadding, ' ') << totalRevenueText << endl;
}