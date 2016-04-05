#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <deque>

HHOOK hKeyboardHook;
KBDLLHOOKSTRUCT hooked_key; 

struct keyStore {
	DWORD scanCode;
	DWORD vkCode;
	DWORD flags;
	DWORD time;
	ULONG_PTR dwExtraInfo;
	bool isStale;
};

keyStore currentKey;
std::deque<keyStore> keyBuffer;

void bufHandle() {

	currentKey.scanCode = hooked_key.scanCode;
	currentKey.vkCode = hooked_key.scanCode;
	currentKey.flags = hooked_key.flags;
	currentKey.dwExtraInfo = hooked_key.dwExtraInfo;
	currentKey.time = hooked_key.time;

	while ((keyBuffer.size() > 1) && (keyBuffer.front().time < (currentKey.time - 1000))) {
		keyBuffer.pop_front();
	}

	keyBuffer.push_back(currentKey);
	if (keyBuffer.size() > 10) {
		float keyRate;
		keyRate = (currentKey.time - keyBuffer.front().time) / keyBuffer.size();
		if (keyRate < 35) {
			printf("\n\nHigh Rate\n\n");
		}
	}
}

LRESULT WINAPI Keylog(int nCode, WPARAM wParam, LPARAM lParam)
{

	if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
	{
		hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
	}
	bufHandle();
	return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

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

void main() {
	Hooker(NULL);
}
