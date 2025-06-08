#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

#define ROWS 15
#define COLS 40
#define FILMS 3

struct Film {
    string name;
    int vipCount;
    int premiumCount;
    int commonCount;
    int totalRevenue;
    int** seats;
};

int userChoice;
int selectedFilm;
int selectedRow;
int selectedCol;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displaySeatingMap(int** seats)
{
    cout << "   ";
    for (int c = 1; c <= COLS; c++) {
        if (c < 10) cout << c << "  ";
        else cout << c << " ";
    }
    cout << "\n";

    for (int row = 1; row <= ROWS; row++)
    {
        if (row < 10) cout << row << "  ";
        else cout << row << " ";

        for (int col = 1; col <= COLS; col++)
        {
            cout << setw(1) << (seats[row][col] == 0 ? "." : "#") << "  ";
        }
        cout << "\n";
    }
}

void reserveSeat(Film& film)
{
    displaySeatingMap(film.seats);

    cout << "\n============PRICES===========" << endl;
    cout << "Rows 1 to 5: $50.00\nRows 6 to 10: $30.00\nRows 11 to 15: $15.00\n";

    cout << "Which row would you like to reserve? (1-" << ROWS << "): ";
    cin >> selectedRow;
    if (cin.fail() || selectedRow < 1 || selectedRow > ROWS) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid row number!\n";
        return;
    }

    cout << "Which column would you like to reserve? (1-" << COLS << "): ";
    cin >> selectedCol;
    if (cin.fail() || selectedCol < 1 || selectedCol > COLS) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid column number!\n";
        return;
    }

    if (film.seats[selectedRow][selectedCol] == 0)
    {
        film.seats[selectedRow][selectedCol] = 1;
        clearScreen();
        cout << "============RESERVATION===========" << endl;
        cout << "Seat successfully reserved!\n\n";

        if (selectedRow >= 1 && selectedRow <= 5)
        {
            film.vipCount++;
            film.totalRevenue += 50;
        }
        else if (selectedRow >= 6 && selectedRow <= 10)
        {
            film.premiumCount++;
            film.totalRevenue += 30;
        }
        else
        {
            film.commonCount++;
            film.totalRevenue += 15;
        }
    }
    else
    {
        clearScreen();
        cout << "============RESERVATION===========" << endl;
        cout << "Seat unavailable!\n\n";
    }
}

void showRevenueReport(Film films[], int count)
{
    clearScreen();
    cout << "============REVENUE REPORT===========" << endl;
    int grandTotalRevenue = 0;
    int grandVip = 0, grandPremium = 0, grandCommon = 0;

    for (int i = 0; i < count; i++)
    {
        cout << "Film: " << films[i].name << endl;
        cout << "  VIP seats reserved: " << films[i].vipCount << endl;
        cout << "  Premium seats reserved: " << films[i].premiumCount << endl;
        cout << "  Common seats reserved: " << films[i].commonCount << endl;
        cout << "  Total revenue: $" << films[i].totalRevenue << "\n\n";

        grandTotalRevenue += films[i].totalRevenue;
        grandVip += films[i].vipCount;
        grandPremium += films[i].premiumCount;
        grandCommon += films[i].commonCount;
    }

    cout << "Overall totals across all films:\n";
    cout << "  VIP seats reserved: " << grandVip << endl;
    cout << "  Premium seats reserved: " << grandPremium << endl;
    cout << "  Common seats reserved: " << grandCommon << endl;
    cout << "  Total revenue: $" << grandTotalRevenue << endl;
}

bool selectFilm(Film films[], int count)
{
    cout << "Select a film to reserve seats for:\n";
    for (int i = 0; i < count; i++)
    {
        cout << i + 1 << " - " << films[i].name << endl;
    }
    cout << "Enter choice: ";
    cin >> selectedFilm;

    if (cin.fail() || selectedFilm < 1 || selectedFilm > count) {
        cin.clear();
        cin.ignore(10000, '\n');
        clearScreen();
        cout << "Invalid film selection!\n";
        return false;
    }
    return true;
}

int main()
{
    Film films[FILMS] = {
        {"Avengers: Endgame", 0, 0, 0, 0, nullptr},
        {"Inception", 0, 0, 0, 0, nullptr},
        {"The Lion King", 0, 0, 0, 0, nullptr}
    };

    for (int f = 0; f < FILMS; f++)
    {
        films[f].seats = new int* [ROWS + 1];
        for (int i = 0; i <= ROWS; i++)
        {
            films[f].seats[i] = new int[COLS + 1];
        }
        for (int i = 0; i <= ROWS; i++)
            for (int j = 0; j <= COLS; j++)
                films[f].seats[i][j] = 0;
    }

    while (true)
    {
        cout << "\n============MENU===========" << endl;
        cout << "0 - Exit Program\n1 - Reserve Seat\n2 - Show Seating Map\n3 - Show Revenue\n";
        cout << "Select an option: ";
        cin >> userChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            clearScreen();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (userChoice)
        {
        case 0:
            clearScreen();
            for (int f = 0; f < FILMS; f++)
            {
                for (int i = 0; i <= ROWS; i++)
                    delete[] films[f].seats[i];
                delete[] films[f].seats;
            }
            return 0;

        case 1:
            clearScreen();
            if (selectFilm(films, FILMS))
            {
                clearScreen();
                cout << "============RESERVATION FOR " << films[selectedFilm - 1].name << "===========" << endl;
                reserveSeat(films[selectedFilm - 1]);
            }
            break;

        case 2:
            clearScreen();
            if (selectFilm(films, FILMS))
            {
                clearScreen();
                cout << "============SEATING MAP FOR " << films[selectedFilm - 1].name << "============" << endl;
                displaySeatingMap(films[selectedFilm - 1].seats);
            }
            break;

        case 3:
            showRevenueReport(films, FILMS);
            break;

        default:
            clearScreen();
            cout << "Invalid option. Please try again.\n";
        }
    }
}
