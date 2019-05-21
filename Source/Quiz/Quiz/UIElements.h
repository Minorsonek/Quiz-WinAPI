#include <windows.h>
#include "ElementsIndexes.h"
#pragma once
class UIElements
{

private:

	UIElements(HWND& hwnd, HINSTANCE& currentInstance);

public:

	// Buttons
	HWND hButtonA;
	HWND hButtonB;
	HWND hButtonC;
	HWND hButtonD;
	HWND hButtonStart;
	HWND hButtonHelp;

	// Textboxes
	HWND hTextUserName;

	// Labels
	HWND hStaticQuestion;
	HWND hStaticUserName;
	HWND hStaticA;
	HWND hStaticB;
	HWND hStaticC;
	HWND hStaticD;
	HWND hStaticQuestionNumber;
	HWND hStaticQuestionNumberTitle;

	static UIElements& GetInstance(HWND& hwnd, HINSTANCE& currentInstance);

};

