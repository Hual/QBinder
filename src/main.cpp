/******************************************************************************************************/
/*********** Main.cpp: The main source file - contains main methods, callbacks and functions **********/
/******************************************************************************************************/

#include "main.h"


/********************************* WinAPI-related methods & callbacks *********************************/
INT_PTR GetColours(HDC hdc, const BYTE type, const BYTE fontType)
{
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(configProps[fontType], configProps[fontType+1], configProps[fontType+2]));
    SetBkColor(hdc, RGB(configProps[type],configProps[type+1],configProps[type+2]));
    return (INT_PTR)brushes[((type-1)/3)-1];
}

bool RegClass(HINSTANCE&instance, LPCSTR className, const WNDPROC callback, const UINT style, const UINT iconResource, LPCSTR cursor, const UINT menu, const HBRUSH backgr)
{
	WNDCLASSEX winclass;
    winclass.hInstance = instance;
    winclass.lpszClassName = className;
    winclass.lpfnWndProc = callback;
    winclass.style = style;
    winclass.cbSize = sizeof(WNDCLASSEX);
    winclass.hIcon = LoadIcon(instance, MAKEINTRESOURCE(iconResource));
    winclass.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(iconResource));
    winclass.hCursor = LoadCursor(instance, cursor);
    winclass.lpszMenuName = MAKEINTRESOURCE(menu);
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hbrBackground = backgr;
    if (!RegisterClassEx(&winclass)) return 0;
    return 1;
}

void ShowSettWindow(const bool show)
{
    if(show)
    {
        settWindow = CreateWindowEx(0, PS_SETT, GetLangString(0), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, (GetSystemMetrics(SM_CXSCREEN) - 400)/2, (GetSystemMetrics(SM_CYSCREEN) - 500)/2, 400, 500, mainWindow, NULL, instance, NULL);
        ShowWindow(settWindow, SW_SHOW);
        EnableWindow(mainWindow, 0);
    }
    else
    {
		for(unsigned i=0;i<sizeof(settCtrl)/sizeof(HWND);++i)
        {
        	DestroyWindow(settCtrl[i]);
        }
        EnableWindow(mainWindow, 1);
        SetFocus(mainWindow);
    }
}

void ShowPageWindow(const bool show)
{
    if(show)
    {
        pageWindow = CreateWindowEx(0, PS_PAGE, (!gotoPage ? (!delPageRange ? GetLangString(1) : GetLangString(2)) :  GetLangString(3)), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, (GetSystemMetrics(SM_CXSCREEN) - 200)/2, (GetSystemMetrics(SM_CYSCREEN) - 100)/2, 200, 100, mainWindow, NULL, instance, NULL);
        ShowWindow(pageWindow, SW_SHOW);
        EnableWindow(mainWindow, 0);
    }
    else
    {
		for(unsigned i=0;i<sizeof(pageRangeCtrl)/sizeof(HWND);++i)
		{
			DestroyWindow(pageRangeCtrl[i]);
		}
        EnableWindow(mainWindow, 1);
        SetFocus(mainWindow);
    }
}

void UpdateMainWindowTitle(const unsigned short page)
{
    char version[MAX_PATH+24];
    sprintf(version, "QBinder %u.%u.%02u  -  %s %i  -  %s", VERSION/65536, (VERSION-(65536*(VERSION/65536))-(VERSION%256))/256, VERSION%256, GetLangString(4), page, (strlen(filePath) > 0 ? filePath : GetLangString(5)));
    SetWindowText(mainWindow, version);
}

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	MSG messages;
	if(!LoadConfigFile())
		if(!CreateDefaultConfigFile())
			MessageBoxEx(mainWindow, (LPCSTR)GetLangString(6), MB_WARNING, MB_OK,  MB_ICONWARNING);
	if(!LoadLanguageFile("english", langStr, mouse_string, media_string, other_tokens_desc, mouse_tokens_desc))
	{
		CreateDefaultLanguageFile();
		LoadLanguageFile("english", langStr, mouse_string, media_string, other_tokens_desc, mouse_tokens_desc);
	}
    brushes[0] = CreateSolidBrush(RGB(configProps[4],configProps[5],configProps[6]));
    brushes[1] = CreateSolidBrush(RGB(configProps[7],configProps[8],configProps[9]));
    brushes[2] = CreateSolidBrush(RGB(configProps[10],configProps[11],configProps[12]));
	HEADER_FONT=CreateFont(20,0,0,0,FW_DONTCARE,0,0,0,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,CLIP_DEFAULT_PRECIS,(configProps[22] ? ANTIALIASED_QUALITY : DEFAULT_QUALITY),VARIABLE_PITCH,TEXT("Arial")),
	instance = hThisInstance;
    keyEvent = CreateEvent(NULL, 1, 0, __TEXT("kevent"));
    CreateThread(NULL, 0, KeyArraySimThread, NULL, 0, &simThreadID);
    inputHook[0] = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    inputHook[1] = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    if(configProps[1]) ToggleVisualStyles(1, _actCookie);
    if(!RegClass(hThisInstance, PS_MAIN, MainWindowProc, CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, ID_ICON, IDC_ARROW, ID_MENU, brushes[0]))
		PostQuitMessage(0);
    if(!RegClass(hThisInstance, PS_SETT, SettWindowProc, CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, ID_ICON, IDC_ARROW, NULL, brushes[0]))
		PostQuitMessage(0);
    if(!RegClass(hThisInstance, PS_PAGE, PageWindowProc, CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, ID_ICON, IDC_ARROW, NULL, brushes[0]))
		PostQuitMessage(0);
    mainWindow = CreateWindowEx(0, PS_MAIN, NULL, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, (GetSystemMetrics(SM_CXSCREEN) - 800)/2, (GetSystemMetrics(SM_CYSCREEN) - 630)/2, 800, 630, HWND_DESKTOP, NULL, hThisInstance, NULL);
    UpdateMainWindowTitle(1);
    ShowWindow(mainWindow, nCmdShow);
    while(GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
	UnregisterClass(PS_MAIN, hThisInstance);
	UnregisterClass(PS_SETT, hThisInstance);
	UnregisterClass(PS_PAGE, hThisInstance);
    return messages.wParam;
}

LRESULT CALLBACK LowLevelMouseProc(int code, WPARAM wParam, LPARAM lParam)
{
    if(code == 0)
    {
        const int item = GetSelectedItem_LB(helpBox);
        if(wParam == WM_LBUTTONDOWN && helpBoxShown && (int)GetActiveWindow() && GetFocus() == helpBox && item > -1)
        {
            MSLLHOOKSTRUCT*mhs = (MSLLHOOKSTRUCT*)lParam;
            if((mhs->time)-lastMousePress <= 500)
                UpdateText();
            lastMousePress = mhs->time;
            lastItem = item;
        }
        else
        {
            for(unsigned i=0;i<_Button.size()*ROWS;++i)
            {
                const unsigned currPage = TabCtrl_GetCurSel(pageTab);
                const bool is_curr_page = i/ROWS == currPage;
                const int msg = (is_curr_page ? (GetSelectedItem_CB(keyComboBox[i%ROWS])) : (_Get_Item.at(i/ROWS).at(i%ROWS)));
                if(((is_curr_page && GetValue(enabled, i%ROWS)) || (!is_curr_page && GetValue(_Button.at(i/ROWS), i%ROWS))) && msg >= NORMAL_KEYS_SIZE && msg < NORMAL_KEYS_SIZE+MOUSE_BUTTONS_SIZE)
                {
                    if(wParam == mouse_value[msg-NORMAL_KEYS_SIZE])
                    {
                        if((is_curr_page && IsChecked(checkBox[0][i%ROWS])) || (!is_curr_page && GetValue(_Check_Ctrl.at(i/ROWS), i%ROWS)))
							if(!(GetKeyState(VK_CONTROL) & 0x8000))
								continue;
                        if((is_curr_page && IsChecked(checkBox[1][i%ROWS])) || (!is_curr_page && GetValue(_Check_Shift.at(i/ROWS), i%ROWS)))
							if(!(GetKeyState(VK_SHIFT) & 0x8000))
								continue;
                        if((is_curr_page && IsChecked(checkBox[2][i%ROWS])) || (!is_curr_page && GetValue(_Check_Alt.at(i/ROWS), i%ROWS)))
							if(!(GetKeyState(VK_MENU) & 0x8000))
								continue;
                        char txt[MAX_TEXT_LEN];
                        if(is_curr_page)
							GetText(textBox[i%ROWS], txt);
                        else
							strcpy(txt, _TextBox.at(i/ROWS).at(i%ROWS).c_str());
                        if(!strcmp(txt, EMPTY_TEXT) || txt == NULL)
							continue;
                        memset(keyInput, 0, sizeof(INPUT)*4);
                        __SimulateKeyArray(txt, strlen(txt));
                        if(!configProps[3])
							return -1;
                    }
                }
            }
        }
    }
    return CallNextHookEx(inputHook[1], code, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if(code == 0)
    {
        KBDLLHOOKSTRUCT*kbstr = (KBDLLHOOKSTRUCT*)lParam;
        if(kbstr->vkCode == VK_ESCAPE && helpBoxShown && (int)GetActiveWindow())
        {
            HideHelpBox();
            return -1;
        }
        else if(kbstr->vkCode == VK_RETURN && helpBoxShown && (int)GetActiveWindow() && GetFocus() == helpBox && GetSelectedItem_LB(helpBox) > -1)
        {
            UpdateText();
            return -1;
        }
        for(unsigned i=0;i<_Button.size()*ROWS;++i)
        {
            unsigned currPage = TabCtrl_GetCurSel(pageTab);
            const bool is_curr_page = i/ROWS == currPage;
            const int msg = (is_curr_page ? (GetSelectedItem_CB(keyComboBox[i%ROWS])) : (_Get_Item.at(i/ROWS).at(i%ROWS)));
            if(((is_curr_page && GetValue(enabled, i%ROWS)) || (!is_curr_page && GetValue(_Button.at(i/ROWS), i%ROWS))) && msg > -1)
            {
                if(wParam == WM_KEYDOWN)
                {
                    if((msg < NORMAL_KEYS_SIZE && kbstr->vkCode == keys_value[msg]) || (configProps[0] && msg >= NORMAL_KEYS_SIZE+MOUSE_BUTTONS_SIZE && msg < NORMAL_KEYS_SIZE+MOUSE_BUTTONS_SIZE+MEDIA_KEYS_SIZE && kbstr->vkCode == media_keys_value[msg-(NORMAL_KEYS_SIZE+MOUSE_BUTTONS_SIZE)]))
                    {
                        if((is_curr_page && IsChecked(checkBox[0][i%ROWS])) || (!is_curr_page && GetValue(_Check_Ctrl.at(i/ROWS), i%ROWS)))
                        {
                            if(!(GetKeyState(VK_CONTROL) & 0x8000))
								continue;
                        }
                        else if((GetKeyState(VK_CONTROL) & 0x8000))
							continue;
                        if((is_curr_page && IsChecked(checkBox[1][i%ROWS])) || (!is_curr_page && GetValue(_Check_Shift.at(i/ROWS), i%ROWS)))
                        {
                            if(!(GetKeyState(VK_SHIFT) & 0x8000))
								continue;
                        }
                        else if((GetKeyState(VK_SHIFT) & 0x8000))
							continue;
                        if((is_curr_page && IsChecked(checkBox[2][i%ROWS])) || (!is_curr_page && GetValue(_Check_Alt.at(i/ROWS), i%ROWS)))
                        {
                            if(!(GetKeyState(VK_MENU) & 0x8000))
								continue;
                        }
                        else if((GetKeyState(VK_MENU) & 0x8000))
							continue;
                        char txt[MAX_TEXT_LEN];
                        if(is_curr_page) GetText(textBox[i%ROWS], txt);
                        else
							strcpy(txt, _TextBox.at(i/ROWS).at(i%ROWS).c_str());
                        if(!strcmp(txt, EMPTY_TEXT) || txt == NULL)
							continue;
                        memset(keyInput, 0, sizeof(INPUT)*4);
                        keyInput[0].type = INPUT_KEYBOARD;
                        keyInput[0].ki.wVk = kbstr->vkCode;
                        keyInput[0].ki.dwFlags= KEYEVENTF_KEYUP;
                        if(SendInput(1, keyInput, sizeof(INPUT)) == 0)
							return 0;
                        __SimulateKeyArray(txt, strlen(txt));
                        if(!configProps[2])
							return -1;
                    }
                }
            }
        }
    }
    return CallNextHookEx(inputHook[0], code, wParam, lParam);
}

LRESULT CALLBACK SettWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_CREATE:
        {
        	CreateSettControls(hwnd);
        	LoadConfigIntoSettWindow();
			break;
        }
        case WM_COMMAND:
        {
        	const unsigned wmId=LOWORD(wParam);
			switch(wmId)
			{
				case ID_SETT_BUTT_ACCEPT:
				{
					if(!LoadConfigFromSettWindow())
					{
						MessageBoxEx(hwnd, (LPCSTR)GetLangString(7), MB_ERROR, MB_OK, MB_ICONERROR);
						break;
					}
					MessageBoxEx(hwnd, (LPCSTR)GetLangString(8), MB_INFO, MB_OK, MB_ICONWARNING);
					DestroyWindow(hwnd);
					break;
				}
				case ID_SETT_BUTT_DEFAULT:
				{
					ResetSettWindowToDefault();
					break;
				}
			}
			break;
        }
        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORBTN:
        {
            return GetColours((HDC)wParam, COL_TYPE_STATIC, FNT_TYPE_STATIC);
        }
        case WM_CTLCOLOREDIT:
        {
        	return GetColours((HDC)wParam, COL_TYPE_EDIT, FNT_TYPE_EDIT);
        }
        case WM_DESTROY:
        {
            ShowSettWindow(0);
			break;
        }
        default:
        {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }
    return 0;
}

LRESULT CALLBACK PageWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_CREATE:
        {
			CreatePageControls(hwnd);
			break;
        }
        case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORBTN:
        {
            return GetColours((HDC)wParam, COL_TYPE_STATIC, FNT_TYPE_STATIC);
        }
        case WM_CTLCOLOREDIT:
        {
        	return GetColours((HDC)wParam, COL_TYPE_EDIT, FNT_TYPE_EDIT);
        }
        case WM_COMMAND:
        {
			if(LOWORD(wParam) == ID_PAGE_BUTTON_OK)
			{
				char buf[6];
				if(!delPageRange)
				{
					GetText(pageRangeCtrl[1], buf);
					const unsigned short _pages = atoi(buf);
					if(!gotoPage)
					{
						if(_pages != 0 && (pages+_pages) < (unsigned short)~0)
						{
							if(_pages > 100 && MessageBoxEx(hwnd, (LPCSTR)GetLangString(9), MB_WARNING, MB_YESNO, MB_ICONWARNING) == 7) break;
							for(unsigned i=0;i<_pages;++i)
								AddNewPage();
							DestroyWindow(pageWindow);
						}
						else
							MessageBoxEx(hwnd, (LPCSTR)GetLangString(10), MB_ERROR, MB_OK, MB_ICONERROR);
					}
					else
					{
						if(_pages > 0 && _pages <= pages)
						{
							DestroyWindow(pageWindow);
							StoreContents(TabCtrl_GetCurSel(pageTab));
							LoadContents(_pages-1);
							TabCtrl_SetCurSel(pageTab, _pages-1);
							UpdateMainWindowTitle(_pages);
						}
						else
							MessageBoxEx(hwnd, (LPCSTR)GetLangString(11), MB_ERROR, MB_OK, MB_ICONERROR);
					}
					break;
				}
				GetText(pageRangeCtrl[3], buf);
				const unsigned short lower_b = atoi(buf);
				GetText(pageRangeCtrl[4], buf);
				unsigned short upper_b = atoi(buf);
				if(lower_b < 2 || upper_b > pages || lower_b > upper_b)
				{
					MessageBoxEx(hwnd, (LPCSTR)GetLangString(12), MB_ERROR, MB_OK, MB_ICONERROR);
					break;
				}
				for(upper_b=upper_b-1;upper_b>=lower_b-1;--upper_b)
				{
					_DestroyPage(mainWindow, upper_b);
				}
				DestroyWindow(hwnd);
			}
			break;
        }
        case WM_DESTROY:
        {
			ShowPageWindow(0);
			break;
        }
        default:
        {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }
    return 0;
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    const unsigned wmId=LOWORD(wParam), wmEvent=HIWORD(wParam);
    switch(message)
    {
        case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORBTN:
        {
            return GetColours((HDC)wParam, COL_TYPE_STATIC, FNT_TYPE_STATIC);
        }
		case WM_CTLCOLOREDIT:
        {
        	return GetColours((HDC)wParam, COL_TYPE_EDIT, FNT_TYPE_EDIT);
        }
		case WM_CTLCOLORLISTBOX:
        {
        	return GetColours((HDC)wParam, COL_TYPE_LISTBOX, FNT_TYPE_LISTBOX);
        }
        case WM_NOTIFY:
        {
            NMHDR*eInfo = (NMHDR*)lParam;
            if(eInfo->hwndFrom == pageTab)
            {
                switch(eInfo->code)
                {
                    case TCN_SELCHANGE:
                    {
                        const int page = TabCtrl_GetCurSel(pageTab);
                        LoadContents(page);
                        UpdateMainWindowTitle(page+1);
						break;
                    }
                    case TCN_SELCHANGING:
                    {
                        StoreContents(TabCtrl_GetCurSel(pageTab));
                    }
                }
            }
			break;
        }
        case WM_CREATE:
        {
            CreateMainControls(hwnd);
			break;
        }
        case WM_COMMAND:
        {
            if(wmId >= ID_MAIN_EDIT && wmId < ID_MAIN_EDIT+16)
            {
                switch(wmEvent)
                {
                    case 768:
                    {
                        ShowHelpBox(wmId);
                        break;
                    }
                    case 512:
                    {
                        lastTextBox = wmId-103;
                        break;
                    }
                    case 256:
                    {
                        if(helpBoxShown && lastTextBox != wmId-103)
							HideHelpBox();
                        _lastTextBox = wmId-103;
                        break;
                    }
                }
            }
            else if(wmId >= ID_BUTTON && wmId <= (ID_BUTTON-1)+ROWS)
            {
                if(GetValue(enabled, wmId-ID_BUTTON))
                {
                	SetButtonDisabled(wmId-ID_BUTTON);
                }
                else
                {
                	SetButtonEnabled(wmId-ID_BUTTON);
                }
            }
			switch (wmId)
            {
                case ID_FILE_EXIT:
                {
                    DestroyWindow(hwnd);
                    break;
                }
                case ID_EDIT_NEWP:
                {
                    AddNewPage();
					break;
                }
                case ID_EDIT_DELP:
                {
                    _DestroyPage(hwnd, TabCtrl_GetCurSel(pageTab));
					break;
                }
                case ID_EDIT_RANGE:
                {
                	delPageRange = 0;
                	gotoPage = 0;
                	ShowPageWindow(1);
					break;
				}
				case ID_EDIT_DELRANGE:
				{
					gotoPage = 0;
					delPageRange = 1;
					ShowPageWindow(1);
					break;
				}
                case ID_EDIT_GOTO:
                {
                	delPageRange = 0;
					gotoPage = 1;
                	ShowPageWindow(1);
					break;
                }
                case ID_FILE_SAVE:
                {
					if(strlen(filePath) == 0)
                	{
						OPENFILENAME sfd;
						char buf[MAX_PATH];
						ZeroMemory(&sfd, sizeof(sfd));
						sfd.lStructSize = sizeof(sfd);
						sfd.hwndOwner = NULL;
						sfd.lpstrFile = buf;
						sfd.lpstrFile[0] = '\0';
						sfd.nMaxFile = sizeof(buf);
						sfd.lpstrFilter = "QBinder Project Files (*.qbp)\0*.QBP\0";
						sfd.nFilterIndex =1;
						sfd.lpstrFileTitle = NULL;
						sfd.nMaxFileTitle = 0;
						sfd.lpstrInitialDir=NULL;
						sfd.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLEHOOK;
						if (GetSaveFileName(&sfd) != true)
							MessageBoxEx(hwnd, (LPCSTR)GetLangString(51), MB_ERROR, MB_OK, MB_ICONWARNING);
						else
							SaveProjectFile(buf);
                	}
                	else
						SaveProjectFile(filePath);
					break;
                }
                case ID_FILE_SAVE_AS:
                {
					OPENFILENAME sfd;
					char buf[MAX_PATH];
					ZeroMemory(&sfd, sizeof(sfd));
					sfd.lStructSize = sizeof(sfd);
					sfd.hwndOwner = NULL;
					sfd.lpstrFile = buf;
					sfd.lpstrFile[0] = '\0';
					sfd.nMaxFile = sizeof(buf);
					sfd.lpstrFilter = "QBinder Project Files (*.qbp)\0*.QBP\0";
					sfd.nFilterIndex = 1;
					sfd.lpstrFileTitle = NULL;
					sfd.nMaxFileTitle = 0;
					sfd.lpstrInitialDir=NULL;
					sfd.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLEHOOK;
					if (GetSaveFileName(&sfd) != true)
						MessageBoxEx(hwnd, (LPCSTR)GetLangString(51), MB_ERROR, MB_OK, MB_ICONWARNING);
					else
						SaveProjectFile(buf);
	                break;
                }
                case ID_FILE_LOAD:
                {
					OPENFILENAME ofd;
					char buf[MAX_PATH];
					ZeroMemory(&ofd, sizeof(ofd));
					ofd.lStructSize = sizeof(ofd);
					ofd.hwndOwner = NULL;
					ofd.lpstrFile = buf;
					ofd.lpstrFile[0] = '\0';
					ofd.nMaxFile = sizeof(buf);
					ofd.lpstrFilter = "QBinder Project Files (*.qbp)\0*.QBP\0";
					ofd.nFilterIndex = 1;
					ofd.lpstrFileTitle = NULL;
					ofd.nMaxFileTitle = 0;
					ofd.lpstrInitialDir=NULL;
					ofd.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
					if (GetOpenFileName(&ofd) != true)
						MessageBoxEx(hwnd, (LPCSTR)GetLangString(51), MB_ERROR, MB_OK, MB_ICONERROR);
                    else
						LoadProjectFile(buf);
	                break;
                }
                case ID_FILE_NEW:
                {
                    ResetPages();
					break;
                }
                case ID_TOOLS_SETTINGS:
                {
                    ShowSettWindow(1);
                    break;
                }
                default:
                    return DefWindowProc(hwnd, message, wParam, lParam);
            }
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        default:
        {
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }
    return 0;
}
/************************************ End of WinAPI-related code **************************************/


/************************************* Key-simulation-related code ************************************/
DWORD WINAPI KeyArraySimThread(LPVOID lpParam)
{
    while(1)
    {
        while(WaitForSingleObjectEx(keyEvent, INFINITE, 1) != WAIT_OBJECT_0) Sleep(10);
        SimulateKeyArray(eventStr, eventLen, keyInput);
    }
    ExitThread(0);
    return 0;
}

void SimulateKeyArray(const char __arr[], const size_t __len, INPUT*__keyInput = keyInput)
{
    memset(__keyInput, 0, sizeof(INPUT)*4);
    __keyInput[0].type = INPUT_KEYBOARD;
    __keyInput[0].ki.wVk = VK_MENU;
    __keyInput[0].ki.dwFlags= KEYEVENTF_KEYUP;
    __keyInput[1].type = INPUT_KEYBOARD;
    __keyInput[1].ki.wVk = VK_CONTROL;
    __keyInput[1].ki.dwFlags= KEYEVENTF_KEYUP;
    __keyInput[2].type = INPUT_KEYBOARD;
    __keyInput[2].ki.wVk = VK_SHIFT;
    __keyInput[2].ki.dwFlags= KEYEVENTF_KEYUP;
    if(SendInput(3, __keyInput, sizeof(INPUT)) == 0) return;
    for(unsigned i=0;i<__len;++i)
    {
        bool should_continue = 0;
        if(i < __len-1 && __arr[i] == 37 && __arr[++i] != 37)
        {
            bool is_long = 0;
            if( i < __len-1)
            {
                if(__arr[i] == 61)
                {
                    ++i;
                    is_long = 1;
                    for(unsigned j=0;j<TOTAL_HOLD_TOKENS;++j)
                    {
                        if(__arr[i] == hold_tokens[j][0])
                        {
                            memset(__keyInput, 0, sizeof(INPUT)*4);
                            __keyInput[0].type = INPUT_KEYBOARD;
                            __keyInput[0].ki.wVk = hold_tokens[j][1];
                            __keyInput[0].ki.dwFlags = 0;
                            if(SendInput(1, __keyInput, sizeof(INPUT)) == 0)
								return;
                            break;
                        }
                    }
                    should_continue = 1;
                }
                else if(__arr[i] == 33)
                {
                    ++i;
                    is_long = 1;
                    for(unsigned j=0;j<TOTAL_HOLD_TOKENS;++j)
                    {
                        if(__arr[i] == hold_tokens[j][0])
                        {
                            memset(__keyInput, 0, sizeof(INPUT)*4);
                            __keyInput[0].type = INPUT_KEYBOARD;
                            __keyInput[0].ki.wVk = hold_tokens[j][1];
                            __keyInput[0].ki.dwFlags= KEYEVENTF_KEYUP;
                            if(SendInput(1, __keyInput, sizeof(INPUT)) == 0)
								return;
                            break;
                        }
                    }
                    should_continue = 1;
                }
            }
            if(!is_long)
            {
                switch(__arr[i])
                {
                    case 114:
                    {
                        memset(__keyInput, 0, sizeof(INPUT)*4);
                        const int __r = random(0x01, 0xFE);
                        __keyInput[0].type = INPUT_KEYBOARD;
                        __keyInput[0].ki.wVk = __r;
                        __keyInput[0].ki.dwFlags= 0;
                        __keyInput[1].type = INPUT_KEYBOARD;
                        __keyInput[1].ki.wVk = __r;
                        __keyInput[1].ki.dwFlags= KEYEVENTF_KEYUP;
                        if(SendInput(2, __keyInput, sizeof(INPUT)) == 0)
							return;
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
                                memset(__keyInput, 0, sizeof(INPUT)*4);
                                const char hex[3] = { __arr[i], __arr[++i], 0 };
                                __keyInput[0].type = INPUT_KEYBOARD;
                                __keyInput[0].ki.wVk = (WORD)strtol(hex, NULL, 16);
                                __keyInput[0].ki.dwFlags= 0;
                                __keyInput[1].type = INPUT_KEYBOARD;
                                __keyInput[1].ki.wVk = (WORD)strtol(hex, NULL, 16);
                                __keyInput[1].ki.dwFlags= KEYEVENTF_KEYUP;
                                if(SendInput(2, __keyInput, sizeof(INPUT)) == 0)
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
                        if(__arr[i] >= 48 && __arr[i] <= 57)
							key[0] = __arr[i];
                        else
                        {
                            break;
                        }
                        ++i;
                        if(__arr[i] >= 48 && __arr[i] <= 57)
							key[1] = __arr[i];
                        ++i;
                        k = atoi(key);
                        if(k > 0 && k < 25)
                        {
                            memset(__keyInput, 0, sizeof(INPUT)*4);
                            __keyInput[0].type = INPUT_KEYBOARD;
                            __keyInput[0].ki.wVk = 0x6F+k;
                            __keyInput[0].ki.dwFlags= 0;
                            __keyInput[1].type = INPUT_KEYBOARD;
                            __keyInput[1].ki.wVk = 0x6F+k;
                            __keyInput[1].ki.dwFlags= KEYEVENTF_KEYUP;
                            if(SendInput(2, __keyInput, sizeof(INPUT)) == 0)
								return;
                        }
                        break;
                    }
                    case 67:
                    {
                        bool should_break = 0;
                        ++i;
                        char key[5] = {0, 0, 0, 0, 0 };
                        int k = 0;
                        for(unsigned j=0;j<sizeof(key)-1;++j)
                        {
                            if(__arr[i] >= 48 && __arr[i] <= 57) key[j] = __arr[i];
                            else
                            {
                                should_break = 1;
                                break;
                            }
                            ++i;
                        }
                        if(should_break)
							break;
                        k = atoi(key);
                        Sleep(k*1000);
                        break;
                    }
                    case 84:
                    {
                        char tym[16];
                        GetSystemTime(tym, 0);
                        SimulateKeyArray(tym, strlen(tym));
                        ++i;
                        break;
                    }
                    case 74:
                    {
                        char tym[16];
                        GetSystemTime(tym, 1);
                        SimulateKeyArray(tym, strlen(tym));
                        ++i;
                        break;
                    }
                    default:
                    {
                        for(unsigned j=0;j<TOTAL_TOKENS;++j)
                        {
                            if(__arr[i] == tokens[j][0])
                            {
                                memset(__keyInput, 0, sizeof(INPUT)*4);
                                __keyInput[0].type = INPUT_KEYBOARD;
                                __keyInput[0].ki.wVk = tokens[j][1];
                                __keyInput[0].ki.dwFlags= 0;
                                __keyInput[1].type = INPUT_KEYBOARD;
                                __keyInput[1].ki.wVk = tokens[j][1];
                                __keyInput[1].ki.dwFlags= KEYEVENTF_KEYUP;
                                if(SendInput(2, __keyInput, sizeof(INPUT)) == 0)
									return;
                                break;
                            }
                        }
                        for(unsigned j=0;j<TOTAL_MOUSE_TOKENS;++j)
                        {
                            if(__arr[i] == mouse_tokens[j][0])
                            {
                                memset(__keyInput, 0, sizeof(INPUT)*4);
                                __keyInput[0].type = INPUT_MOUSE;
                                __keyInput[0].mi.dwFlags = mouse_tokens[j][1];
                                __keyInput[1].type = INPUT_MOUSE;
                                __keyInput[1].mi.dwFlags = mouse_tokens[j][2];
                                if(SendInput(2, __keyInput, sizeof(INPUT)) == 0)
									return;
                                break;
                            }
                        }
                        should_continue = 1;
                    }
                }
            }
        }
        if(should_continue)
			continue;
        const short __vk_shrt = VkKeyScanEx(__arr[i], GetKeyboardLayout(0));
        memset(__keyInput, 0, sizeof(INPUT)*4);
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
            if(SendInput(4, __keyInput, sizeof(INPUT)) == 0)
				return;
        }
        else
        {
            __keyInput[0].type = INPUT_KEYBOARD;
            __keyInput[0].ki.wVk = __vk_shrt & 0xff;
            __keyInput[0].ki.dwFlags = 0;
            __keyInput[1].type = INPUT_KEYBOARD;
            __keyInput[1].ki.wVk = __vk_shrt & 0xff;
            __keyInput[1].ki.dwFlags = KEYEVENTF_KEYUP;
            if(SendInput(2, __keyInput, sizeof(INPUT)) == 0)
				return;
        }
    }
}

void __SimulateKeyArray(const char __arr[], const size_t __len)
{
    eventLen = __len;
    strcpy(eventStr, __arr);
    PulseEvent(keyEvent);
}
/********************************* End of key-simulation-related code *********************************/


/**************************************** Control-related code ****************************************/
void CreateMainControls(HWND&hwnd)
{
	helpBox = CreateWindowEx(0, WC_LISTBOX, NULL, WS_CHILD | WS_VSCROLL | WS_BORDER, 0,0,244,100, hwnd, (HMENU)ID_HELPBOX, NULL, NULL);
    SetFont(helpBox, DEFAULT_FONT);
    if(configProps[23])
		CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 1, 50, 795, 1, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    for(unsigned i=0;i<ROWS;++i)
    {
        textBox[i] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 10, (i*30)+56, 100+TEXTBOX_SIZE, 20, hwnd, (HMENU)(ID_MAIN_EDIT+i), GetModuleHandle(NULL), NULL);
        SetFont(textBox[i], DEFAULT_FONT);
        SetTextLimit(textBox[i], MAX_TEXT_LEN);
        if(configProps[23])
			CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, (i == ROWS-1 ? 1 : 10), (i*30)+80, (i == ROWS-1 ? 795 : 772), 1, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
        button[i] = CreateWindowEx(0, WC_BUTTON, GetLangString(49), WS_CHILD | WS_VISIBLE, 645-24+TEXTBOX_SIZE-(WHITESPACE_SIZE*4)+5, (i*30)+54, 80, 23, hwnd, (HMENU)(ID_BUTTON+i), GetModuleHandle(NULL), NULL);
        SetFont(button[i], DEFAULT_FONT);
        checkBox[0][i] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 200-24+TEXTBOX_SIZE-WHITESPACE_SIZE, (i*30)+56, 15, 20, hwnd, (HMENU)(ID_CHECKBOX_CTRL+i), GetModuleHandle(NULL), NULL);
        checkBox[1][i] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 305-25+TEXTBOX_SIZE-(WHITESPACE_SIZE*2), (i*30)+56, 15, 20, hwnd, (HMENU)(ID_CHECKBOX_SHIFT+i), GetModuleHandle(NULL), NULL);
        checkBox[2][i] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 410-25+TEXTBOX_SIZE-(WHITESPACE_SIZE*3), (i*30)+56, 15, 20, hwnd, (HMENU)(ID_CHECKBOX_ALT+i), GetModuleHandle(NULL), NULL);
        keyComboBox[i] = CreateWindowEx(0, WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_TABSTOP | WS_VSCROLL, 515-24-36+8+5+TEXTBOX_SIZE-(WHITESPACE_SIZE*4), (i*30)+55, 146, 335, hwnd, (HMENU)ID_MAIN_COMBO, GetModuleHandle(NULL), NULL);
        SetFont(keyComboBox[i], DEFAULT_FONT);
        unsigned j;
        for(j=0;j<NORMAL_KEYS_SIZE;++j)
			AddString_CB(keyComboBox[i], keys_string[j]);
        for(j=0;j<MOUSE_BUTTONS_SIZE;++j)
			AddString_CB(keyComboBox[i], mouse_string[j]);
        if(configProps[0])
			for(j=0;j<MEDIA_KEYS_SIZE;++j)
				AddString_CB(keyComboBox[i], media_string[j]);

    }
    pageTab = CreateWindowEx(0, WC_TABCONTROL, NULL, WS_CHILD | WS_VISIBLE, -1, 550, 798, 35, hwnd, (HMENU)ID_MAIN_PAGE, GetModuleHandle(NULL), NULL);
    SetFont(pageTab, DEFAULT_FONT);
    if(!configProps[23])
		CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 1, 42, 795, 1, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 0, 548, 797, 1, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 255, 0, 1, (configProps[23] ? 532 : 550), hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 345, 0, 1, (configProps[23] ? 532 : 550), hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 435, 0, 1, (configProps[23] ? 532 : 550), hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 525, 0, 1, (configProps[23] ? 532 : 550), hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    const BYTE tmp = (configProps[23] ? 15 : 12);
    HWND lbl = CreateWindowEx(0, WC_STATIC, GetLangString(13), WS_CHILD | WS_VISIBLE, atoi(GetLangString(14)), tmp, 150, 20, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    SetFont(lbl, HEADER_FONT);
    lbl = CreateWindowEx(0, WC_STATIC, "Control", WS_CHILD | WS_VISIBLE,274, tmp, 60, 20, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    SetFont(lbl, HEADER_FONT);
    lbl = CreateWindowEx(0, WC_STATIC, "Shift", WS_CHILD | WS_VISIBLE, 373, tmp, 60, 20, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    SetFont(lbl, HEADER_FONT);
    lbl = CreateWindowEx(0, WC_STATIC, "Alt", WS_CHILD | WS_VISIBLE, 471, tmp, 20, 20, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    SetFont(lbl, HEADER_FONT);
    lbl = CreateWindowEx(0, WC_STATIC, GetLangString(15), WS_CHILD | WS_VISIBLE, atoi(GetLangString(16)), tmp, 300, 20, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
    SetFont(lbl, HEADER_FONT);
    AddNewPage();
}

void CreateSettControls(HWND&hwnd)
{
	settCtrl[0] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 4, 15, 20, hwnd, (HMENU)ID_SETT_CHECK, GetModuleHandle(NULL), NULL);
	settCtrl[1] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 24, 15, 20, hwnd, (HMENU)(ID_SETT_CHECK+1), GetModuleHandle(NULL), NULL);
	settCtrl[2] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 44, 15, 20, hwnd, (HMENU)(ID_SETT_CHECK+2), GetModuleHandle(NULL), NULL);
	settCtrl[3] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 64, 15, 20, hwnd, (HMENU)(ID_SETT_CHECK+3), GetModuleHandle(NULL), NULL);
	settCtrl[22] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 84, 15, 20, hwnd, (HMENU)(ID_SETT_CHECK+4), GetModuleHandle(NULL), NULL);
	settCtrl[23] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 104, 15, 20, hwnd, (HMENU)(ID_SETT_CHECK+5), GetModuleHandle(NULL), NULL);
	for(unsigned i=0;i<6;++i)
	{
		BYTE id = (3*(i+1));
		settCtrl[20+id+1] = CreateWindowEx(0, WC_STATIC, GetLangString(17), WS_CHILD | WS_VISIBLE, SETT_RGB_X, 155+(SETT_RGB_Y_SPACE*i), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(settCtrl[20+id+1], DEFAULT_FONT);
		settCtrl[id+1] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, SETT_RGB_X+15, 153+(SETT_RGB_Y_SPACE*i), 40, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(settCtrl[id+1], DEFAULT_FONT);
		SetTextLimit(settCtrl[id+1], 3);
		settCtrl[20+id+2] = CreateWindowEx(0, WC_STATIC, GetLangString(18), WS_CHILD | WS_VISIBLE, SETT_RGB_X+73, 155+(SETT_RGB_Y_SPACE*i), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(settCtrl[20+id+2], DEFAULT_FONT);
		settCtrl[id+2] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, SETT_RGB_X+88, 153+(SETT_RGB_Y_SPACE*i), 40, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(settCtrl[id+2], DEFAULT_FONT);
		SetTextLimit(settCtrl[id+2], 3);
		settCtrl[20+id+3] = CreateWindowEx(0, WC_STATIC, GetLangString(19), WS_CHILD | WS_VISIBLE, SETT_RGB_X+146, 155+(SETT_RGB_Y_SPACE*i), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(settCtrl[20+id+3], DEFAULT_FONT);
		settCtrl[id+3] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, SETT_RGB_X+161, 153+(SETT_RGB_Y_SPACE*i), 40, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(settCtrl[id+3], DEFAULT_FONT);
		SetTextLimit(settCtrl[id+3], 3);
	}
	settCtrl[42] = CreateWindowEx(0, WC_STATIC, GetLangString(20), WS_CHILD | WS_VISIBLE, 37, 7, 300, 20, hwnd, (HMENU)ID_SETT_CHECK, GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[42], DEFAULT_FONT);
	settCtrl[44] = CreateWindowEx(0, WC_STATIC, GetLangString(22), WS_CHILD | WS_VISIBLE, 37, 27, 300, 20, hwnd, (HMENU)(ID_SETT_CHECK+1), GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[44], DEFAULT_FONT);
	settCtrl[45] = CreateWindowEx(0, WC_STATIC, GetLangString(23), WS_CHILD | WS_VISIBLE, 37, 47, 300, 20, hwnd, (HMENU)(ID_SETT_CHECK+2), GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[45], DEFAULT_FONT);
	settCtrl[46] = CreateWindowEx(0, WC_STATIC, GetLangString(24), WS_CHILD | WS_VISIBLE, 37, 67, 300, 20, hwnd, (HMENU)(ID_SETT_CHECK+3), GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[46], DEFAULT_FONT);
	settCtrl[43] = CreateWindowEx(0, WC_STATIC, GetLangString(21), WS_CHILD | WS_VISIBLE, 37, 87, 300, 20, hwnd, (HMENU)(ID_SETT_CHECK+4), GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[43], DEFAULT_FONT);
	settCtrl[47] = CreateWindowEx(0, WC_STATIC, GetLangString(25), WS_CHILD | WS_VISIBLE, 37, 107, 300, 20, hwnd, (HMENU)(ID_SETT_CHECK+5), GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[47], DEFAULT_FONT);
	settCtrl[48] = CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 5, 126, 386, 1, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	settCtrl[49] = CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 5, 435, 386, 1, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	settCtrl[50] = CreateWindowEx(0, WC_STATIC, GetLangString(26), WS_CHILD | WS_VISIBLE, atoi(GetLangString(27)), 133, 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[50], SETT_FONT);
	settCtrl[51] = CreateWindowEx(0, WC_STATIC, GetLangString(28), WS_CHILD | WS_VISIBLE, atoi(GetLangString(29)), 133+SETT_RGB_Y_SPACE, 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[51], SETT_FONT);
	settCtrl[52] = CreateWindowEx(0, WC_STATIC, GetLangString(30), WS_CHILD | WS_VISIBLE, atoi(GetLangString(31)), 133+(SETT_RGB_Y_SPACE*2), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[52], SETT_FONT);
	settCtrl[53] = CreateWindowEx(0, WC_STATIC, GetLangString(32), WS_CHILD | WS_VISIBLE, atoi(GetLangString(33)), 133+(SETT_RGB_Y_SPACE*3), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[53], SETT_FONT);
	settCtrl[54] = CreateWindowEx(0, WC_STATIC, GetLangString(34), WS_CHILD | WS_VISIBLE, atoi(GetLangString(35)), 133+(SETT_RGB_Y_SPACE*4), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[54], SETT_FONT);
	settCtrl[55] = CreateWindowEx(0, WC_STATIC, GetLangString(36), WS_CHILD | WS_VISIBLE, atoi(GetLangString(37)), 133+(SETT_RGB_Y_SPACE*5), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[55], SETT_FONT);
	settCtrl[56] = CreateWindowEx(0, WC_BUTTON, GetLangString(38), WS_CHILD | WS_VISIBLE, 319, 442, 70, 23, hwnd, (HMENU)ID_SETT_BUTT_ACCEPT, GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[56], DEFAULT_FONT);
	settCtrl[57] = CreateWindowEx(0, WC_BUTTON, GetLangString(39), WS_CHILD | WS_VISIBLE, 4, 442, 70, 23, hwnd, (HMENU)ID_SETT_BUTT_DEFAULT, GetModuleHandle(NULL), NULL);
	SetFont(settCtrl[57], DEFAULT_FONT);
}

void CreatePageControls(HWND&hwnd)
{
	pageRangeCtrl[2] = CreateWindowEx(0, WC_BUTTON, GetLangString(40), WS_CHILD | WS_VISIBLE, 63, 45, 70, 23, hwnd, (HMENU)ID_PAGE_BUTTON_OK, GetModuleHandle(NULL), NULL);
	SetFont(pageRangeCtrl[2], DEFAULT_FONT);
	if(!delPageRange)
	{
		pageRangeCtrl[0] = CreateWindowEx(0, WC_STATIC, (gotoPage ? GetLangString(41) : GetLangString(42)), WS_CHILD | WS_VISIBLE, 16, 15, 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(pageRangeCtrl[0], DEFAULT_FONT);
		pageRangeCtrl[1] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, 95, 13, 85, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetTextLimit(pageRangeCtrl[1], 5);
		SetFont(pageRangeCtrl[1], DEFAULT_FONT);
		return;
	}
	pageRangeCtrl[0] = CreateWindowEx(0, WC_STATIC, GetLangString(43), WS_CHILD | WS_VISIBLE, 7, 15, 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(pageRangeCtrl[0], DEFAULT_FONT);
	pageRangeCtrl[1] = CreateWindowEx(0, WC_STATIC, "-", WS_CHILD | WS_VISIBLE, 137, 15, 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(pageRangeCtrl[1], DEFAULT_FONT);
	pageRangeCtrl[3] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, 91, 13, 40, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetTextLimit(pageRangeCtrl[3], 5);
	SetFont(pageRangeCtrl[3], DEFAULT_FONT);
	pageRangeCtrl[4] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, 146, 13, 40, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetTextLimit(pageRangeCtrl[4], 5);
	SetFont(pageRangeCtrl[4], DEFAULT_FONT);
}
/************************************* End of control-related code ************************************/


/****************************************** Page-related code *****************************************/
void AddNewPage(void)
{
    std::vector<std::string> new_str;
    std::vector<char> new_short;
    char buf[12];
    TC_ITEM new_item;
    ++pages;
    sprintf(buf, "Page %i", pages);
    new_item.pszText = (LPSTR)buf;
    new_item.cchTextMax = strlen(buf);
    new_item.mask = TCIF_TEXT;
    for(unsigned i=0;i<ROWS;++i)
    {
        new_str.push_back(EMPTY_TEXT);
        new_short.push_back(-1);
    }
    _TextBox.push_back(new_str);
    _Button.push_back(0);
    _Check_Ctrl.push_back(0);
    _Check_Shift.push_back(0);
    _Check_Alt.push_back(0);
    _sTextBoxIndex.push_back(0);
    _Get_Item.push_back(new_short);
    InsertItem(pageTab, pages-1, new_item);
    TabCtrl_SetCurSel(pageTab, pages);
    new_short.clear();
    new_str.clear();
}

void DestroyPage(const unsigned short page)
{
    --pages;
    TabCtrl_DeleteItem(pageTab, page);
    _TextBox.erase(_TextBox.begin()+page);
    _Button.erase(_Button.begin()+page);
    _Check_Ctrl.erase(_Check_Ctrl.begin()+page);
    _Check_Shift.erase(_Check_Shift.begin()+page);
    _Check_Alt.erase(_Check_Alt.begin()+page);
    _Get_Item.erase(_Get_Item.begin()+page);
    _sTextBoxIndex.erase(_sTextBoxIndex.begin()+page);
    const unsigned int size = TabCtrl_GetItemCount(pageTab);
    unsigned i=0;
    while(i<size)
    {
        char buf[12];
        TC_ITEM new_item;
        sprintf(buf, "Page %i", ++i);
        new_item.pszText = (LPSTR)buf;
        new_item.cchTextMax = strlen(buf);
        new_item.mask = TCIF_TEXT;
        ChangeItem(pageTab, i-1, new_item);
    }
}

void _DestroyPage(HWND&hwnd, unsigned short sel)
{
    if(sel == 0)
    {
		MessageBoxEx(hwnd, (LPCSTR)GetLangString(44), MB_ERROR, MB_OK, MB_ICONINFORMATION);
        return;
    }
    DestroyPage(sel);
    if(sel >= TabCtrl_GetItemCount(pageTab))
		sel = TabCtrl_GetItemCount(pageTab)-1;
    TabCtrl_SetCurSel(pageTab, sel);
    for(unsigned i=0;i<ROWS;++i)
    {
        SetText(textBox[i], (_TextBox.at(sel)).at(i).c_str());
        SetSelectedItem_CB(keyComboBox[i], (_Get_Item.at(sel)).at(i));
        ToggleChecked(checkBox[0][i], GetValue(_Check_Ctrl.at(sel), i));
        ToggleChecked(checkBox[1][i], GetValue(_Check_Shift.at(sel), i));
        ToggleChecked(checkBox[2][i], GetValue(_Check_Alt.at(sel), i));
        if(GetValue(_Button.at(sel), i))
        {
        	SetButtonEnabled(i);
        }
        else
        {
			SetButtonDisabled(i);
        }
    }
    SetFocus(textBox[_sTextBoxIndex.at(sel)]);
}

void ResetPages(void)
{
    while(TabCtrl_GetItemCount(pageTab) != 0) DestroyPage(0);
    AddNewPage();
    for(unsigned i=0;i<ROWS;++i)
    {
        SetText(textBox[i], EMPTY_TEXT);
        SetSelectedItem_CB(keyComboBox[i], -1);
        ToggleChecked(checkBox[0][i], 0);
        ToggleChecked(checkBox[1][i], 0);
        ToggleChecked(checkBox[2][i], 0);
        SetButtonDisabled(i);
    }
    SetFocus(textBox[_sTextBoxIndex.at(0)]);
    strset(filePath, 0);
    UpdateMainWindowTitle(1);
}

void StoreContents(const int page)
{
    std::vector<std::string> new_str;
    std::vector<char> new_short;
    bool_16 new_bool[4];
    for(unsigned i=0;i<ROWS;++i)
    {
        char buf[MAX_TEXT_LEN];
        GetText(textBox[i], buf);
        new_str.push_back((std::string)buf);
        new_short.push_back((short)GetSelectedItem_CB(keyComboBox[i]));
        if(GetValue(enabled, i))
			SetTrue(new_bool[0], i);
        else
			SetFalse(new_bool[0], i);
        if(IsChecked(checkBox[0][i]))
			SetTrue(new_bool[1], i);
        else
			SetFalse(new_bool[1], i);
        if(IsChecked(checkBox[1][i]))
			SetTrue(new_bool[2], i);
        else
			SetFalse(new_bool[2], i);
        if(IsChecked(checkBox[2][i]))
			SetTrue(new_bool[3], i);
        else
			SetFalse(new_bool[3], i);
    }
    _sTextBoxIndex.at(page) = _lastTextBox;
    _TextBox.at(page) = new_str;
    _Button.at(page) = new_bool[0];
    _Check_Ctrl.at(page) = new_bool[1];
    _Check_Shift.at(page) = new_bool[2];
    _Check_Alt.at(page) = new_bool[3];
    _Get_Item.at(page) = new_short;
    new_short.clear();
    new_str.clear();
}

void LoadContents(const int page)
{
    for(unsigned i=0;i<ROWS;++i)
    {
        SetText(textBox[i], (_TextBox.at(page)).at(i).c_str());
        SetSelectedItem_CB(keyComboBox[i], (_Get_Item.at(page)).at(i));
        ToggleChecked(checkBox[0][i], GetValue(_Check_Ctrl.at(page), i));
        ToggleChecked(checkBox[1][i], GetValue(_Check_Shift.at(page), i));
        ToggleChecked(checkBox[2][i], GetValue(_Check_Alt.at(page), i));
        if(GetValue(_Button.at(page), i))
        {
        	SetButtonEnabled(i);
        }
        else
        {
        	SetButtonDisabled(i);
        }
    }
    SetFocus(textBox[_sTextBoxIndex.at(page)]);
    _lastTextBox = _sTextBoxIndex.at(page);
    SetCaret(textBox[_lastTextBox], _TextBox.at(page).at(_lastTextBox).length());
}
/************************************** End of page-related code **************************************/


/*************************************** Help-box-related code ****************************************/
void ShowHelpBox(const unsigned id)
{
    char buf[MAX_TEXT_LEN];
    const int len = GetText(textBox[id-103], buf);
    if((len > lastSize && lastTextBox == id-103) || lastTextBox != id-103)
    {
        if(buf[strlen(buf)-1] == 37)
        {
            if(strlen(buf) > 1 && buf[strlen(buf)-2] == 37 && helpBoxShown) HideHelpBox();
            else
            {
            	unsigned i;
                ClearItems_LB(helpBox);
                for(i=0;i<TOTAL_TOKENS;++i)
                {
                    char _buf[strlen(tokens_desc[i])+12];
                    sprintf(_buf, "%%%c   -   %s", tokens[i][0], tokens_desc[i]);
                    AddString_LB(helpBox, _buf);
                }
                for(i=0;i<TOTAL_HOLD_TOKENS;++i)
                {
                    char _buf[strlen(hold_tokens_desc[i])+13];
                    sprintf(_buf, "%%=%c   -   %s %s", hold_tokens[i][0], GetLangString(45), hold_tokens_desc[i]);
                    AddString_LB(helpBox, _buf);
                }
                for(i=0;i<TOTAL_HOLD_TOKENS;++i)
                {
                    char _buf[strlen(hold_tokens_desc[i])+13];
                    sprintf(_buf, "%%!%c   -   %s %s", hold_tokens[i][0], GetLangString(46), hold_tokens_desc[i]);
                    AddString_LB(helpBox, _buf);
                }
                for(i=0;i<TOTAL_OTHER_TOKENS;++i)
                {
                    char _buf[strlen(other_tokens_desc[i])+12];
                    sprintf(_buf, "%%%c   -   %s", other_tokens[i], other_tokens_desc[i]);
                    AddString_LB(helpBox, _buf);
                }
                for(i=0;i<TOTAL_MOUSE_TOKENS;++i)
                {
                    char _buf[strlen(mouse_tokens_desc[i])+12];
                    sprintf(_buf, "%%%c   -   %s", mouse_tokens[i][0], mouse_tokens_desc[i]);
                    AddString_LB(helpBox, _buf);
                }
                if(id-103 >= 13)
                {
                    SetWindowPos(helpBox, 0, 10, ((id-103)*30)+78-120, 235, 98, 0);
                    ShowWindow(textBox[id-104], SW_HIDE);
                    ShowWindow(textBox[id-105], SW_HIDE);
                    ShowWindow(textBox[id-106], SW_HIDE);
                    textBoxHidden[0] = id-104;
                    textBoxHidden[1] = id-105;
                    textBoxHidden[2] = id-106;
                }
                else
                {
                    SetWindowPos(helpBox, 0, 10, ((id-103)*30)+78, 235, 98, 0);
                    ShowWindow(textBox[id-102], SW_HIDE);
                    ShowWindow(textBox[id-101], SW_HIDE);
                    ShowWindow(textBox[id-100], SW_HIDE);
                    textBoxHidden[0] = id-102;
                    textBoxHidden[1] = id-101;
                    textBoxHidden[2] = id-100;
                }
                ShowWindow(helpBox, SW_SHOW);
                helpBoxShown = 1;
            }
        }
        else if(strlen(buf) > 1 && buf[strlen(buf)-2] == 37 && helpBoxShown)
        {
            if(buf[strlen(buf)-1] == 61 || buf[strlen(buf)-1] == 33)
            {
                ClearItems_LB(helpBox);
                for(unsigned i=0;i<TOTAL_HOLD_TOKENS;++i)
                {
                    char _buf[32];
                    sprintf(_buf, "%%%c%c", buf[strlen(buf)-1], hold_tokens[i][0]);
                    AddString_LB(helpBox, _buf);
                }
            }
            else
            {
            	HideHelpBox();
				SetFocus(textBox[lastTextBox]);
            }
        }
    }
    else if(helpBoxShown)
        HideHelpBox();
    lastSize = len;
}

void HideHelpBox(void)
{
    ShowWindow(textBox[textBoxHidden[0]], SW_SHOW);
    ShowWindow(textBox[textBoxHidden[1]], SW_SHOW);
    ShowWindow(textBox[textBoxHidden[2]], SW_SHOW);
    ShowWindow(helpBox, SW_HIDE);
    helpBoxShown = 0;
}

void UpdateText(void)
{
    char chr[MAX_TEXT_LEN];
	const int item = (int)GetSelectedItem_LB(helpBox);
    int len = GetText(textBox[lastTextBox], chr);
    if(chr[len-1] == 37)
    {
        if(item < TOTAL_TOKENS && len < MAX_TEXT_LEN-1)
        {
            sprintf(chr, "%s%c", chr, tokens[item][0]);
            ++len;
        }
        else if(item >= TOTAL_TOKENS && item < TOTAL_TOKENS+(TOTAL_HOLD_TOKENS*2) && len < MAX_TEXT_LEN-2)
        {
            sprintf(chr, "%s%c%c", chr, (((item-TOTAL_TOKENS)/TOTAL_HOLD_TOKENS) ? (33) : (61)), hold_tokens[(item-TOTAL_TOKENS)%TOTAL_HOLD_TOKENS][0]);
            len+=2;
        }
        else if(item >= TOTAL_TOKENS+(TOTAL_HOLD_TOKENS*2) && item < TOTAL_TOKENS+(TOTAL_HOLD_TOKENS*2)+TOTAL_OTHER_TOKENS && len < MAX_TEXT_LEN-1)
        {
            sprintf(chr, "%s%c", chr, other_tokens[item - (TOTAL_TOKENS+(TOTAL_HOLD_TOKENS*2))]);
            ++len;
        }
        else if(item >= TOTAL_TOKENS+(TOTAL_HOLD_TOKENS*2)+TOTAL_OTHER_TOKENS && item < TOTAL_TOKENS+(TOTAL_HOLD_TOKENS*2)+TOTAL_OTHER_TOKENS+TOTAL_MOUSE_TOKENS && len < MAX_TEXT_LEN-1)
        {
            sprintf(chr, "%s%c", chr, mouse_tokens[item - (TOTAL_TOKENS+(TOTAL_HOLD_TOKENS*2)+TOTAL_OTHER_TOKENS)][0]);
            ++len;
        }
    }
    else if(chr[len-2] == 37 && (chr[len-1] == 33 || chr[len-1] == 61) && len < MAX_TEXT_LEN-1)
    {
        sprintf(chr, "%s%c", chr, hold_tokens[item][0]);
        ++len;
    }
    SetText(textBox[lastTextBox], chr);
    HideHelpBox();
    SetFocus(textBox[lastTextBox]);
    SetCaret(textBox[lastTextBox], len);
}
/*********************************** End of help-box-related code *************************************/


/*************************************** File-I/O-related code ****************************************/
void LoadProjectFile(const char path[])
{
	try
	{
		std::ifstream file;
		file.open(path, std::ios::in);
		if(file.is_open())
		{
			std::string _line;
			std::getline(file,_line);
			if(_line.length() == 0)
				return;
			while(TabCtrl_GetItemCount(pageTab) != 0) DestroyPage(0);
			for(int i=0;i<atoi(_line.c_str());++i) AddNewPage();
			std::getline(file,_line);
			if(_line.length() == 0)
				return;
			unsigned curr_page = atoi(_line.c_str()), j=0;
			while (file.good())
			{
				std::getline(file,_line);
				if(_line.length() == 0)
					continue;
				std::vector<std::string> line_exp = split(_line, (char)3);
				if(j == curr_page)
				{
					_lastTextBox = atoi(line_exp.at(16).c_str());
					SetFocus(textBox[_lastTextBox]);
				}
				else
					_sTextBoxIndex.at(j) = atoi(line_exp.at(16).c_str());
				for(unsigned i=0;i<line_exp.size()-1;++i)
				{
					std::vector<std::string> params = split(line_exp[i], (char)2);
					if(j == curr_page)
					{
						TabCtrl_SetCurSel(pageTab, curr_page);
						SetText(textBox[i], ((params.at(0).length() == 1 && params.at(0).at(0) == 32) ? (EMPTY_TEXT) : (params.at(0).c_str())));
						if(i == _lastTextBox)
							SetCaret(textBox[_lastTextBox], params.at(0).length());
						SetSelectedItem_CB(keyComboBox[i], (short)atoi(params.at(1).c_str()));
						if((bool)atoi(params.at(2).c_str()))
						{
							SetButtonEnabled(i);
						}
						else
						{
							SetButtonDisabled(i);
						}
						ToggleChecked(checkBox[0][i], (bool)atoi(params.at(3).c_str()));
						ToggleChecked(checkBox[1][i], (bool)atoi(params.at(4).c_str()));
						ToggleChecked(checkBox[2][i], (bool)atoi(params.at(5).c_str()));
						continue;
					}
					if(params.size() == 6)
					{
						_TextBox.at(j).at(i) = (( params.at(0).length() == 1 && params.at(0).at(0) == 32) ? (EMPTY_TEXT) : (params.at(0)));
						_Get_Item.at(j).at(i) = (short)atoi(params.at(1).c_str());
						if((bool)atoi(params.at(2).c_str()))
							SetTrue(_Button.at(j), i);
						else
							SetFalse(_Button.at(j), i);
						if((bool)atoi(params.at(3).c_str()))
							SetTrue(_Check_Ctrl.at(j), i);
						else
							SetFalse(_Check_Ctrl.at(j), i);
						if((bool)atoi(params.at(4).c_str()))
							SetTrue(_Check_Shift.at(j), i);
						else
							SetFalse(_Check_Shift.at(j), i);
						if((bool)atoi(params.at(5).c_str()))
							SetTrue(_Check_Alt.at(j), i);
						else
							SetFalse(_Check_Alt.at(j), i);
					}
				}
				++j;
			}
			file.close();
			strcpy(filePath, path);
			UpdateMainWindowTitle(curr_page+1);
		}
	}
	catch(std::exception)
	{
		ResetPages();
		MessageBoxEx(mainWindow, (LPCSTR)GetLangString(47), MB_ERROR, MB_OK,  MB_ICONERROR);
	}
}

void SaveProjectFile(const char path[])
{
	try
	{
		std::ofstream file(path);
		if (file.is_open())
		{
			const unsigned short currPage = TabCtrl_GetCurSel(pageTab);
			file << pages << std::endl << TabCtrl_GetCurSel(pageTab) << std::endl;
			for(short i=0;i<pages;++i)
			{
				bool is_curr_page = i == currPage;
				for(unsigned j=0;j<ROWS;++j)
				{
					char txt_buf[MAX_TEXT_LEN];
					if(is_curr_page)
						GetText(textBox[j], txt_buf);
					else
						strcpy(txt_buf, _TextBox.at(i).at(j).c_str());
					if(strlen(txt_buf) == 0)
					{
						txt_buf[0] = 32;
						txt_buf[1] = 0;
					}
					const int sel_item = (is_curr_page ? (GetSelectedItem_CB(keyComboBox[j])) : (_Get_Item.at(i).at(j)));
					bool is_enabled = (is_curr_page ? (GetValue(enabled, j)) : (GetValue(_Button.at(i), j))),
						is_ctrl = (is_curr_page ? (IsChecked(checkBox[0][j])) : (GetValue(_Check_Ctrl.at(i), j))),
						is_shift = (is_curr_page ? (IsChecked(checkBox[1][j])) : (GetValue(_Check_Shift.at(i), j))),
						is_alt = (is_curr_page ? (IsChecked(checkBox[2][j])) : (GetValue(_Check_Alt.at(i), j)));
					file << txt_buf << (char)2 << sel_item << (char)2 << is_enabled << (char)2 << is_ctrl <<
						(char)2 << is_shift << (char)2 << is_alt << (char)3;
				}
				file << (int)(is_curr_page ? _lastTextBox : _sTextBoxIndex.at(i)) << std::endl;
			}
			file.close();
			strcpy(filePath, path);
			UpdateMainWindowTitle(currPage+1);
		}
	}
	catch(std::exception)
	{
		MessageBoxEx(mainWindow, (LPCSTR)GetLangString(48), MB_ERROR, MB_OK,  MB_ICONERROR);
	}
}

bool SaveConfigFile(void)
{
	try
	{
		char dir[CONFIG_PATH];
		GetConfigFilePath(dir);
		std::ofstream file(dir);
		if (file.is_open())
		{
			for(unsigned i=0;i<CONFIG_PARAM;++i)
			{
				file << (unsigned short)configProps[i] << (char)8;
			}
			file.close();
			return 1;
		}
		return 0;
	}
	catch(std::exception)
	{
		return 0;
	}
	return 0;
}

bool LoadConfigFile(void)
{
	try
	{
		char dir[CONFIG_PATH];
		GetConfigFilePath(dir);
		std::ifstream file;
		file.open(dir, std::ios::in);
		if(file.is_open())
		{
			std::string _line;
			std::getline(file,_line);
			if(_line.length() == 0)
				return 0;
			const std::vector<std::string> cfg = split(_line, (char)8);
			if(cfg.size() != CONFIG_PARAM)
				return 0;
			for(unsigned i=0;i<CONFIG_PARAM;++i)
				configProps[i] = atoi(cfg.at(i).c_str());
			file.close();
			return 1;
		}
		return 0;
	}
	catch(std::exception)
	{
		return 0;
	}
	return 0;
}
/*********************************** End of file-I/O-related code *************************************/


/************************************ Configuration-related code **************************************/
void ResetConfig(void)
{
	unsigned short i;
	configProps[0] = configProps[1] = configProps[22] = 1;
	configProps[2] = configProps[3] = configProps[23] = 0;
	configProps[4] = 236;
	configProps[5] = 233;
	configProps[6] = 216;
	for(i=7;i<13;++i)
		configProps[i] = 255;
	for(i=13;i<22;++i)
		configProps[i] = 0;
}

bool CreateDefaultConfigFile(void)
{
	ResetConfig();
	return SaveConfigFile();
}

void LoadConfigIntoSettWindow(void)
{
	unsigned i;
	for(i=0;i<4;++i)
	{
		ToggleChecked(settCtrl[i], configProps[i]);
	}
	ToggleChecked(settCtrl[22], configProps[22]);
	ToggleChecked(settCtrl[23], configProps[23]);
	char buf[4];
	for(i=4;i<22;++i)
	{
		sprintf(buf, "%i", configProps[i]);
		SetText(settCtrl[i], buf);
	}
}

void ResetSettWindowToDefault(void)
{
	unsigned i;
	ToggleChecked(settCtrl[0], 1);
	ToggleChecked(settCtrl[1], 1);
	ToggleChecked(settCtrl[2], 0);
	ToggleChecked(settCtrl[3], 0);
	ToggleChecked(settCtrl[22], 1);
	ToggleChecked(settCtrl[22], 0);
	SetText(settCtrl[4], "236");
	SetText(settCtrl[5], "233");
	SetText(settCtrl[6], "216");
	for(i=7;i<13;++i)
		SetText(settCtrl[i], "255");
	for(i=13;i<22;++i)
		SetText(settCtrl[i], "0");
}

bool LoadConfigFromSettWindow(void)
{
	unsigned i;
	for(i=0;i<4;++i)
	{
		configProps[i] = IsChecked(settCtrl[i]);
	}
	configProps[22] = IsChecked(settCtrl[22]);
	configProps[23] = IsChecked(settCtrl[23]);
	char buf[4];
	for(i=4;i<22;++i)
	{
		GetText(settCtrl[i], buf);
		if(strlen(buf) == 0 || atoi(buf) > 255) return 0;
		configProps[i] = atoi(buf);
	}
	SaveConfigFile();
	return 1;
}
/********************************* End of configuration-related code **********************************/
