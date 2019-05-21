#include <Windows.h>
#include <iostream>
#include "Rng.h"
#include "Toggleshift.h"

const int ignoredKeyCodes[] { 162, 0xA0, 0x14, 8, 0xA1, 188, 190, 191, 186, 222, 220, 219, 221, 32, 8, 13, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 189, 187 };
BOOL lastCharWasG{ FALSE };
int lastTimesWereCaps{ 0 };

BOOL checkForCapsInsertion(PKBDLLHOOKSTRUCT p)
{
	int random = round(generateRandomNumber(1, 3));

	if (random != 1 && lastTimesWereCaps < 2)
	{
		lastTimesWereCaps++;
		std::cout << "RNG == " << random << " Caps counter: " << lastTimesWereCaps<< "\n";
		toggleShift(true);
		keybd_event(p->vkCode, 0, 0, 0);
		keybd_event(p->vkCode, 0, KEYEVENTF_KEYUP, 0);
		toggleShift(false);
		return true;
	}
	else
	{
		std::cout << "No caps\n";
		lastTimesWereCaps = 0;
		return false;
	}
		
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BOOL cancelReturn = FALSE;
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;

	if (wParam == WM_KEYDOWN && //checks if its a keydown event
		!(std::find(std::begin(ignoredKeyCodes), std::end(ignoredKeyCodes), p->vkCode) != std::end(ignoredKeyCodes)) && //checks for blacklisted keys defined in ignoredKeyCodes
		!(GetKeyState(VK_SHIFT) & 0x8000 || GetKeyState(VK_CAPITAL) & 0x8000 || GetKeyState(VK_CONTROL) & 0x8000)) //checks if some modifier keys are down
	{
		cancelReturn = checkForCapsInsertion(p);
	}

	if (cancelReturn)
		return cancelReturn; //Return nothing of use, resulting in the user input being completely ignored.
	else
		return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
	// Install the low-level keyboard & mouse hooks
	HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

	// Keep this application running until we're told to stop
	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {    //this while loop keeps the hook
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hhkLowLevelKybd);

	return(0);
}