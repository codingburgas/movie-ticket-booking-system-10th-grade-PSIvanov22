#include <iostream>
#include <iomanip>
#include "SeatManager.h"

using namespace std;

int vipCount[MOVIE_COUNT] = {};
int premiumCount[MOVIE_COUNT] = {};
int commonCount[MOVIE_COUNT] = {};
int totalRevenue[MOVIE_COUNT] = {};

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
    cout << "Rows 1 to 5: $50.00\nRows 6 to 10: $30.00\nRows 11 to 15: $15.00";
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
    cin.ignore();
    cin.get();
}

void showRevenue(int movieIndex)
{
    cout << "============REVENUE REPORT===========" << endl;
    cout << "Movie: " << movieTitles[movieIndex] << " at " << movieTimes[movieIndex] << "\n\n";
    cout << "VIP seats reserved: " << vipCount[movieIndex] << endl;
    cout << "Premium seats reserved: " << premiumCount[movieIndex] << endl;
    cout << "Common seats reserved: " << commonCount[movieIndex] << endl;
    cout << "\nTotal revenue: $" << totalRevenue[movieIndex] << endl;
}