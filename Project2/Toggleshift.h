#pragma once

void toggleShift(BOOL shiftState)
{
	if (shiftState)
		keybd_event(161, 0, 0, 0);
	else
		keybd_event(161, 0, KEYEVENTF_KEYUP, 0);
}