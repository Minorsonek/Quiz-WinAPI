#include <tchar.h>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>

#include "UIElements.h"
#include "QuestionHandler.h"
using namespace std;

///////////////////////////////////////////////////////
// Tutaj zmienic te dwa define przed kompilacja
#define SCIEZKA_DO_PLIKU "pytania.txt"
#define SCIEZKA_DO_WYNIKU "wynik.txt"
///////////////////////////////////////////////////////

/* Global variables */
UIElements* UI;

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
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void policz_pytania();
void losuj_pytanie();
void zapisz_wynik();
void QuitApp();

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("Sprawdzian z systemow liczb");

int WINAPI WinMain(HINSTANCE hThisInstance,
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
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
	/* Use Windows's default colour as the background of the window */
	wincl.hbrBackground = CreateSolidBrush(RGB(49, 187, 246));

	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx(&wincl))
		return 0;

	/* The class is registered, let's create the program*/
	hwnd = CreateWindowEx(
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

	/* Preparation before showing window */
	UI = &UIElements::GetInstance(hwnd, hThisInstance);

	SetWindowTextA((*UI).hStaticUserName, LPSTR("Podpisz sie tutaj"));
	SetWindowTextA((*UI).hStaticQuestionNumberTitle, LPSTR("Numer Pytania"));

	HWND hStaticTitle = CreateWindowExA(0, LPSTR("STATIC"), NULL, WS_CHILD | WS_VISIBLE |
		SS_CENTER, 150, 20, 500, 40, hwnd, NULL, hThisInstance, NULL);
	SetWindowTextA(hStaticTitle, LPSTR("QUIZ"));

	HFONT hFont = CreateFont(32, 0, 0, 0, 700, FALSE, TRUE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
	SendMessage(hStaticTitle, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));

	/* Make the window visible on the screen */
	FreeConsole();
	ShowWindow(hwnd, nCmdShow);
	ShowWindow((*UI).hStaticQuestionNumberTitle, SW_HIDE);
	ShowWindow((*UI).hStaticQuestionNumber, SW_HIDE);
	ShowWindow((*UI).hButtonA, SW_HIDE);
	ShowWindow((*UI).hButtonB, SW_HIDE);
	ShowWindow((*UI).hButtonC, SW_HIDE);
	ShowWindow((*UI).hButtonD, SW_HIDE);
	MessageBoxA(NULL, LPSTR("Witam w Quizie!.\nAby rozpoczac, kliknij przycisk \"Start\".\nWyswietli sie wtedy pytanie, aby odpowiedziec kliknij jeden z przyciskow A B C D.\n\nProgram by Patryk Mikulski\nAll rights reserved."), LPSTR("Help"), MB_ICONEXCLAMATION);

	/* Run the message loop. It will run until GetMessage() returns 0 */
	while (GetMessage(&messages, NULL, 0, 0))
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

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)                  /* handle the messages */
	{
	case WM_DESTROY:
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		break;
	case WM_COMMAND:
		HWND lParamAsHWND = (HWND)lParam;
		if (lParamAsHWND == (*UI).hButtonA || lParamAsHWND == (*UI).hButtonB ||
			lParamAsHWND == (*UI).hButtonC || lParamAsHWND == (*UI).hButtonD ||
			lParamAsHWND == (*UI).hButtonStart || lParamAsHWND == (*UI).hButtonHelp)
		{
			switch (wParam)
			{
			case ID_BUTTONA:
				if (poprODP == 'A')
				{
					//MessageBoxA( hwnd, "Poprawna odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
					pktPYT = atoi(charpktPYT.c_str());
					punkty += pktPYT;
				}
				//else MessageBoxA( hwnd, "Zla odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
				v_uzyskODP.push_back('A');
				if (wylosowane_pytanie == liczba_pytan) QuitApp();
				else losuj_pytanie();
				pytanko = ToString(wylosowane_pytanie);
				SetWindowTextA((*UI).hStaticQuestionNumber, LPSTR(pytanko.c_str()));
				break;
			case ID_BUTTONB:
				if (poprODP == 'B')
				{
					//MessageBoxA( hwnd, "Poprawna odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
					pktPYT = atoi(charpktPYT.c_str());
					punkty += pktPYT;
				}
				// else MessageBoxA( hwnd, "Zla odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
				v_uzyskODP.push_back('B');
				if (wylosowane_pytanie == liczba_pytan) QuitApp();
				else losuj_pytanie();
				pytanko = ToString(wylosowane_pytanie);
				SetWindowTextA((*UI).hStaticQuestionNumber, LPSTR(pytanko.c_str()));
				break;
			case ID_BUTTONC:
				if (poprODP == 'C')
				{
					//MessageBoxA( hwnd, "Poprawna odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
					pktPYT = atoi(charpktPYT.c_str());
					punkty += pktPYT;
				}
				//else MessageBoxA( hwnd, "Zla odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
				v_uzyskODP.push_back('C');
				if (wylosowane_pytanie == liczba_pytan) QuitApp();
				else losuj_pytanie();
				pytanko = ToString(wylosowane_pytanie);
				SetWindowTextA((*UI).hStaticQuestionNumber, LPSTR(pytanko.c_str()));
				break;
			case ID_BUTTOND:
				if (poprODP == 'D')
				{
					// MessageBoxA( hwnd, "Poprawna odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
					pktPYT = atoi(charpktPYT.c_str());
					punkty += pktPYT;
				}
				//else MessageBoxA( hwnd, "Zla odpowiedz", "Odpowiedz", MB_ICONINFORMATION );
				v_uzyskODP.push_back('D');
				if (wylosowane_pytanie == liczba_pytan) QuitApp();
				else losuj_pytanie();
				pytanko = ToString(wylosowane_pytanie);
				SetWindowTextA((*UI).hStaticQuestionNumber, LPSTR(pytanko.c_str()));
				break;

			case ID_BUTTONSTART:
			{
				if (punkty == -1)
				{
					SetWindowTextA((*UI).hButtonStart, LPSTR("Powodzenia"));
					policz_pytania();
					losuj_pytanie();
					punkty++;
					ShowWindow((*UI).hButtonA, SW_SHOW);
					ShowWindow((*UI).hButtonB, SW_SHOW);
					ShowWindow((*UI).hButtonC, SW_SHOW);
					ShowWindow((*UI).hButtonD, SW_SHOW);
					ShowWindow((*UI).hStaticQuestionNumber, SW_SHOW);
					ShowWindow((*UI).hStaticQuestionNumberTitle, SW_SHOW);
					pytanko = ToString(wylosowane_pytanie);
					SetWindowTextA((*UI).hStaticQuestionNumber, LPSTR(pytanko.c_str()));
				}
				else
				{
					//QuitApp();
				}
			}
			break;
			case ID_BUTTONHELP:
				MessageBoxA(NULL, LPSTR("Aby rozpoczac, kliknij przycisk \"Start\".\nWyswietli sie wtedy pytanie, aby odpowiedziec kliknij jeden z przyciskow A B C D.\n\nProgram by Patryk Mikulski\nAll rights reserved."), LPSTR("Help"), MB_ICONEXCLAMATION);
				break;
			}
		}
	default:                      /* for messages that we don't deal with */
		return DefWindowProc(hwnd, message, wParam, lParam);
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
		if (nr_linii == (wylosowane_pytanie - 1) * 6 + 1) pytanie = line;
		if (nr_linii == (wylosowane_pytanie - 1) * 6 + 2) odpA = line;
		if (nr_linii == (wylosowane_pytanie - 1) * 6 + 3) odpB = line;
		if (nr_linii == (wylosowane_pytanie - 1) * 6 + 4) odpC = line;
		if (nr_linii == (wylosowane_pytanie - 1) * 6 + 5) odpD = line;
		if (nr_linii == (wylosowane_pytanie - 1) * 6 + 6)
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
	SetWindowTextA((*UI).hStaticQuestion, LPSTR(pytanie.c_str()));
	SetWindowTextA((*UI).hStaticA, LPSTR(odpA.c_str()));
	SetWindowTextA((*UI).hStaticB, LPSTR(odpB.c_str()));
	SetWindowTextA((*UI).hStaticC, LPSTR(odpC.c_str()));
	SetWindowTextA((*UI).hStaticD, LPSTR(odpD.c_str()));
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
	for (const auto& e : v_poprODP) out_file << " " << e << " ";
	out_file << endl;
	out_file << "Uzyskana ODP\t\t";
	for (const auto& f : v_uzyskODP) out_file << " " << f << " ";
	out_file << endl;
	out_file << "Punkty do zdobycia:\t";
	for (const auto& g : v_punktyzaPYT) out_file << " " << g << " ";
	out_file << endl << endl;
	out_file << "Ilosc zdobytych punktow: " << punkty;
	out_file.close();
	ShowWindow((*UI).hButtonA, SW_HIDE);
	ShowWindow((*UI).hButtonB, SW_HIDE);
	ShowWindow((*UI).hButtonC, SW_HIDE);
	ShowWindow((*UI).hButtonD, SW_HIDE);
}

void QuitApp()
{
	string pkt;
	pkt = ToString(punkty);
	string wiadomosc = "Koniec, zdobyles/as " + pkt + " punkty/ow!";

	DWORD dwRozmiar;
	dwRozmiar = GetWindowTextLength((*UI).hTextUserName) + 1;
	Bufor1 = (LPSTR)GlobalAlloc(GPTR, dwRozmiar + 1);
	GetWindowTextA((*UI).hTextUserName, Bufor1, dwRozmiar);

	zapisz_wynik();
	MessageBoxA(NULL, LPSTR(wiadomosc.c_str()), LPSTR("Zakonczenie"), MB_ICONEXCLAMATION);
	PostQuitMessage(0);
}