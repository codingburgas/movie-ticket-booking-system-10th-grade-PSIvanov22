#include <iostream>
#include <string>
#include <algorithm>
#include <windows.h> // ADD THIS LINE for SetConsoleOutputCP (Windows only)
#include "SeatManager.h"
#include "DatabaseManager.h"

using namespace std;

int userChoice, movieChoice;
int selectedRow, selectedCol; // DEFINED HERE - NO 'extern'

extern const char* movieTitles[MOVIE_COUNT];
extern const char* movieTimes[MOVIE_COUNT];

void showMovieList()
{
    cout << "\n============ MOVIE SCHEDULE ============\n";
    for (int i = 0; i < MOVIE_COUNT; ++i)
    {
        cout << i + 1 << " - " << movieTitles[i] << " at " << movieTimes[i] << endl;
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8); // ADD THIS LINE to fix strange characters in console

    if (!DBManager::initDB("cinema.db")) {
        cerr << "Failed to initialize database. Exiting." << endl;
        return 1;
    }

    int*** allSeats = new int** [MOVIE_COUNT];
    for (int m = 0; m < MOVIE_COUNT; ++m)
    {
        allSeats[m] = new int* [ROWS + 1];
        for (int i = 0; i <= ROWS; ++i)
        {
            allSeats[m][i] = new int[COLS + 1];
            for (int j = 0; j <= COLS; ++j)
                allSeats[m][i][j] = 0;
        }
        DBManager::loadSeatsForMovie(m, allSeats[m]);
    }

    while (true)
    {
        clearScreen();
        printLogo();
        cout << "\n";

        const int CONSOLE_WIDTH = 80;
        string menuTitle = "============ MAIN MENU ============";
        int menuPadding = max(0, (CONSOLE_WIDTH - (int)menuTitle.length()) / 2);
        cout << string(menuPadding, ' ') << menuTitle << "\n";

        int optionIndent = 25;
        cout << string(optionIndent, ' ') << "0 - Exit Program\n";
        cout << string(optionIndent, ' ') << "1 - Reserve Seat\n";
        cout << string(optionIndent, ' ') << "2 - Show Seating Map\n";
        cout << string(optionIndent, ' ') << "3 - Show Revenue\n";
        cout << string(optionIndent, ' ') << "Select an option: ";
        cin >> userChoice;

        switch (userChoice)
        {
        case 0:
            clearScreen();
            printLogo();
            cout << "Exiting program...\n";

            // Clean up allocated memory before exiting
            for (int m = 0; m < MOVIE_COUNT; ++m) {
                for (int i = 0; i <= ROWS; ++i) {
                    delete[] allSeats[m][i];
                }
                delete[] allSeats[m];
            }
            delete[] allSeats;

            DBManager::closeDB();
            return 0;

        case 1:
            clearScreen();
            printLogo();
            showMovieList();
            cout << "\nSelect movie number: ";
            cin >> movieChoice;
            if (movieChoice >= 1 && movieChoice <= MOVIE_COUNT)
                reserveSeat(allSeats, movieChoice - 1, selectedRow, selectedCol);
            else {
                cout << "Invalid movie choice.\n";
                cin.ignore(); // Clear the invalid input
                cin.get();    // Wait for user to press enter
            }
            break;
        case 2:
            clearScreen();
            printLogo();
            showMovieList();
            cout << "\nSelect movie number to view seats: ";
            cin >> movieChoice;
            if (movieChoice >= 1 && movieChoice <= MOVIE_COUNT) {
                clearScreen();
                printLogo();
                cout << "Movie: " << movieTitles[movieChoice - 1] << " at " << movieTimes[movieChoice - 1] << "\n";
                displaySeatingMap(allSeats[movieChoice - 1]);
            }
            else {
                cout << "Invalid movie choice.\n";
            }
            cout << "\nPress ENTER to return to menu...";
            cin.ignore();
            cin.get();
            break;

        case 3:
            clearScreen();
            printLogo();
            showMovieList();
            cout << "\nSelect movie number to view revenue: ";
            cin >> movieChoice;
            if (movieChoice >= 1 && movieChoice <= MOVIE_COUNT) {
                clearScreen();
                printLogo();
                showRevenue(movieChoice - 1);
            }
            else {
                cout << "Invalid movie choice.\n";
            }
            cout << "\nPress ENTER to return to menu...";
            cin.ignore();
            cin.get();
            break;

        default:
            clearScreen();
            printLogo();
            cout << "Invalid option. Please try again.\n";
            cout << "\nPress ENTER to return to menu...";
            cin.ignore();
            cin.get();
        }
    }
}