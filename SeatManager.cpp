include <iostream>
#include <iomanip>
#include <cstdlib>
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

int selectedRow, selectedCol;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLogo()
{
    cout << R"(
   _____ _                            _       _       
  / ____| |                          | |     | |      
 | |    | |__   ___  _ __   ___ _ __ | |_ ___| |__    
 | |    | '_ \ / _ \| '_ \ / _ \ '_ \| __/ __| '_ \   
 | |____| | | | (_) | | | |  __/ | | | || (__| | | |  
  \_____|_| |_|\___/|_| |_|\___|_| |_|\__\___|_| |_|  
                                                     
                  CINEMA CLUB
    )" << "\n";
}

void displaySeatingMap(int** seats)
{
    const int width = COLS * 4 - 1;

    // Show screen rectangle
    string horizontal = "+" + string(width, '-') + "+";

    int labelLength = 6; // "SCREEN"
    int labelPadding = (width - labelLength) / 2;

    string middleRow = "|" + string(labelPadding, ' ') + "SCREEN" + string(width - labelPadding - labelLength, ' ') + "|";

    cout << horizontal << "\n";
    cout << middleRow << "\n";
    cout << horizontal << "\n\n";

    // Show seat map
    for (int row = 1; row <= ROWS; row++)
    {
        for (int col = 1; col <= COLS; col++)
        {
            if (seats[row][col] == 0)
                cout << "[ ] ";
            else
                cout << "[#] ";
        }
        cout << "\n";
    }
}



void reserveSeat(int*** allSeats, int movieIndex)
{
    clearScreen();
    printLogo();

    int** seats = allSeats[movieIndex];

    cout << "Movie: " << movieTitles[movieIndex] << " at " << movieTimes[movieIndex] << "\n";
    displaySeatingMap(seats);

    cout << "\n============PRICES===========" << endl;
    cout << "Rows 1 to 5: $50.00\nRows 6 to 10: $30.00\nRows 11 to 15: $15.00";
    cout << "\n\nWhich row would you like to reserve?: ";
    cin >> selectedRow;
    cout << "Which column would you like to reserve?: ";
    cin >> selectedCol;

    if (selectedRow >= 1 && selectedRow <= ROWS &&
        selectedCol >= 1 && selectedCol <= COLS)
    {
        if (seats[selectedRow][selectedCol] == 0)
        {
            seats[selectedRow][selectedCol] = 1;
            clearScreen();
            printLogo();
            cout << "============RESERVATION===========" << endl;
            cout << "\nSeat successfully reserved!\n\n";

            if (selectedRow <= 5) {
                vipCount[movieIndex]++;
                totalRevenue[movieIndex] += 50;
            }
            else if (selectedRow <= 10) {
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
