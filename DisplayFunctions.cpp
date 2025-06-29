#include <iostream>
#include <string>
#include <cstdlib>
#include "SeatManager.h"

using namespace std;

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
  _________.__              .__         .__           
 /   _____/|__| ____   ____ |  |   ____ |  | __ ____  
 \_____  \|  |/ ___\ / ___\|  | _/ __ \|  |/ // __ \ 
 /        \  \  \___/ / /_/ >  |_>\  ___/|    <\  ___/ 
/_______  /__|\___  >\___  /|____/ \___  >__|_ \\___  >
        \/        \/     \/            \/     \/    \/ 
                               
                  CINEMA CLUB
    )" << "\n";
}

void displaySeatingMap(int** seats)
{
    const int width = COLS * 4 - 1;

    string horizontal = "+" + string(width, '-') + "+";

    int labelLength = 6;
    int labelPadding = (width - labelLength) / 2;

    string middleRow = "|" + string(labelPadding, ' ') + "SCREEN" + string(width - labelPadding - labelLength, ' ') + "|";

    cout << horizontal << "\n";
    cout << middleRow << "\n";
    cout << horizontal << "\n\n";

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