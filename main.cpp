#include <iostream>
#include "SeatManager.h"

using namespace std;

int userChoice, movieChoice;
int selectedRow, selectedCol;

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
    }

    while (true)
    {
        clearScreen();
        printLogo();
        cout << "\n============ MAIN MENU ============\n";
        cout << "0 - Exit Program\n1 - Reserve Seat\n2 - Show Seating Map\n3 - Show Revenue\n";
        cout << "Select an option: ";
        cin >> userChoice;

        switch (userChoice)
        {
        case 0:
            clearScreen();
            printLogo();
            cout << "Exiting program...\n";
            for (int m = 0; m < MOVIE_COUNT; ++m) {
                for (int i = 0; i <= ROWS; ++i) {
                    delete[] allSeats[m][i];
                }
                delete[] allSeats[m];
            }
            delete[] allSeats;
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
                cin.ignore();
                cin.get();
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