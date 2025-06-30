#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include <algorithm> // For max()
#include "SeatManager.h"

using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("cls"); // For Windows
#else
    system("clear"); // For Linux/macOS
#endif
}

void printLogo()
{
    const int CONSOLE_WIDTH = 80;

    string logoLines[] = {
        "  _________.__              .__         .__           ",
        " /   _____/|__| ____   ____ |  |   ____ |  | __ ____  ",
        " \\_____  \\|  |/ ___\\ / ___\\|  | _/ __ \\|  |/ // __ \\ ",
        " /        \\  \\  \\___/ / /_/ >  |_>\\  ___/|    <\\  ___/ ",
        "/_______  /__|\\___  >\\___  /|____/ \\___  >__|_ \\\\___  >",
        "        \\/        \\/     \\/            \\/     \\/    \\/ "
    };

    cout << "\n";
    for (const string& line : logoLines) {
        int padding = max(0, (CONSOLE_WIDTH - (int)line.length()) / 2);
        cout << string(padding, ' ') << line << endl;
    }

    string cinemaClubText = "CINEMA CLUB";
    int cinemaClubPadding = max(0, (CONSOLE_WIDTH - (int)cinemaClubText.length()) / 2);
    cout << string(cinemaClubPadding, ' ') << cinemaClubText << "\n";
    cout << "\n";
}

void displaySeatingMap(int** seats)
{
    const int width = COLS * 4 - 1; // Width for the screen line based on COLS

    string horizontal = "+" + string(width, '-') + "+";

    int labelLength = 6; // Length of "SCREEN"
    int labelPadding = (width - labelLength) / 2;

    string middleRow = "|" + string(labelPadding, ' ') + "SCREEN" + string(width - labelPadding - labelLength, ' ') + "|";

    cout << horizontal << "\n";
    cout << middleRow << "\n";
    cout << horizontal << "\n\n";

    // Print the seating map
    for (int row = 1; row <= ROWS; row++)
    {
        for (int col = 1; col <= COLS; col++)
        {
            if (seats[row][col] == 0) // Empty seat
                cout << "[ ] ";
            else // Reserved seat
                cout << "[#] ";
        }
        cout << "\n"; // New line for each row
    }
}