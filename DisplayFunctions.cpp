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
   _____ _                            _       _       
  / ____| |                          | |     | |      
 | |  
  | |__   ___  _ __   ___ _ __ | |_ ___| |__    
 | |    | '_ \ / _ \| '_ \ / _ \ '_ \| __/ __| '_ \   
 | |____| | | | (_) | | | |  __/ | | | || (__| | | |  
  \_____|_| |_|\___|_| |_|\___|_| |_|\__\___|_| |_|  
                       
                               
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