#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <deque>

HHOOK hKeyboardHook;
KBDLLHOOKSTRUCT hooked_key; 

std::deque<hooked_key> keyBuffer;

/*
bufHandle is the meat of the program

There is then a short loop to search over the double-ended queue to find any keys older than 1 second and remove them

The new key stroke is added to the end of the queue

The key rate is then calculated, checked against a tolerance and then alert is produced if necessary.

*/
void bufHandle() {

	while ((keyBuffer.size() > 1) && (keyBuffer.front().time < (hooked_key.time - 1000))) {
		keyBuffer.pop_front();
	}

	keyBuffer.push_back(hooked_key);
	if (keyBuffer.size() > 10) {
		float keyRate;
		keyRate = (currentKey.time - keyBuffer.front().time) / keyBuffer.size();
		if (keyRate < 35) {
			printf("\n\nHigh Rate\n\n");
		}
	}
}

//Check the key and call the bufHandle
LRESULT WINAPI Keylog(int nCode, WPARAM wParam, LPARAM lParam)
{

	if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
	{
		hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
	}
	bufHandle();
	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

//Setup a hook to catch all keystrokes
DWORD WINAPI Hooker(LPVOID lpParm)
{
	HINSTANCE hins;
	hins = GetModuleHandle(NULL);
	hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)Keylog, hins, 0);

	MSG message;

	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnhookWindowsHookEx(hKeyboardHook);
	return 0;
}

//Call Hooker
void main() {
	Hooker(NULL);
}
