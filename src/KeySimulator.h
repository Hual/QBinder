#pragma once

#include <Windows.h>

#include "util/limits.h"
#include "CGUI.h"
#include "CPageManager.h"

struct key_event_data
{
	DWORD thread;
	HANDLE event;
	char parameter[MAX_TEXT_LEN];
	size_t parameterLength;
};

class KeySimulator
{
public:
	static void initialize();
	static void simulate(const char* arr, const size_t len);
	static bool pop(WORD vkCode);
	static void updateHotkey(CGUI* pGUI, unsigned char index);
	static void updateHotkeyFromPage(CGUI* pGUI, CPageManager* pPageManager, unsigned short pageIndex, unsigned char index);
	static void unregisterHotkeys(CGUI* pGUI, unsigned short pageIndex);

	static const BYTE (&getTokens())[TOTAL_TOKENS][2] { return m_tokens; }
	static const BYTE (&getHoldTokens())[TOTAL_TOKENS][2] { return m_holdTokens; }
	static const unsigned short (&getMouseTokens())[TOTAL_MOUSE_TOKENS][3] { return m_mouseTokens; }
	static const BYTE* getOtherTokens() { return m_otherTokens; }

private:
	static DWORD WINAPI processor(LPVOID lpParam);
	static void simulate_LL(const char arr[], const size_t len, INPUT* keyInput = m_keyInput);
	static UINT sendInput_Safe(UINT cInputs, LPINPUT pInputs, int cbSize);

	static key_event_data m_data;
	static INPUT* m_keyInput;

	static const BYTE m_tokens[TOTAL_TOKENS][2];
	static const BYTE m_holdTokens[TOTAL_TOKENS][2];
	static const unsigned short m_mouseTokens[TOTAL_MOUSE_TOKENS][3];
	static const BYTE m_otherTokens[TOTAL_OTHER_TOKENS];

};