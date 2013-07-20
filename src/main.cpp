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
    SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    ToggleVisualStyles();
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = "QbinderApp";
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
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

void HideHelpBox(void)
{
    ShowWindow(TextBox[textBoxHidden[0]], SW_SHOW);
    ShowWindow(TextBox[textBoxHidden[1]], SW_SHOW);
    ShowWindow(TextBox[textBoxHidden[2]], SW_SHOW);
    ShowWindow(helpBox, SW_HIDE);
    helpBoxShown = false;
    return;
}

void UpdateText(void)
{
    char chr[256];
    int len = GetText(TextBox[lastTextBox], chr);
    int item = (int)GetSelectedListBoxItem(helpBox);
    if(chr[len-1] == '%')
    {
        if(item < TOTAL_TOKENS && len < 255) { sprintf(chr, "%s%c", chr, tokens[item][0]); ++len; }
        else if(item >= TOTAL_TOKENS && item < TOTAL_TOKENS+(TOTAL_HOLD_TOKENS*2) && len < 254) { sprintf(chr, "%s%c%c", chr, (((item-TOTAL_TOKENS)/TOTAL_HOLD_TOKENS) ? ('!') : ('=')), hold_tokens[(item-TOTAL_TOKENS)%TOTAL_HOLD_TOKENS][0]); len+=2; }
        else if(item >= TOTAL_TOKENS+(TOTAL_HOLD_TOKENS*2) && item < TOTAL_TOKENS+(TOTAL_HOLD_TOKENS*2)+TOTAL_OTHER_TOKENS && len < 255) { sprintf(chr, "%s%c", chr, other_tokens[item - (TOTAL_TOKENS+(TOTAL_HOLD_TOKENS*2))]); ++len; }
    }
    else if(chr[len-2] == '%' && (chr[len-1] == '!' || chr[len-1] == '=') && len < 255)
    {
        sprintf(chr, "%s%c", chr, hold_tokens[item][0]);
        ++len;
    }
    SetText(TextBox[lastTextBox], chr);
    HideHelpBox();
    SetFocus(TextBox[lastTextBox]);
    SendMessage(TextBox[lastTextBox],EM_SETSEL,len,len);
    return;
}

LRESULT CALLBACK LowLevelMouseProc(int code, WPARAM wParam, LPARAM lParam)
{
    if(code == 0)
    {
        int item = GetSelectedItem(helpBox);
        if(wParam == WM_LBUTTONDOWN && helpBoxShown && (int)GetActiveWindow() && GetFocus() == helpBox && item > -1)
        {
            MSLLHOOKSTRUCT* mhs = (MSLLHOOKSTRUCT*)lParam;
            if((mhs->time)-lastMousePress <= 500 && lastItem == item)
            {
                UpdateText();
            }
            lastMousePress = mhs->time;
            lastItem = item;
        }
    }
    return 0;
}

LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if(code == 0)
    {
        KBDLLHOOKSTRUCT* kbstr = (KBDLLHOOKSTRUCT*)lParam;
        if(kbstr->vkCode == VK_ESCAPE && helpBoxShown && (int)GetActiveWindow())
        {
            HideHelpBox();
            return -1;
        }
        else if(kbstr->vkCode == VK_RETURN && helpBoxShown && (int)GetActiveWindow() && GetFocus() == helpBox && GetSelectedItem(helpBox) > -1)
        {
            UpdateText();
            return -1;
        }
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
            helpBox = CreateWindow ("LISTBOX", "", WS_CHILD | WS_VSCROLL | WS_BORDER, 0,0,244,100, hwnd, (HMENU)3001, NULL, NULL);
            SendMessage(helpBox, WM_SETFONT, (WPARAM)DefaultFont, MAKELPARAM(FALSE, 0));
            for(unsigned i=0;i<_BOXES;++i)
            {
                TextBox[i] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 10, (i*30)+55, 100+TEXTBOX_SIZE, 23, hwnd, (HMENU)(IDC_MAIN_EDIT+i), GetModuleHandle(NULL), NULL);
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
            if(wmId >= IDC_MAIN_EDIT && wmId < IDC_MAIN_EDIT+16)
            {
                if(wmEvent == 768)
                {
                    char buf[256];
                    int len = GetText(TextBox[wmId-103], buf);
                    if((len > lastSize && lastTextBox == wmId-103) || lastTextBox != wmId-103)
                    {
                        if(buf[strlen(buf)-1] == '%')
                        {
                            if(strlen(buf) > 1 && buf[strlen(buf)-2] == '%' && helpBoxShown) HideHelpBox();
                            else {
                                SendMessage(helpBox, LB_RESETCONTENT, 0, 0);
                                for(unsigned i=0;i<TOTAL_TOKENS;++i)
                                {
                                    char buf[32];
                                    sprintf(buf, "%%%c   -   %s", tokens[i][0], tokens_desc[i]);
                                    AddString(helpBox, buf);
                                }
                                for(unsigned i=0;i<TOTAL_HOLD_TOKENS;++i)
                                {
                                    char _buf[32];
                                    sprintf(_buf, "%%=%c   -   Hold %s", hold_tokens[i][0], hold_tokens_desc[i]);
                                    AddString(helpBox, _buf);
                                }
                                for(unsigned i=0;i<TOTAL_HOLD_TOKENS;++i)
                                {
                                    char _buf[32];
                                    sprintf(_buf, "%%!%c   -   Release %s", hold_tokens[i][0], hold_tokens_desc[i]);
                                    AddString(helpBox, _buf);
                                }
                                for(unsigned i=0;i<TOTAL_OTHER_TOKENS;++i)
                                {
                                    char _buf[32];
                                    sprintf(_buf, "%%%c   -   %s", other_tokens[i], other_tokens_desc[i]);
                                    AddString(helpBox, _buf);
                                }
                                if(wmId-103 >= 13)
                                {
                                    SetWindowPos(helpBox, 0, 10, ((wmId-103)*30)+78-120, 235, 98, 0);
                                    ShowWindow(TextBox[wmId-104], SW_HIDE);
                                    ShowWindow(TextBox[wmId-105], SW_HIDE);
                                    ShowWindow(TextBox[wmId-106], SW_HIDE);
                                    textBoxHidden[0] = wmId-104;
                                    textBoxHidden[1] = wmId-105;
                                    textBoxHidden[2] = wmId-106;
                                }
                                else {
                                    SetWindowPos(helpBox, 0, 10, ((wmId-103)*30)+78, 235, 98, 0);
                                    ShowWindow(TextBox[wmId-102], SW_HIDE);
                                    ShowWindow(TextBox[wmId-101], SW_HIDE);
                                    ShowWindow(TextBox[wmId-100], SW_HIDE);
                                    textBoxHidden[0] = wmId-102;
                                    textBoxHidden[1] = wmId-101;
                                    textBoxHidden[2] = wmId-100;
                                }
                                ShowWindow(helpBox, SW_SHOW);
                                helpBoxShown = true;
                            }
                        }
                        else if(strlen(buf) > 1 && buf[strlen(buf)-2] == '%' && helpBoxShown)
                        {
                            if(buf[strlen(buf)-1] == '=' || buf[strlen(buf)-1] == '!')
                            {
                                SendMessage(helpBox, LB_RESETCONTENT, 0, 0);
                                for(unsigned i=0;i<TOTAL_HOLD_TOKENS;++i)
                                {
                                    char _buf[32];
                                    sprintf(_buf, "%%%c%c", buf[strlen(buf)-1], hold_tokens[i][0]);
                                    AddString(helpBox, _buf);
                                }
                            }
                            else
                                HideHelpBox();
                                SetFocus(TextBox[lastTextBox]);
                        }
                    }
                    else if(helpBoxShown)
                    {
                        HideHelpBox();
                    }
                    lastSize = len;
                }
                else if(wmEvent == 512)
                {
                    lastTextBox =wmId-103;
                }
                else if(wmEvent == 256 && helpBoxShown && lastTextBox != wmId-103)
                {
                    HideHelpBox();
                }
            }
            else if(wmId >= 1000 && wmId <= 999+_BOXES)
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
