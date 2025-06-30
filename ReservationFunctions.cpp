#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cctype>
#include "SeatManager.h"

using namespace std;

int vipCount[MOVIE_COUNT] = {};
int premiumCount[MOVIE_COUNT] = {};
int commonCount[MOVIE_COUNT] = {};
int totalRevenue[MOVIE_COUNT] = {};
int popcornSold[MOVIE_COUNT] = {};
int sodaSold[MOVIE_COUNT] = {};
int candySold[MOVIE_COUNT] = {};

const char* movieTitles[MOVIE_COUNT] = {
    "The Matrix", "Inception", "Interstellar", "The Godfather",
    "Pulp Fiction", "Avatar", "Avengers: Endgame", "Toy Story"
};
const char* movieTimes[MOVIE_COUNT] = {
    "14:00", "16:00", "18:00", "20:00",
    "22:00", "13:00", "17:00", "11:00"
};

void reserveSeat(int*** allSeats, int movieIndex, int& row, int& col)
{
    clearScreen();
    printLogo();

    int** seats = allSeats[movieIndex];

    cout << "Movie: " << movieTitles[movieIndex] << " at " << movieTimes[movieIndex] << "\n";
    displaySeatingMap(seats);
    cout << "\n============PRICES===========" << endl;
    cout << "Rows 1 to 3: $50.00 (VIP)\nRows 4 to 7: $30.00 (Premium)\nRows 8 to 10: $15.00 (Common)";
    cout << "\n\nWhich row would you like to reserve?: ";
    cin >> row;
    cout << "Which column would you like to reserve?: ";
    cin >> col;
    if (row >= 1 && row <= ROWS &&
        col >= 1 && col <= COLS)
    {
        if (seats[row][col] == 0)
        {
            seats[row][col] = 1;
            clearScreen();
            printLogo();
            cout << "============RESERVATION===========" << endl;
            cout << "\nSeat successfully reserved!\n\n";

            if (row <= 3) {
                vipCount[movieIndex]++;
                totalRevenue[movieIndex] += 50;
            }
            else if (row <= 7) {
                premiumCount[movieIndex]++;
                totalRevenue[movieIndex] += 30;
            }
            else {
                commonCount[movieIndex]++;
                totalRevenue[movieIndex] += 15;
            }

            char buyFoodChoice;
            cout << "\nWould you like to purchase any food/drinks? (y/n): ";
            cin >> buyFoodChoice;
            cin.ignore();

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
                    cin >> itemChoice;

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
                        foodPrice = 0;
                    }

                    if (foodPrice > 0) {
                        cout << "How many portions of " << foodName << " would you like?: ";
                        cin >> quantity;
                        if (quantity > 0) {
                            totalRevenue[movieIndex] += (foodPrice * quantity);
                            cout << quantity << " " << foodName << "(s) added. Cost: $" << (foodPrice * quantity) << ".\n";

                            switch (itemChoice) {
                            case 1: popcornSold[movieIndex] += quantity; break;
                            case 2: sodaSold[movieIndex] += quantity; break;
                            case 3: candySold[movieIndex] += quantity; break;
                            }
                        }
                        else {
                            cout << "Invalid quantity. No food added for this selection.\n";
                        }
                    }

                    cout << "\nWould you like to add more food/drinks? (y/n): ";
                    cin >> addMoreFood;
                    cin.ignore();
                }
            }
        }
        else
        {
            clearScreen();
            printLogo();
            cout << "Seat is already taken!\n";
        }
    }
    else
    {
        clearScreen();
        printLogo();
        cout << "Invalid seat coordinates!\n";
    }

    cout << "\nPress ENTER to continue...";
    cin.get();
}

void showRevenue(int movieIndex)
{
    clearScreen(); // Добавено, за да изчисти екрана преди да покаже доклада
    printLogo();   // Добавено, за да покаже логото

    const int CONSOLE_WIDTH = 80;
    string reportTitle = "============ REVENUE REPORT ============";
    int titlePadding = max(0, (CONSOLE_WIDTH - (int)reportTitle.length()) / 2);

    cout << "\n";
    cout << string(titlePadding, ' ') << reportTitle << "\n";
    cout << "\n";

    // Центриране на информацията за филма
    string movieInfo = "Movie: " + string(movieTitles[movieIndex]) + " at " + string(movieTimes[movieIndex]);
    int movieInfoPadding = max(0, (CONSOLE_WIDTH - (int)movieInfo.length()) / 2);
    cout << string(movieInfoPadding, ' ') << movieInfo << "\n\n";

    // Центриране на категориите за места
    int categoryIndent = 25; // Отстъп, за да съответства на опциите от главното меню
    cout << string(categoryIndent, ' ') << "VIP seats reserved: " << vipCount[movieIndex] << endl;
    cout << string(categoryIndent, ' ') << "Premium seats reserved: " << premiumCount[movieIndex] << endl;
    cout << string(categoryIndent, ' ') << "Common seats reserved: " << commonCount[movieIndex] << endl;
    cout << string(categoryIndent, ' ') << "\n--- Food and Drinks Sales ---\n";

    if (popcornSold[movieIndex] > 0) {
        string popcornText = "Popcorn: " + to_string(popcornSold[movieIndex]) + " x $10 = $" + to_string(popcornSold[movieIndex] * 10);
        cout << string(categoryIndent, ' ') << popcornText << endl;
    }
    if (sodaSold[movieIndex] > 0) {
        string sodaText = "Soda: " + to_string(sodaSold[movieIndex]) + " x $5 = $" + to_string(sodaSold[movieIndex] * 5);
        cout << string(categoryIndent, ' ') << sodaText << endl;
    }
    if (candySold[movieIndex] > 0) {
        string candyText = "Candy: " + to_string(candySold[movieIndex]) + " x $7 = $" + to_string(candySold[movieIndex] * 7);
        cout << string(categoryIndent, ' ') << candyText << endl;
    }
    if (popcornSold[movieIndex] == 0 && sodaSold[movieIndex] == 0 && candySold[movieIndex] == 0) {
        cout << string(categoryIndent, ' ') << "No food or drinks sold for this movie.\n";
    }

    cout << "\n";
    string totalRevenueText = "Total revenue: $" + to_string(totalRevenue[movieIndex]);
    int totalRevenuePadding = max(0, (CONSOLE_WIDTH - (int)totalRevenueText.length()) / 2);
    cout << string(totalRevenuePadding, ' ') << totalRevenueText << endl;
}