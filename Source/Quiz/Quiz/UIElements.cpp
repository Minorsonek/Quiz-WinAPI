#include "UIElements.h"

UIElements::UIElements(HWND& hwnd, HINSTANCE& currentInstance)
{
	hButtonA = CreateWindowExA(WS_EX_CLIENTEDGE, LPSTR("BUTTON"), LPSTR("A"), WS_CHILD | WS_VISIBLE,
		100, 400, 100, 100, hwnd, (HMENU)ID_BUTTONA, currentInstance, NULL);

	hButtonB = CreateWindowExA(WS_EX_CLIENTEDGE, LPSTR("BUTTON"), LPSTR("B"), WS_CHILD | WS_VISIBLE,
		260, 400, 100, 100, hwnd, (HMENU)ID_BUTTONB, currentInstance, NULL);

	hButtonC = CreateWindowExA(WS_EX_CLIENTEDGE, LPSTR("BUTTON"), LPSTR("C"), WS_CHILD | WS_VISIBLE,
		440, 400, 100, 100, hwnd, (HMENU)ID_BUTTONC, currentInstance, NULL);

	hButtonD = CreateWindowExA(WS_EX_CLIENTEDGE, LPSTR("BUTTON"), LPSTR("D"), WS_CHILD | WS_VISIBLE,
		600, 400, 100, 100, hwnd, (HMENU)ID_BUTTOND, currentInstance, NULL);

	hButtonStart = CreateWindowExA(WS_EX_CLIENTEDGE, LPSTR("BUTTON"), LPSTR("Start"), WS_CHILD | WS_VISIBLE,
		680, 100, 90, 150, hwnd, (HMENU)ID_BUTTONSTART, currentInstance, NULL);

	hButtonHelp = CreateWindowExA(WS_EX_CLIENTEDGE, LPSTR("BUTTON"), LPSTR("Pomoc"), WS_CHILD | WS_VISIBLE,
		30, 180, 90, 150, hwnd, (HMENU)ID_BUTTONHELP, currentInstance, NULL);

	hTextUserName = CreateWindowExA(WS_EX_CLIENTEDGE, LPSTR("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
		150, 350, 300, 30, hwnd, NULL, currentInstance, NULL);


	hStaticTitle = CreateWindowExA(0, LPSTR("STATIC"), NULL, WS_CHILD | WS_VISIBLE |
		SS_CENTER, 150, 20, 500, 40, hwnd, NULL, currentInstance, NULL);

	hStaticQuestion = CreateWindowExA(0, LPSTR("STATIC"), NULL, WS_CHILD | WS_VISIBLE |
		SS_CENTER, 150, 80, 500, 150, hwnd, NULL, currentInstance, NULL);

	hStaticUserName = CreateWindowExA(0, LPSTR("STATIC"), NULL, WS_CHILD | WS_VISIBLE |
		SS_CENTER, 450, 350, 200, 30, hwnd, NULL, currentInstance, NULL);

	hStaticA = CreateWindowExA(0, LPSTR("STATIC"), NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 150, 130, 500, 50, hwnd, NULL, currentInstance, NULL);

	hStaticB = CreateWindowExA(0, LPSTR("STATIC"), NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 150, 180, 500, 50, hwnd, NULL, currentInstance, NULL);

	hStaticC = CreateWindowExA(0, LPSTR("STATIC"), NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 150, 230, 500, 50, hwnd, NULL, currentInstance, NULL);

	hStaticD = CreateWindowExA(0, LPSTR("STATIC"), NULL, WS_CHILD | WS_VISIBLE |
		SS_LEFT, 150, 280, 500, 50, hwnd, NULL, currentInstance, NULL);

	hStaticQuestionNumberTitle = CreateWindowExA(0, LPSTR("STATIC"), NULL, WS_CHILD | WS_VISIBLE |
		SS_CENTER, 30, 20, 60, 60, hwnd, NULL, currentInstance, NULL);

	hStaticQuestionNumber = CreateWindowExA(0, LPSTR("STATIC"), NULL, WS_CHILD | WS_VISIBLE |
		SS_CENTER, 30, 70, 60, 60, hwnd, NULL, currentInstance, NULL);


	SetWindowTextA(hStaticTitle, LPSTR("QUIZ"));
	SetWindowTextA(hStaticUserName, LPSTR("Podpisz sie tutaj"));
	SetWindowTextA(hStaticQuestionNumberTitle, LPSTR("Numer Pytania"));

	HFONT hFont = CreateFont(32, 0, 0, 0, 700, FALSE, TRUE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
	SendMessage(hStaticTitle, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));
}


UIElements& UIElements::GetInstance(HWND& hwnd, HINSTANCE& currentInstance)
{
	static UIElements objectInstance(hwnd, currentInstance);

	return objectInstance;
}
