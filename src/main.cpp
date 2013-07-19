#include "main.hpp"

ULONG_PTR ToggleVisualStyles(void)
{
    char dir[MAX_PATH];
    ULONG_PTR cookie = 0;
    ACTCTX aX = { sizeof(aX), ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_SET_PROCESS_DEFAULT | ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID, TEXT("shell32.dll"), 0, 0, dir, (LPCTSTR)124 };
    UINT cch = GetSystemDirectory(dir, sizeof(dir) / sizeof(*dir));
    if (cch >= sizeof(dir) / sizeof(*dir)) return 0;
    dir[cch] = TEXT(0);
    ActivateActCtx(CreateActCtx(&aX), &cookie);
    return cookie;
}

bool SimulateKeyArray(char __arr[], size_t __len, INPUT *__keyInput = new INPUT[4])
{
    __keyInput[0].type = INPUT_KEYBOARD;
    __keyInput[0].ki.wVk = VK_MENU;
    __keyInput[0].ki.dwFlags= KEYEVENTF_KEYUP;
    __keyInput[1].type = INPUT_KEYBOARD;
    __keyInput[1].ki.wVk = VK_CONTROL;
    __keyInput[1].ki.dwFlags= KEYEVENTF_KEYUP;
    __keyInput[2].type = INPUT_KEYBOARD;
    __keyInput[2].ki.wVk = VK_SHIFT;
    __keyInput[2].ki.dwFlags= KEYEVENTF_KEYUP;
    if(SendInput(3, __keyInput, sizeof(INPUT)) == 0) return false;
	for(unsigned i=0;i<__len;++i)
	{
		bool should_continue = false;
		if(i < __len-1 && __arr[i] == 37 && __arr[++i] != 37)
		{
			bool is_long = false;
			if( i < __len-1)
			{
				if(__arr[i] == 61)
				{
					++i;
					is_long = true;
					for(unsigned j=0;j<TOTAL_HOLD_TOKENS;++j)
					{
						if(__arr[i] == hold_tokens[j][0])
						{
							__keyInput[0].type = INPUT_KEYBOARD;
							__keyInput[0].ki.wVk = hold_tokens[j][1];
							__keyInput[0].ki.dwFlags = 0;
							if(SendInput(1, __keyInput, sizeof(INPUT)) == 0) return false;
							break;
						}
					}
					should_continue = true;
				}
				else if(__arr[i] == 33)
				{
					++i;
					is_long = true;
					for(unsigned j=0;j<TOTAL_HOLD_TOKENS;++j)
					{
						if(__arr[i] == hold_tokens[j][0])
						{
							__keyInput[0].type = INPUT_KEYBOARD;
							__keyInput[0].ki.wVk = hold_tokens[j][1];
							__keyInput[0].ki.dwFlags= KEYEVENTF_KEYUP;
							if(SendInput(1, __keyInput, sizeof(INPUT)) == 0) return false;
							break;
						}
					}
					should_continue = true;
				}
			}
			if(!is_long)
			{
				switch(__arr[i])
				{
				case 114:
					{
						srand((unsigned)time(NULL));
						int __r = rand()%(0xFE - 0x01 + 1)+0x01;
						__keyInput[0].type = INPUT_KEYBOARD;
						__keyInput[0].ki.wVk = __r;
						__keyInput[0].ki.dwFlags= 0;
						__keyInput[1].type = INPUT_KEYBOARD;
						__keyInput[1].ki.wVk = __r;
						__keyInput[1].ki.dwFlags= KEYEVENTF_KEYUP;
						if(SendInput(2, __keyInput, sizeof(INPUT)) == 0) return false;
						++i;
						break;
					}
				case 120:
					{
						++i;
						if(i < __len-2)
						{
							if(((__arr[i] >= 48 && __arr[i] <= 57)  || (__arr[i] >= 65 && __arr[i] <= 70) || (__arr[i] >= 97 && __arr[i] <= 102)) && ((__arr[i+1] >= 48 && __arr[i+1] <= 57)  || (__arr[i+1] >= 65 && __arr[i+1] <= 70) || (__arr[i+1] >= 97 && __arr[i+1] <= 102)))
							{
								char hex[3] = { __arr[i], __arr[++i], 0 };
								__keyInput[0].type = INPUT_KEYBOARD;
								__keyInput[0].ki.wVk = (WORD)strtol(hex, NULL, 16);
								__keyInput[0].ki.dwFlags= 0;
								__keyInput[1].type = INPUT_KEYBOARD;
								__keyInput[1].ki.wVk = (WORD)strtol(hex, NULL, 16);
								__keyInput[1].ki.dwFlags= KEYEVENTF_KEYUP;
								if(SendInput(2, __keyInput, sizeof(INPUT)) == 0) return false;
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
						if(__arr[i] >= 48 && __arr[i] <= 57) key[0] = __arr[i]; else { break; }
						++i;
						if(__arr[i] >= 48 && __arr[i] <= 57) key[1] = __arr[i];
						++i;
						k = atoi(key);
						if(k > 0 && k < 25)
						{
							__keyInput[0].type = INPUT_KEYBOARD;
							__keyInput[0].ki.wVk = 0x6F+k;
							__keyInput[0].ki.dwFlags= 0;
							__keyInput[1].type = INPUT_KEYBOARD;
							__keyInput[1].ki.wVk = 0x6F+k;
							__keyInput[1].ki.dwFlags= KEYEVENTF_KEYUP;
							if(SendInput(2, __keyInput, sizeof(INPUT)) == 0) return false;
						}
						break;
					}
				case 'T':
					{
						time_t rawtime;
						struct tm *timeinfo;
						time(&rawtime);
						timeinfo = localtime(&rawtime);
						char tym[16];
						sprintf(tym, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
						SimulateKeyArray(tym, strlen(tym), __keyInput);
						++i;
						break;
					}
				case 'J':
					{
						time_t rawtime;
						struct tm *timeinfo;
						time(&rawtime);
						timeinfo = localtime(&rawtime);
						char tym[16];
						sprintf(tym, "%02d/%02d/%d", timeinfo->tm_mday, timeinfo->tm_mon + 1, 1900 + timeinfo->tm_year);
						SimulateKeyArray(tym, strlen(tym), __keyInput);
						++i;
						break;
					}
				default:
					{
						for(unsigned j=0;j<TOTAL_TOKENS;++j)
						{
							if(__arr[i] == tokens[j][0])
							{
								__keyInput[0].type = INPUT_KEYBOARD;
								__keyInput[0].ki.wVk = tokens[j][1];
								__keyInput[0].ki.dwFlags= 0;
								__keyInput[1].type = INPUT_KEYBOARD;
								__keyInput[1].ki.wVk = tokens[j][1];
								__keyInput[1].ki.dwFlags= KEYEVENTF_KEYUP;
								if(SendInput(2, __keyInput, sizeof(INPUT)) == 0) return false;
								break;
							}
						}
						should_continue = true;
					}
				}
			}
		}
		if(should_continue) continue;
		SHORT __vk_shrt = VkKeyScanEx(__arr[i], GetKeyboardLayout(0));
		if((__vk_shrt & 0xff00) >> 8 == 1)
		{
			__keyInput[0].type = INPUT_KEYBOARD;
			__keyInput[0].ki.wVk = VK_SHIFT;
			__keyInput[0].ki.dwFlags= 0;
			__keyInput[1].type = INPUT_KEYBOARD;
			__keyInput[1].ki.wVk = __vk_shrt & 0xff;
			__keyInput[1].ki.dwFlags= 0;
			__keyInput[2].type = INPUT_KEYBOARD;
			__keyInput[2].ki.wVk = __vk_shrt & 0xff;
			__keyInput[2].ki.dwFlags= KEYEVENTF_KEYUP;
			__keyInput[3].type = INPUT_KEYBOARD;
			__keyInput[3].ki.wVk = VK_SHIFT;
			__keyInput[3].ki.dwFlags= KEYEVENTF_KEYUP;
			if(SendInput(4, __keyInput, sizeof(INPUT)) == 0) return false;
		}
		else {
			__keyInput[0].type = INPUT_KEYBOARD;
			__keyInput[0].ki.wVk = __vk_shrt & 0xff;
			__keyInput[0].ki.dwFlags= 0;
			__keyInput[1].type = INPUT_KEYBOARD;
			__keyInput[1].ki.wVk = __vk_shrt & 0xff;
			__keyInput[1].ki.dwFlags= KEYEVENTF_KEYUP;
			if(SendInput(2, __keyInput, sizeof(INPUT)) == 0) return false;
		}
	}
	return true;
}

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;
    SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    ToggleVisualStyles();
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = "QbinderApp";
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_ICON));
    wincl.hIconSm = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_ICON));
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = BackgroundBrush;
    if (!RegisterClassEx (&wincl)) return 0;
    hwnd = CreateWindowEx(0, "QbinderApp", "QBinder", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX, (GetSystemMetrics(SM_CXSCREEN) - 800)/2, (GetSystemMetrics(SM_CYSCREEN) - 600)/2, 800, 600, HWND_DESKTOP, NULL, hThisInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    while (GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    return messages.wParam;
}

LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if(code == 0)
    {
        KBDLLHOOKSTRUCT* kbstr = (KBDLLHOOKSTRUCT*)lParam;
        for(unsigned i=0;i<_BOXES;++i)
        {
            int msg = SendMessage(KeyComboBox[i], (UINT)CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
            if(enabled[i] && msg > -1)
            {
                if(wParam == WM_KEYDOWN)
                {
                    if(!key_down[i] && kbstr->vkCode == keys_value[msg])
                    {
                        if(IsChecked(checkBox[0][i])) if(!(GetKeyState(VK_CONTROL) & 0x8000)) continue;
                        if(IsChecked(checkBox[1][i])) if(!(GetKeyState(VK_SHIFT) & 0x8000)) continue;
                        if(IsChecked(checkBox[2][i])) if(!(GetKeyState(VK_MENU) & 0x8000)) continue;
                        char txt[256];
                        GetText(TextBox[i], txt);
                        if(!strcmp(txt, "") || txt == NULL) continue;
                        txt[strlen(txt)] = 0;
                        SimulateKeyArray(txt, strlen(txt), keyInput);
                        key_down[i] = true;
                        return -1;
                    }
                }
                else {
                    key_down[i] = false;
                }
            }
        }
    }
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    unsigned wmId=0, wmEvent=0;
    wmId = LOWORD(wParam);
    wmEvent = HIWORD(wParam);
    switch(message)
    {
        case WM_CTLCOLORSTATIC:
        {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(0,0,0));
            SetBkMode(hdcStatic, TRANSPARENT);
            SetBkColor(hdcStatic, RGB(236,233,216));
            return (INT_PTR)BackgroundBrush;
        }
        break;
        case WM_CREATE:
        {
            for(unsigned i=0;i<_BOXES;++i)
            {
                TextBox[i] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 10, (i*30)+55, 100+TEXTBOX_SIZE, 23, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
                SendMessage(TextBox[i], WM_SETFONT, (WPARAM)DefaultFont, MAKELPARAM(FALSE, 0));
            }
            for(unsigned i=0;i<_BOXES;++i)
            {
                button[i] = CreateWindowEx(0, "BUTTON", "Enable", WS_CHILD | WS_VISIBLE, 645-24+TEXTBOX_SIZE-(WHITESPACE_SIZE*4), (i*30)+54, 80, 23, hwnd, (HMENU)(1000+i), GetModuleHandle(NULL), NULL);
                SendMessage(button[i], WM_SETFONT, (WPARAM)DefaultFont, MAKELPARAM(FALSE, 0));
            }
            for(unsigned i=0;i<_BOXES;++i)
            {
                checkBox[0][i] = CreateWindowEx(0, "BUTTON", "", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 200-24+TEXTBOX_SIZE-WHITESPACE_SIZE, (i*30)+56, 15, 20, hwnd, (HMENU)(2000+i), GetModuleHandle(NULL), NULL);
            }
            for(unsigned i=0;i<_BOXES;++i)
            {
                checkBox[1][i] = CreateWindowEx(0, "BUTTON", "", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 305-25+TEXTBOX_SIZE-(WHITESPACE_SIZE*2), (i*30)+56, 15, 20, hwnd, (HMENU)(2016+i), GetModuleHandle(NULL), NULL);
            }
            for(unsigned i=0;i<_BOXES;++i)
            {
                checkBox[2][i] = CreateWindowEx(0, "BUTTON", "", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 410-25+TEXTBOX_SIZE-(WHITESPACE_SIZE*3), (i*30)+56, 15, 20, hwnd, (HMENU)(2032+i), GetModuleHandle(NULL), NULL);
            }
            for(unsigned i=0;i<_BOXES;++i)
            {
                KeyComboBox[i] = CreateWindowEx(0, "COMBOBOX", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_TABSTOP | WS_VSCROLL, 515-24-36+8+TEXTBOX_SIZE-(WHITESPACE_SIZE*4), (i*30)+55, 146, 335, hwnd, (HMENU)IDC_MAIN_COMBO, GetModuleHandle(NULL), NULL);
                SendMessage(KeyComboBox[i], WM_SETFONT, (WPARAM)DefaultFont, MAKELPARAM(FALSE, 0));
                for(unsigned j=0;j<NORMAL_KEYS_SIZE;++j) SendMessage(KeyComboBox[i], CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCTSTR)keys_string[j]));
            }
            CreateWindowEx(0, "static", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 1, 35, 785, 1, hwnd, (HMENU)IDC_MAIN_COMBO, GetModuleHandle(NULL), NULL);
            CreateWindowEx(0, "static", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 255, 0, 1, 544, hwnd, (HMENU)IDC_MAIN_COMBO, GetModuleHandle(NULL), NULL);
            CreateWindowEx(0, "static", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 345, 0, 1, 544, hwnd, (HMENU)IDC_MAIN_COMBO, GetModuleHandle(NULL), NULL);
            CreateWindowEx(0, "static", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 435, 0, 1, 544, hwnd, (HMENU)IDC_MAIN_COMBO, GetModuleHandle(NULL), NULL);
            CreateWindowEx(0, "static", "", WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 525, 0, 1, 544, hwnd, (HMENU)IDC_MAIN_COMBO, GetModuleHandle(NULL), NULL);
            HWND lbl = CreateWindowEx(0, "static", "Text to simulate", WS_CHILD | WS_VISIBLE,75, 8, 150, 20, hwnd, (HMENU)IDC_MAIN_COMBO, GetModuleHandle(NULL), NULL);
            SendMessage(lbl, WM_SETFONT, (WPARAM)HeaderFont, MAKELPARAM(FALSE, 0));
            lbl = CreateWindowEx(0, "static", "Control", WS_CHILD | WS_VISIBLE,274, 8, 60, 20, hwnd, (HMENU)IDC_MAIN_COMBO, GetModuleHandle(NULL), NULL);
            SendMessage(lbl, WM_SETFONT, (WPARAM)HeaderFont, MAKELPARAM(FALSE, 0));
            lbl = CreateWindowEx(0, "static", "Shift", WS_CHILD | WS_VISIBLE, 373, 8, 60, 20, hwnd, (HMENU)IDC_MAIN_COMBO, GetModuleHandle(NULL), NULL);
            SendMessage(lbl, WM_SETFONT, (WPARAM)HeaderFont, MAKELPARAM(FALSE, 0));
            lbl = CreateWindowEx(0, "static", "Alt", WS_CHILD | WS_VISIBLE, 471, 8, 20, 20, hwnd, (HMENU)IDC_MAIN_COMBO, GetModuleHandle(NULL), NULL);
            SendMessage(lbl, WM_SETFONT, (WPARAM)HeaderFont, MAKELPARAM(FALSE, 0));
            lbl = CreateWindowEx(0, "static", "Hotkey && toggle", WS_CHILD | WS_VISIBLE, 596, 8, 300, 20, hwnd, (HMENU)IDC_MAIN_COMBO, GetModuleHandle(NULL), NULL);
            SendMessage(lbl, WM_SETFONT, (WPARAM)HeaderFont, MAKELPARAM(FALSE, 0));
        }
        break;
        case WM_COMMAND:
            if(wmId >= 1000 && wmId <= 999+_BOXES)
            {
                if(enabled[wmId-1000])
                {
                    SetText(button[wmId-1000], "Enable");
                    enabled[wmId-1000] = false;
                }
                else {
                    SetText(button[wmId-1000], "Disable");
                    enabled[wmId-1000] = true;
                }
            }
            else if(wmId >= 2000 && wmId <= 1999+(_BOXES*3))
            {
                SendMessage(checkBox[(wmId-2000)/16][(wmId-2000)%16], BM_SETCHECK, !IsChecked(checkBox[(wmId-2000)/16][(wmId-2000)%16]), (LPARAM)"Enable");
            }
            switch (wmId)
            {
            case ID_FILE_EXIT:
                DestroyWindow(hwnd);
                break;
            default:
                return DefWindowProc(hwnd, message, wParam, lParam);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        default:
        {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }
    return 0;
}
