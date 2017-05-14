#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>

/* ID for recognising */
#define ID_PRZYCISKA 501
#define ID_PRZYCISKB 502
#define ID_PRZYCISKC 503
#define ID_PRZYCISKD 504
#define ID_PRZYCISKSTART 505
#define ID_PRZYCISKPOMOC 506
using namespace std;

///////////////////////////////////////////////////////
// Tutaj zmienic te dwa define przed kompilacja
#define SCIEZKA_DO_PLIKU "pytania.txt"
#define SCIEZKA_DO_WYNIKU "wynik.txt"
///////////////////////////////////////////////////////

/* Global variables */
HWND g_hButtonA;
HWND g_hButtonB;
HWND g_hButtonC;
HWND g_hButtonD;
HWND g_hButtonStart;
HWND g_hButtonPomoc;
HWND g_hTextNazwisko;

HWND g_hStaticPytanie;
HWND g_hStaticNazwisko;
HWND g_hStaticA;
HWND g_hStaticB;
HWND g_hStaticC;
HWND g_hStaticD;
HWND g_hStaticNumerPytania;
HWND g_hStaticNrPytTitle;

LPSTR Bufor1;

char poprODP;
int punkty = -1;
string charpktPYT;
int pktPYT = 0;

int liczba_pytan;
int wylosowane_pytanie = 0;
string pytanko;

vector <char> v_poprODP;
vector <char> v_uzyskODP;
vector <int> v_punktyzaPYT;

template <typename T>
std::string ToString(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void policz_pytania();
void losuj_pytanie();
void zapisz_wynik();
void zakoncz_program();

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("Sprawdzian z systemow liczb");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(RGB(49, 187, 246));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("QUIZ"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    /*Preparation before showing window */
    g_hButtonA = CreateWindowEx( WS_EX_CLIENTEDGE, "BUTTON", "A", WS_CHILD | WS_VISIBLE,
                 100, 400, 100, 100, hwnd,( HMENU ) ID_PRZYCISKA, hThisInstance, NULL );

    g_hButtonB = CreateWindowEx( WS_EX_CLIENTEDGE, "BUTTON", "B", WS_CHILD | WS_VISIBLE,
                 260, 400, 100, 100, hwnd,( HMENU ) ID_PRZYCISKB, hThisInstance, NULL );

    g_hButtonC = CreateWindowEx( WS_EX_CLIENTEDGE, "BUTTON", "C", WS_CHILD | WS_VISIBLE,
                 440, 400, 100, 100, hwnd,( HMENU ) ID_PRZYCISKC, hThisInstance, NULL );

    g_hButtonD = CreateWindowEx( WS_EX_CLIENTEDGE, "BUTTON", "D", WS_CHILD | WS_VISIBLE,
                 600, 400, 100, 100, hwnd,( HMENU ) ID_PRZYCISKD, hThisInstance, NULL );


    g_hButtonStart = CreateWindowEx( WS_EX_CLIENTEDGE, "BUTTON", "Start", WS_CHILD | WS_VISIBLE,
                 700, 100, 80, 150, hwnd,( HMENU ) ID_PRZYCISKSTART, hThisInstance, NULL );

    g_hButtonPomoc = CreateWindowEx( WS_EX_CLIENTEDGE, "BUTTON", "Pomoc", WS_CHILD | WS_VISIBLE,
                 20, 180, 80, 150, hwnd,( HMENU ) ID_PRZYCISKPOMOC, hThisInstance, NULL );

    g_hTextNazwisko = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
    150, 350, 300, 30, hwnd, NULL, hThisInstance, NULL );


    g_hStaticPytanie = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
        SS_CENTER, 150, 80, 500, 150, hwnd, NULL, hThisInstance, NULL );
    g_hStaticNazwisko = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
        SS_CENTER, 450, 350, 200, 30, hwnd, NULL, hThisInstance, NULL );
    SetWindowText(g_hStaticNazwisko, "Podpisz sie tutaj");
    g_hStaticA = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
        SS_LEFT, 150, 130, 500, 50, hwnd, NULL, hThisInstance, NULL );
    g_hStaticB = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
        SS_LEFT, 150, 180, 500, 50, hwnd, NULL, hThisInstance, NULL );
    g_hStaticC = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
        SS_LEFT, 150, 230, 500, 50, hwnd, NULL, hThisInstance, NULL );
    g_hStaticD = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
        SS_LEFT, 150, 280, 500, 50, hwnd, NULL, hThisInstance, NULL );

    g_hStaticNrPytTitle = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
        SS_CENTER, 30, 20, 60, 60, hwnd, NULL, hThisInstance, NULL );
        SetWindowText(g_hStaticNrPytTitle, "Numer Pytania");
    g_hStaticNumerPytania = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
        SS_CENTER, 30, 70, 60, 60, hwnd, NULL, hThisInstance, NULL );

    HWND hStaticTitle = CreateWindowEx( 0, "STATIC", NULL, WS_CHILD | WS_VISIBLE |
    SS_CENTER, 150, 20, 500, 40, hwnd, NULL, hThisInstance, NULL );
    SetWindowText(hStaticTitle, "QUIZ");

    HFONT hFont = CreateFont (32, 0, 0, 0, 700, FALSE, TRUE, FALSE, ANSI_CHARSET,
    OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
    SendMessage(hStaticTitle, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));

    /* Make the window visible on the screen */
    FreeConsole();
    ShowWindow (hwnd, nCmdShow);
    ShowWindow(g_hStaticNrPytTitle, SW_HIDE);
    ShowWindow(g_hStaticNumerPytania, SW_HIDE);
    ShowWindow(g_hButtonA, SW_HIDE);
    ShowWindow(g_hButtonB, SW_HIDE);
    ShowWindow(g_hButtonC, SW_HIDE);
    ShowWindow(g_hButtonD, SW_HIDE);
    MessageBox( NULL, "Witam w Quizie!.\nAby rozpoczac, kliknij przycisk \"Start\".\nWyswietli sie wtedy pytanie, aby odpowiedziec kliknij jeden z przyciskow A B C D.\n\nProgram by Patryk Mikulski\nAll rights reserved.", "Help", MB_ICONEXCLAMATION );

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_COMMAND:
            if(( HWND ) lParam == g_hButtonA || ( HWND ) lParam == g_hButtonB ||
               ( HWND ) lParam == g_hButtonC || ( HWND ) lParam == g_hButtonD ||
               ( HWND ) lParam == g_hButtonStart || ( HWND ) lParam == g_hButtonPomoc)
            {
                switch( wParam )
                {
                    case ID_PRZYCISKA:
                        if (poprODP == 'A')
                        {
                            //MessageBox( hwnd, "Poprawna odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
                            pktPYT = atoi(charpktPYT.c_str());
                            punkty += pktPYT;
                        }
                        //else MessageBox( hwnd, "Zla odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
                        v_uzyskODP.push_back('A');
                        if (wylosowane_pytanie == liczba_pytan) zakoncz_program();
                        else losuj_pytanie();
                        pytanko = ToString(wylosowane_pytanie);
                        SetWindowText(g_hStaticNumerPytania, pytanko.c_str());
                    break;
                    case ID_PRZYCISKB:
                        if (poprODP == 'B')
                        {
                            //MessageBox( hwnd, "Poprawna odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
                            pktPYT = atoi(charpktPYT.c_str());
                            punkty += pktPYT;
                        }
                        // else MessageBox( hwnd, "Zla odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
                        v_uzyskODP.push_back('B');
                        if (wylosowane_pytanie == liczba_pytan) zakoncz_program();
                        else losuj_pytanie();
                        pytanko = ToString(wylosowane_pytanie);
                        SetWindowText(g_hStaticNumerPytania, pytanko.c_str());
                    break;
                    case ID_PRZYCISKC:
                        if (poprODP == 'C')
                        {
                            //MessageBox( hwnd, "Poprawna odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
                            pktPYT = atoi(charpktPYT.c_str());
                            punkty += pktPYT;
                        }
                        //else MessageBox( hwnd, "Zla odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
                        v_uzyskODP.push_back('C');
                        if (wylosowane_pytanie == liczba_pytan) zakoncz_program();
                        else losuj_pytanie();
                        pytanko = ToString(wylosowane_pytanie);
                        SetWindowText(g_hStaticNumerPytania, pytanko.c_str());
                    break;
                    case ID_PRZYCISKD:
                        if (poprODP == 'D')
                        {
                           // MessageBox( hwnd, "Poprawna odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
                            pktPYT = atoi(charpktPYT.c_str());
                            punkty += pktPYT;
                        }
                        //else MessageBox( hwnd, "Zla odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
                        v_uzyskODP.push_back('D');
                        if (wylosowane_pytanie == liczba_pytan) zakoncz_program();
                        else losuj_pytanie();
                        pytanko = ToString(wylosowane_pytanie);
                        SetWindowText(g_hStaticNumerPytania, pytanko.c_str());
                    break;

                    case ID_PRZYCISKSTART:
                        {
                            if (punkty == -1)
                            {
                                SetWindowText(g_hButtonStart, "Powodzenia");
                                policz_pytania();
                                losuj_pytanie();
                                punkty++;
                                ShowWindow(g_hButtonA, SW_SHOW);
                                ShowWindow(g_hButtonB, SW_SHOW);
                                ShowWindow(g_hButtonC, SW_SHOW);
                                ShowWindow(g_hButtonD, SW_SHOW);
                                ShowWindow(g_hStaticNumerPytania, SW_SHOW);
                                ShowWindow(g_hStaticNrPytTitle, SW_SHOW);
                                pytanko = ToString(wylosowane_pytanie);
                                SetWindowText(g_hStaticNumerPytania, pytanko.c_str());
                            }
                            else
                            {
                                //zakoncz_program();
                            }
                        }
                    break;
                    case ID_PRZYCISKPOMOC:
                        MessageBox( NULL, "Aby rozpoczac, kliknij przycisk \"Start\".\nWyswietli sie wtedy pytanie, aby odpowiedziec kliknij jeden z przyciskow A B C D.\n\nProgram by Patryk Mikulski\nAll rights reserved.", "Help", MB_ICONEXCLAMATION );
                    break;
                }
            }
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void policz_pytania()
{
    string line;
    fstream in_file;
    in_file.open(SCIEZKA_DO_PLIKU, ios::in);
    int nr_linii = 0;
    while (getline(in_file, line)) nr_linii++;
    in_file.close();
    liczba_pytan = nr_linii / 6;
}

void losuj_pytanie()
{
    //srand(time(NULL));
    string line;
    string pytanie, odpA, odpB, odpC, odpD;
    fstream in_file;
    in_file.open(SCIEZKA_DO_PLIKU, ios::in);
    //int wylosowane_pytanie = (rand() % liczba_pytan-1) + 1;
    int nr_linii = 0;
    wylosowane_pytanie++;
    while (getline(in_file, line))
    {
        nr_linii++;
        if (nr_linii == (wylosowane_pytanie-1)*6 + 1) pytanie = line;
        if (nr_linii == (wylosowane_pytanie-1)*6 + 2) odpA = line;
        if (nr_linii == (wylosowane_pytanie-1)*6 + 3) odpB = line;
        if (nr_linii == (wylosowane_pytanie-1)*6 + 4) odpC = line;
        if (nr_linii == (wylosowane_pytanie-1)*6 + 5) odpD = line;
        if (nr_linii == (wylosowane_pytanie-1)*6 + 6)
        {
            poprODP = line[0];
            v_poprODP.push_back(poprODP);
            charpktPYT = line[1];
            pktPYT = atoi(charpktPYT.c_str());
            v_punktyzaPYT.push_back(pktPYT);
        }
    }
    in_file.close();
    odpA = "Odp A:  " + odpA;
    odpB = "Odp B:  " + odpB;
    odpC = "Odp C:  " + odpC;
    odpD = "Odp D:  " + odpD;
    SetWindowText(g_hStaticPytanie, pytanie.c_str());
    SetWindowText(g_hStaticA, odpA.c_str());
    SetWindowText(g_hStaticB, odpB.c_str());
    SetWindowText(g_hStaticC, odpC.c_str());
    SetWindowText(g_hStaticD, odpD.c_str());
}

void zapisz_wynik()
{
    fstream out_file;
    out_file.open(SCIEZKA_DO_WYNIKU, ios::out);
    out_file << "Imie i Nazwisko:\t" << Bufor1 << endl;
    out_file << "Numer pytania\t\t";
    if (liczba_pytan > 10)
    {
        for (int i = 1; i <= 10; i++) out_file << " " << i << " ";
        for (int i = 11; i <= liczba_pytan; i++) out_file << i << " ";
    }
    else
    {
        for (int i = 1; i <= liczba_pytan; i++) out_file << " " << i << " ";
    }
    out_file << endl;
    out_file << "Poprawna ODP\t\t";
    for (const auto &e : v_poprODP) out_file << " " << e << " ";
    out_file << endl;
    out_file << "Uzyskana ODP\t\t";
    for (const auto &f : v_uzyskODP) out_file << " " << f << " ";
    out_file << endl;
    out_file << "Punkty do zdobycia:\t";
    for (const auto &g: v_punktyzaPYT) out_file << " " << g << " ";
    out_file << endl << endl;
    out_file << "Ilosc zdobytych punktow: " << punkty;
    out_file.close();
    ShowWindow(g_hButtonA, SW_HIDE);
    ShowWindow(g_hButtonB, SW_HIDE);
    ShowWindow(g_hButtonC, SW_HIDE);
    ShowWindow(g_hButtonD, SW_HIDE);
}

void zakoncz_program()
{
    string pkt;
    pkt = ToString(punkty);
    string wiadomosc = "Koniec, zdobyles/as " + pkt + " punkty/ow!";

    DWORD dwRozmiar;
    dwRozmiar = GetWindowTextLength( g_hTextNazwisko ) + 1;
    Bufor1 =( LPSTR ) GlobalAlloc( GPTR, dwRozmiar + 1 );
    GetWindowText( g_hTextNazwisko, Bufor1, dwRozmiar );

    zapisz_wynik();
    MessageBox(NULL, wiadomosc.c_str(), "Zakonczenie", MB_ICONEXCLAMATION);
    PostQuitMessage (0);
}
