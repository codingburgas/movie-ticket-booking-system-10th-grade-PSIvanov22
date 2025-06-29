#pragma once
#ifndef SEAT_MANAGER_H
#define SEAT_MANAGER_H

#define ROWS 15
#define COLS 40
#define MOVIE_COUNT 8

extern int vipCount[MOVIE_COUNT];
extern int premiumCount[MOVIE_COUNT];
extern int commonCount[MOVIE_COUNT];
extern int totalRevenue[MOVIE_COUNT];

extern const char* movieTitles[MOVIE_COUNT];
extern const char* movieTimes[MOVIE_COUNT];

void clearScreen();
void printLogo();
void displaySeatingMap(int** seats);
void reserveSeat(int*** allSeats, int movieIndex);
void showRevenue(int movieIndex);

#endif
