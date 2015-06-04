#include "util/misc.h"
#include "KeySimulator.h"

const BYTE KeySimulator::m_tokens[TOTAL_TOKENS][2] = {
	{ 0x65, VK_RETURN },
	{ 0x73, VK_LSHIFT },
	{ 0x53, VK_RSHIFT },
	{ 0x62, VK_BACK },
	{ 0x74, VK_TAB },
	{ 0x63, VK_LCONTROL },
	{ 0x6F, VK_RCONTROL },
	{ 0x61, VK_LMENU },
	{ 0x41, VK_RMENU },
	{ 0x6C, VK_CAPITAL },
	{ 0x70, VK_ESCAPE },
	{ 0x6E, VK_END },
	{ 0x75, VK_PRIOR },
	{ 0x64, VK_NEXT },
	{ 0x68, VK_HOME },
	{ 0x69, VK_INSERT },
	{ 0x55, VK_UP },
	{ 0x44, VK_DOWN },
	{ 0x4C, VK_LEFT },
	{ 0x52, VK_RIGHT }
};
const BYTE KeySimulator::m_otherTokens[TOTAL_OTHER_TOKENS] = {
	0x72,
	0x78,
	0x66,
	0x54,
	0x4A,
	0x43,
	0x25
};
const BYTE KeySimulator::m_holdTokens[TOTAL_TOKENS][2] = {
	{ 0x73, VK_SHIFT },
	{ 0x63, VK_CONTROL },
	{ 0x61, VK_MENU }
};

const unsigned short KeySimulator::m_mouseTokens[TOTAL_MOUSE_TOKENS][3] = {
	{ 0x42, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP },
	{ 0x49, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP },
	{ 0x4D, MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP },
	{ 0x47, MOUSEEVENTF_XDOWN, MOUSEEVENTF_XUP }
};

const DWORD keys_value[NORMAL_KEYS_SIZE] = {
	0x30,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,
	0x41,
	0x42,
	0x43,
	0x44,
	0x45,
	0x46,
	0x47,
	0x48,
	0x49,
	0x4A,
	0x4B,
	0x4C,
	0x4D,
	0x4E,
	0x4F,
	0x50,
	0x51,
	0x52,
	0x53,
	0x54,
	0x55,
	0x56,
	0x57,
	0x58,
	0x59,
	0x5A,
	0x60,
	0x61,
	0x62,
	0x63,
	0x64,
	0x65,
	0x66,
	0x67,
	0x68,
	0x69,
	0x6A,
	0x6B,
	0x6C,
	0x6D,
	0x6E,
	0x6F,
	0x70,
	0x71,
	0x72,
	0x73,
	0x74,
	0x75,
	0x76,
	0x77,
	0x78,
	0x79,
	0x7A,
	0x7B,
	0x7C,
	0x7D,
	0x7E,
	0x7F,
	0x80,
	0x81,
	0x82,
	0x83,
	0x84,
	0x85,
	0x86,
	0x87,
	0xBA,
	0xBB,
	0xBC,
	0xBD,
	0xBE,
	0xBF,
	0xC0,
	0xDB,
	0xDC,
	0xDD,
	0xDE,
	0x08,
	0x09,
	0x0D,
	0x1B,
	0x21,
	0x22,
	0x23,
	0x24,
	0x25,
	0x26,
	0x27,
	0x28,
	0x29,
	0x2C,
	0x2D,
	0x2E
};

const DWORD mouse_value[MOUSE_BUTTONS_SIZE] = {
	0x0201,
	0x0204,
	0x0207,
	0x020B
};

const DWORD media_keys_value[MEDIA_KEYS_SIZE] = {
	0xB1,
	0xB3,
	0xB2,
	0xB0
};

INPUT* KeySimulator::m_keyInput = new INPUT[4];
key_event_data KeySimulator::m_data = { NULL };

void KeySimulator::initialize()
{
	m_data.event = CreateEvent(NULL, 1, 0, "kevent");
	CreateThread(NULL, 0, processor, NULL, 0, &m_data.thread);
}

DWORD WINAPI KeySimulator::processor(LPVOID lpParam)
{
	while (true)
	{
		while (WaitForSingleObjectEx(m_data.event, INFINITE, 1) != WAIT_OBJECT_0)
			Sleep(10);

		simulate_LL(m_data.parameter, m_data.parameterLength, m_keyInput);
	}

	ExitThread(0);
	return 0;
}

void KeySimulator::simulate_LL(const char* arr, const size_t len, INPUT* keyInput)
{
	memset(keyInput, 0, sizeof(INPUT) * 4);
	keyInput[0].type = INPUT_KEYBOARD;
	keyInput[0].ki.wVk = VK_MENU;
	keyInput[0].ki.dwFlags = KEYEVENTF_KEYUP;
	keyInput[1].type = INPUT_KEYBOARD;
	keyInput[1].ki.wVk = VK_CONTROL;
	keyInput[1].ki.dwFlags = KEYEVENTF_KEYUP;
	keyInput[2].type = INPUT_KEYBOARD;
	keyInput[2].ki.wVk = VK_SHIFT;
	keyInput[2].ki.dwFlags = KEYEVENTF_KEYUP;

	if (!SendInput(3, keyInput, sizeof(INPUT)))
		return;

	for (unsigned i = 0; i < len; ++i)
	{
		bool should_continue = 0;
		if (i < len - 1 && arr[i] == 37 && arr[++i] != 37)
		{
			bool is_long = 0;
			if (i < len - 1)
			{
				if (arr[i] == 61)
				{
					++i;
					is_long = 1;
					for (unsigned j = 0; j < TOTAL_HOLD_TOKENS; ++j)
					{
						if (arr[i] == m_holdTokens[j][0])
						{
							memset(keyInput, 0, sizeof(INPUT) * 4);
							keyInput[0].type = INPUT_KEYBOARD;
							keyInput[0].ki.wVk = m_holdTokens[j][1];
							keyInput[0].ki.dwFlags = 0;

							if (SendInput(1, keyInput, sizeof(INPUT)) == 0)
								return;
							
							break;
						}
					}
					should_continue = 1;
				}
				else if (arr[i] == 33)
				{
					++i;
					is_long = 1;
					for (unsigned j = 0; j < TOTAL_HOLD_TOKENS; ++j)
					{
						if (arr[i] == m_holdTokens[j][0])
						{
							memset(keyInput, 0, sizeof(INPUT) * 4);
							keyInput[0].type = INPUT_KEYBOARD;
							keyInput[0].ki.wVk = m_holdTokens[j][1];
							keyInput[0].ki.dwFlags = KEYEVENTF_KEYUP;

							if (SendInput(1, keyInput, sizeof(INPUT)) == 0)
								return;
							
							break;
						}
					}
					should_continue = 1;
				}
			}
			if (!is_long)
			{
				switch (arr[i])
				{
				case 114:
				{
					memset(keyInput, 0, sizeof(INPUT) * 4);
					const int __r = random(0x01, 0xFE);
					keyInput[0].type = INPUT_KEYBOARD;
					keyInput[0].ki.wVk = __r;
					keyInput[0].ki.dwFlags = 0;
					keyInput[1].type = INPUT_KEYBOARD;
					keyInput[1].ki.wVk = __r;
					keyInput[1].ki.dwFlags = KEYEVENTF_KEYUP;

					if (SendInput(2, keyInput, sizeof(INPUT)) == 0)
						return;

					++i;
					break;
				}
				case 120:
				{
					++i;
					if (i < len - 2)
					{
						if (((arr[i] >= 48 && arr[i] <= 57) || (arr[i] >= 65 && arr[i] <= 70) || (arr[i] >= 97 && arr[i] <= 102)) && ((arr[i + 1] >= 48 && arr[i + 1] <= 57) || (arr[i + 1] >= 65 && arr[i + 1] <= 70) || (arr[i + 1] >= 97 && arr[i + 1] <= 102)))
						{
							memset(keyInput, 0, sizeof(INPUT) * 4);
							const char hex[3] = { arr[i], arr[++i], 0 };
							keyInput[0].type = INPUT_KEYBOARD;
							keyInput[0].ki.wVk = (WORD)strtol(hex, NULL, 16);
							keyInput[0].ki.dwFlags = 0;
							keyInput[1].type = INPUT_KEYBOARD;
							keyInput[1].ki.wVk = (WORD)strtol(hex, NULL, 16);
							keyInput[1].ki.dwFlags = KEYEVENTF_KEYUP;

							if (SendInput(2, keyInput, sizeof(INPUT)) == 0)
								return;

							++i;
						}
					}
					break;
				}
				case 102:
				{
					++i;
					char key[3] = { 0, 0, 0 };
					int k = 0;

					if (arr[i] >= 48 && arr[i] <= 57)
						key[0] = arr[i];
					else
						break;

					++i;
					
					if (arr[i] >= 48 && arr[i] <= 57)
						key[1] = arr[i];
					
					++i;
					k = atoi(key);

					if (k > 0 && k < 25)
					{
						memset(keyInput, 0, sizeof(INPUT) * 4);
						keyInput[0].type = INPUT_KEYBOARD;
						keyInput[0].ki.wVk = 0x6F + k;
						keyInput[0].ki.dwFlags = 0;
						keyInput[1].type = INPUT_KEYBOARD;
						keyInput[1].ki.wVk = 0x6F + k;
						keyInput[1].ki.dwFlags = KEYEVENTF_KEYUP;

						if (SendInput(2, keyInput, sizeof(INPUT)) == 0)
							return;

					}
					break;
				}
				case 67:
				{
					bool should_break = 0;
					++i;
					char key[5] = { 0, 0, 0, 0, 0 };
					int k = 0;

					for (unsigned j = 0; j < sizeof(key) - 1; ++j)
					{
						if (arr[i] >= 48 && arr[i] <= 57) key[j] = arr[i];
						else
						{
							should_break = 1;
							break;
						}
						++i;
					}

					if (should_break)
						break;
					k = atoi(key);
					Sleep(k * 1000);

					break;
				}
				case 84:
				{
					char tym[16];
					GetSystemTime(tym, 0);
					simulate_LL(tym, strlen(tym));
					++i;
					break;
				}
				case 74:
				{
					char tym[16];
					GetSystemTime(tym, 1);
					simulate_LL(tym, strlen(tym));
					++i;
					break;
				}
				default:
				{
					for (unsigned j = 0; j < TOTAL_TOKENS; ++j)
					{
						if (arr[i] == m_tokens[j][0])
						{
							memset(keyInput, 0, sizeof(INPUT) * 4);
							keyInput[0].type = INPUT_KEYBOARD;
							keyInput[0].ki.wVk = m_tokens[j][1];
							keyInput[0].ki.dwFlags = 0;
							keyInput[1].type = INPUT_KEYBOARD;
							keyInput[1].ki.wVk = m_tokens[j][1];
							keyInput[1].ki.dwFlags = KEYEVENTF_KEYUP;

							if (SendInput(2, keyInput, sizeof(INPUT)) == 0)
								return;

							break;
						}
					}
					for (unsigned j = 0; j < TOTAL_MOUSE_TOKENS; ++j)
					{
						if (arr[i] == m_mouseTokens[j][0])
						{
							memset(keyInput, 0, sizeof(INPUT) * 4);
							keyInput[0].type = INPUT_MOUSE;
							keyInput[0].mi.dwFlags = m_mouseTokens[j][1];
							keyInput[1].type = INPUT_MOUSE;
							keyInput[1].mi.dwFlags = m_mouseTokens[j][2];

							if (SendInput(2, keyInput, sizeof(INPUT)) == 0)
								return;

							break;
						}
					}
					should_continue = 1;
				}
				}
			}
		}

		if (should_continue)
			continue;

		const short __vk_shrt = VkKeyScanEx(arr[i], GetKeyboardLayout(0));
		memset(keyInput, 0, sizeof(INPUT) * 4);

		if ((__vk_shrt & 0xff00) >> 8 == 1)
		{
			keyInput[0].type = INPUT_KEYBOARD;
			keyInput[0].ki.wVk = VK_SHIFT;
			keyInput[0].ki.dwFlags = 0;
			keyInput[1].type = INPUT_KEYBOARD;
			keyInput[1].ki.wVk = __vk_shrt & 0xff;
			keyInput[1].ki.dwFlags = 0;
			keyInput[2].type = INPUT_KEYBOARD;
			keyInput[2].ki.wVk = __vk_shrt & 0xff;
			keyInput[2].ki.dwFlags = KEYEVENTF_KEYUP;
			keyInput[3].type = INPUT_KEYBOARD;
			keyInput[3].ki.wVk = VK_SHIFT;
			keyInput[3].ki.dwFlags = KEYEVENTF_KEYUP;

			if (SendInput(4, keyInput, sizeof(INPUT)) == 0)
				return;

		}
		else
		{
			keyInput[0].type = INPUT_KEYBOARD;
			keyInput[0].ki.wVk = __vk_shrt & 0xff;
			keyInput[0].ki.dwFlags = 0;
			keyInput[1].type = INPUT_KEYBOARD;
			keyInput[1].ki.wVk = __vk_shrt & 0xff;
			keyInput[1].ki.dwFlags = KEYEVENTF_KEYUP;

			if (SendInput(2, keyInput, sizeof(INPUT)) == 0)
				return;

		}
	}
}

void KeySimulator::simulate(const char* arr, const size_t len)
{
	m_data.parameterLength = len;
	strcpy_s(m_data.parameter, arr);
	PulseEvent(m_data.event);
}

bool KeySimulator::pop(WORD vkCode)
{
	memset(m_keyInput, 0, sizeof(INPUT) * 4);
	m_keyInput[0].type = INPUT_KEYBOARD;
	m_keyInput[0].ki.wVk = vkCode;
	m_keyInput[0].ki.dwFlags = KEYEVENTF_KEYUP;

	return true;
}

void KeySimulator::updateHotkey(CGUI* pGUI, unsigned char index)
{
	UnregisterHotKey(pGUI->getMainWindow(), pGUI->getSelectedPageTab() * ROWS + index);

	if (Bitwise::Get<bool_16>(pGUI->getButtonStates(), index) != 0)
	{
		UINT fsModifiers = 0x4000;

		if (IsChecked(pGUI->getCheckBoxControls()[0][index]))
			fsModifiers |= MOD_CONTROL;

		if (IsChecked(pGUI->getCheckBoxControls()[1][index]))
			fsModifiers |= MOD_SHIFT;

		if (IsChecked(pGUI->getCheckBoxControls()[2][index]))
			fsModifiers |= MOD_ALT;

		RegisterHotKey(pGUI->getMainWindow(), pGUI->getSelectedPageTab() * 16 + index, fsModifiers, keys_value[pGUI->getComboBoxSelectedItem(index)]);
	}
}

void KeySimulator::updateHotkeyFromPage(CGUI* pGUI, CPageManager* pPageManager, unsigned short pageIndex, unsigned char index)
{
	UnregisterHotKey(pGUI->getMainWindow(), pageIndex * ROWS + index);
	CPage& pPage = pPageManager->at(pageIndex);

	if (Bitwise::Get<bool_16>(pPage.buttonStates(), index) != 0)
	{
		UINT fsModifiers = 0x4000;
		
		if (Bitwise::Get<bool_16>(pPage.ctrlStates(), index))
			fsModifiers |= MOD_CONTROL;

		if (Bitwise::Get<bool_16>(pPage.shiftStates(), index))
			fsModifiers |= MOD_SHIFT;

		if (Bitwise::Get<bool_16>(pPage.altStates(), index))
			fsModifiers |= MOD_ALT;

		RegisterHotKey(pGUI->getMainWindow(), pageIndex * 16 + index, fsModifiers, keys_value[pPage.comboBoxSelectedItems()[index]]);
	}
}

void KeySimulator::unregisterHotkeys(CGUI* pGUI, unsigned short pageIndex)
{
	for (unsigned char i = 0; i < ROWS; ++i)
		UnregisterHotKey(pGUI->getMainWindow(), pageIndex * ROWS + i);

}

UINT KeySimulator::sendInput_Safe(UINT cInputs, LPINPUT pInputs, int cbSize)
{
	UINT result;

	while ((result = SendInput(cInputs, pInputs, cbSize)) == 0)
		Sleep(100);

	return result;
}