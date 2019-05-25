#include <tchar.h>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

#include "UIElements.h"
#include "QuestionHandler.h"
using namespace std;

///////////////////////////////////////////////////////
#define QUESTION_FILE_PATH "pytania.txt"
#define RESULTS_FILE_PATH "wynik.txt"
///////////////////////////////////////////////////////

/* Global variables */
UIElements* UI;
QuestionHandler* QuestionH;

LPSTR Buffer1;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void HandleAnswerButtonClick();
void FinishQuiz();

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
	QuestionH = &QuestionHandler::GetInstance(QUESTION_FILE_PATH, RESULTS_FILE_PATH);

	/* Make the window visible on the screen */
	FreeConsole();
	ShowWindow(hwnd, nCmdShow);
	ShowWindow(UI->hStaticQuestionNumberTitle, SW_HIDE);
	ShowWindow(UI->hStaticQuestionNumber, SW_HIDE);
	ShowWindow(UI->hButtonA, SW_HIDE);
	ShowWindow(UI->hButtonB, SW_HIDE);
	ShowWindow(UI->hButtonC, SW_HIDE);
	ShowWindow(UI->hButtonD, SW_HIDE);
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
	{
		HWND lParamAsHWND = (HWND)lParam;
		if (lParamAsHWND == UI->hButtonA || lParamAsHWND == UI->hButtonB ||
			lParamAsHWND == UI->hButtonC || lParamAsHWND == UI->hButtonD ||
			lParamAsHWND == UI->hButtonStart || lParamAsHWND == UI->hButtonHelp)
		{
			switch (wParam)
			{
			case ID_BUTTONA:
				QuestionH->AnswerClick('A');
				HandleAnswerButtonClick();
				break;
			case ID_BUTTONB:
				QuestionH->AnswerClick('B');
				HandleAnswerButtonClick();
				break;
			case ID_BUTTONC:
				QuestionH->AnswerClick('C');
				HandleAnswerButtonClick();
				break;
			case ID_BUTTOND:
				QuestionH->AnswerClick('D');
				HandleAnswerButtonClick();
				break;

			case ID_BUTTONSTART:
			{
				if (QuestionH->userScore == -1)
				{			 
					QuestionH->CountQuestions();
					HandleAnswerButtonClick();
					QuestionH->userScore++;
					ShowWindow(UI->hButtonA, SW_SHOW);
					ShowWindow(UI->hButtonB, SW_SHOW);
					ShowWindow(UI->hButtonC, SW_SHOW);
					ShowWindow(UI->hButtonD, SW_SHOW);
					ShowWindow(UI->hStaticQuestionNumber, SW_SHOW);
					ShowWindow(UI->hStaticQuestionNumberTitle, SW_SHOW);
					SetWindowTextA(UI->hButtonStart, LPSTR("Powodzenia"));
					SetWindowTextA(UI->hStaticQuestionNumber, LPSTR(ToString(QuestionH->currentQuestionIndex).c_str()));
				}
				else
				{
					//FinishQuiz();
				}
			}
			break;
			case ID_BUTTONHELP:
				MessageBoxA(NULL, LPSTR("Aby rozpoczac, kliknij przycisk \"Start\".\nWyswietli sie wtedy pytanie, aby odpowiedziec kliknij jeden z przyciskow A B C D.\n\nProgram by Patryk Mikulski\nAll rights reserved."), LPSTR("Help"), MB_ICONEXCLAMATION);
				break;
			}
		}
	}
	break;
	default:                      /* for messages that we don't deal with */
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

void HandleAnswerButtonClick()
{
	if (QuestionH->CheckTestFinished())
	{
		FinishQuiz();
	}

	SetWindowTextA(UI->hStaticQuestion, LPSTR(QuestionH->currentQuestionTask.c_str()));
	SetWindowTextA(UI->hStaticA, LPSTR(QuestionH->currentQuestionAnswerA.c_str()));
	SetWindowTextA(UI->hStaticB, LPSTR(QuestionH->currentQuestionAnswerB.c_str()));
	SetWindowTextA(UI->hStaticC, LPSTR(QuestionH->currentQuestionAnswerC.c_str()));
	SetWindowTextA(UI->hStaticD, LPSTR(QuestionH->currentQuestionAnswerD.c_str()));
	SetWindowTextA(UI->hStaticQuestionNumber, LPSTR(ToString(QuestionH->currentQuestionIndex).c_str()));
}

void FinishQuiz()
{
	string points = ToString(QuestionH->userScore);

	DWORD dwSize = GetWindowTextLength(UI->hTextUserName) + 1;
	Buffer1 = (LPSTR)GlobalAlloc(GPTR, dwSize + 1);
	GetWindowTextA(UI->hTextUserName, Buffer1, dwSize);

	ShowWindow(UI->hButtonA, SW_HIDE);
	ShowWindow(UI->hButtonB, SW_HIDE);
	ShowWindow(UI->hButtonC, SW_HIDE);
	ShowWindow(UI->hButtonD, SW_HIDE);

	QuestionH->SaveScore(Buffer1);

	string message = "Koniec, zdobyles/as " + points + " punkty/ow!";
	MessageBoxA(NULL, LPSTR(message.c_str()), LPSTR("Zakonczenie"), MB_ICONEXCLAMATION);
	PostQuitMessage(0);
}