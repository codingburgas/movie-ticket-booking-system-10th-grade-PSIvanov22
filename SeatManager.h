#ifndef SEAT_MANAGER_H
#define SEAT_MANAGER_H

#define ROWS 10
#define COLS 20
#define MOVIE_COUNT 8

extern const char* movieTitles[MOVIE_COUNT];
extern const char* movieTimes[MOVIE_COUNT];

void clearScreen();
void printLogo();
void displaySeatingMap(int** seats);
void reserveSeat(int*** allSeats, int movieIndex, int& row, int& col);
void showRevenue(int movieIndex);

#endif