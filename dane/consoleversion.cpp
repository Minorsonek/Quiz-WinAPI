#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;
int liczba_pytan = 19; //19 pytan
int main()
{
    srand(time(NULL));
    cout << "Witaj w Quizie wiedzy programistycznej, losujesz pytania i wpisujesz literke odpowiedzi odpowiednio : A, B, C, D\nPowodzenia!\n\n";
    string line;
    int punkty = 0;
    while(1)
    {
    /////////////
    string pytanie, odpA, odpB, odpC, odpD;
    char poprawna, odpowiedz;
    /////////////
    fstream in_file;
    in_file.open("pytania.txt", ios::in);
    if (in_file.good())
    {
        int wylosowane_pytanie =(rand() % liczba_pytan-1) + 1;
        int nr_linii = 0;
        while (getline(in_file, line))
        {
            nr_linii++;
            if (nr_linii == (wylosowane_pytanie-1)*6 + 1) pytanie = line;
            if (nr_linii == (wylosowane_pytanie-1)*6 + 2) odpA = line;
            if (nr_linii == (wylosowane_pytanie-1)*6 + 3) odpB = line;
            if (nr_linii == (wylosowane_pytanie-1)*6 + 4) odpC = line;
            if (nr_linii == (wylosowane_pytanie-1)*6 + 5) odpD = line;
            if (nr_linii == (wylosowane_pytanie-1)*6 + 6) poprawna = line[0];
        }
        cout << pytanie << endl << "A: " << odpA << endl << "B: " << odpB << endl << "C: " << odpC << endl << "D: " << odpD << endl;
        cin >> odpowiedz;
        if (odpowiedz != poprawna)
        {
            cout << "Bledna odpowiedz" << endl
                 << "Udalo Ci sie zdobyc " << punkty << " punktow";
            break;
        }
        cout << "Brawo, poprawna odpowiedz." << endl;
        punkty++;
    }
    else
        cout << "Brak dostepu do pliku z danymi";
    }
}
