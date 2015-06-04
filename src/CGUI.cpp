#include "util/misc.h"
#include "CGUI.h"
#include "Core.h"
#include "Bitwise.h"
#include "CConfig.h"
#include "CLocale.h"
#include "KeySimulator.h"

CGUI::CGUI(HINSTANCE hInstance)
{
	this->m_brushes[0] = CreateSolidBrush(RGB(Core::getConfig()->getProp(4), Core::getConfig()->getProp(5), Core::getConfig()->getProp(6)));
	this->m_brushes[1] = CreateSolidBrush(RGB(Core::getConfig()->getProp(7), Core::getConfig()->getProp(8), Core::getConfig()->getProp(9)));
	this->m_brushes[2] = CreateSolidBrush(RGB(Core::getConfig()->getProp(10), Core::getConfig()->getProp(11), Core::getConfig()->getProp(12)));
	this->m_headerFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, (Core::getConfig()->getProp(22) ? ANTIALIASED_QUALITY : DEFAULT_QUALITY), VARIABLE_PITCH, TEXT("Arial"));
	this->m_hInstance = hInstance;

	if (Core::getConfig()->getProp(1))
		ToggleVisualStyles(1, m_actCookie);

	if (!this->registerClass(PS_MAIN, mainWindowProc_static, CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, ID_ICON, IDC_ARROW, ID_MENU, this->m_brushes[0]))
		PostQuitMessage(0);

	if (!this->registerClass(PS_SETT, settingsWindowProc_static, CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, ID_ICON, IDC_ARROW, NULL, this->m_brushes[0]))
		PostQuitMessage(0);

	if (!this->registerClass(PS_PAGE, pageWindowProc_static, CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, ID_ICON, IDC_ARROW, NULL, this->m_brushes[0]))
		PostQuitMessage(0);
}

void CGUI::startMessageLoop(int nCmdShow)
{
	MSG messages;

	m_hMainWindow = CreateWindowEx(0, PS_MAIN, NULL, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, (GetSystemMetrics(SM_CXSCREEN) - 800) / 2, (GetSystemMetrics(SM_CYSCREEN) - 630) / 2, 800, 630, HWND_DESKTOP, NULL, m_hInstance, NULL);
	this->updateMainWindowTitle(1);
	ShowWindow(m_hMainWindow, nCmdShow);

	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}

	this->unregisterClasses();
}

bool CGUI::registerClass(LPCSTR className, const WNDPROC callback, const UINT style, const UINT iconResource, LPCSTR cursor, const UINT menu, const HBRUSH backgr)
{
	WNDCLASSEX winclass;
	winclass.hInstance = m_hInstance;
	winclass.lpszClassName = className;
	winclass.lpfnWndProc = callback;
	winclass.style = style;
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(iconResource));
	winclass.hIconSm = LoadIcon(m_hInstance, MAKEINTRESOURCE(iconResource));
	winclass.hCursor = LoadCursor(m_hInstance, cursor);
	winclass.lpszMenuName = MAKEINTRESOURCE(menu);
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hbrBackground = backgr;

	if (!RegisterClassEx(&winclass))
		return 0;

	return 1;
}

void CGUI::unregisterClasses()
{
	UnregisterClass(PS_MAIN, m_hInstance);
	UnregisterClass(PS_SETT, m_hInstance);
	UnregisterClass(PS_PAGE, m_hInstance);
}

void CGUI::updateMainWindowTitle(const unsigned short page)
{
	char version[MAX_PATH + 24];
	sprintf_s(version, "QBinder %u.%u.%02u  -  %s %i  -  %s", VERSION / 65536, (VERSION - (65536 * (VERSION / 65536)) - (VERSION % 256)) / 256, VERSION % 256, Core::getLocale()->getLocaleString(4), page, (Core::getPageManager()->getFilePath().length() > 0 ? Core::getPageManager()->getFilePath().c_str() : Core::getLocale()->getLocaleString(5)));
	SetWindowText(m_hMainWindow, version);
}

LRESULT CALLBACK CGUI::settingsWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		this->createSettingsControls(hwnd);
		this->populateSettingsWindow(Core::getConfig());
		break;
	}
	case WM_COMMAND:
	{
		const unsigned wmId = LOWORD(wParam);
		switch (wmId)
		{
		case ID_SETT_BUTT_ACCEPT:
		{
			if (!populateConfig(Core::getConfig()))
			{
				MessageBoxEx(hwnd, (LPCSTR)Core::getLocale()->getLocaleString(7), MB_ERROR, MB_OK, MB_ICONERROR);
				break;
			}
			MessageBoxEx(hwnd, (LPCSTR)Core::getLocale()->getLocaleString(8), MB_INFO, MB_OK, MB_ICONWARNING);
			DestroyWindow(hwnd);
			break;
		}
		case ID_SETT_BUTT_DEFAULT:
		{
			this->resetSettingsWindow();
			break;
		}
		}
		break;
	}
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	{
		return this->getColours(Core::getConfig(), (HDC)wParam, COL_TYPE_STATIC, FNT_TYPE_STATIC);
	}
	case WM_CTLCOLOREDIT:
	{
		return this->getColours(Core::getConfig(), (HDC)wParam, COL_TYPE_EDIT, FNT_TYPE_EDIT);
	}
	case WM_DESTROY:
	{
		this->showSettingsWindow(0);
		break;
	}
	default:
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	}
	return 0;
}

LRESULT CALLBACK CGUI::pageWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		this->createPageControls(hwnd);
		break;
	}
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	{
		return this->getColours(Core::getConfig(), (HDC)wParam, COL_TYPE_STATIC, FNT_TYPE_STATIC);
	}
	case WM_CTLCOLOREDIT:
	{
		return this->getColours(Core::getConfig(), (HDC)wParam, COL_TYPE_EDIT, FNT_TYPE_EDIT);
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == ID_PAGE_BUTTON_OK)
		{
			char buf[6];
			if (!m_delPageRange)
			{
				GetText(m_pageRangeControls[1], buf, sizeof(buf));
				const unsigned short _pages = atoi(buf);
				if (!m_gotoPage)
				{
					if (_pages != 0 && (Core::getPageManager()->pages() + _pages) < (unsigned short)~0)
					{
						if (_pages > 100 && MessageBoxEx(hwnd, (LPCSTR)Core::getLocale()->getLocaleString(9), MB_WARNING, MB_YESNO, MB_ICONWARNING) == 7)
							break;

						for (unsigned i = 0; i < _pages; ++i)
							Core::getPageManager()->add();

						DestroyWindow(m_hPageWindow);
					}
					else
						MessageBoxEx(hwnd, (LPCSTR)Core::getLocale()->getLocaleString(10), MB_ERROR, MB_OK, MB_ICONERROR);
				}
				else
				{
					if (_pages > 0 && _pages <= Core::getPageManager()->pages())
					{
						DestroyWindow(m_hPageWindow);
						Core::getPageManager()->store(this->getSelectedPageTab());
						Core::getPageManager()->load(_pages - 1);
						this->setSelectedPageTab(_pages - 1);
						this->updateMainWindowTitle(_pages);
					}
					else
						MessageBoxEx(hwnd, (LPCSTR)Core::getLocale()->getLocaleString(11), MB_ERROR, MB_OK, MB_ICONERROR);
				}
				break;
			}
			GetText(m_pageRangeControls[3], buf, sizeof(buf));
			const unsigned short lower_b = atoi(buf);
			GetText(m_pageRangeControls[4], buf, sizeof(buf));
			unsigned short upper_b = atoi(buf);
			if (lower_b < 2 || upper_b > Core::getPageManager()->pages() || lower_b > upper_b)
			{
				MessageBoxEx(hwnd, (LPCSTR)Core::getLocale()->getLocaleString(12), MB_ERROR, MB_OK, MB_ICONERROR);
				break;
			}
			for (upper_b = upper_b - 1; upper_b >= lower_b - 1; --upper_b)
			{
				Core::getPageManager()->destroyAndRecover(upper_b);
			}
			DestroyWindow(hwnd);
		}
		break;
	}
	case WM_DESTROY:
	{
		this->showPageWindow(false);
		break;
	}
	default:
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	}
	return 0;
}

LRESULT CALLBACK CGUI::mainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const unsigned wmId = LOWORD(wParam), wmEvent = HIWORD(wParam);
	switch (message)
	{
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	{
		return this->getColours(Core::getConfig(), (HDC)wParam, COL_TYPE_STATIC, FNT_TYPE_STATIC);
	}
	case WM_CTLCOLOREDIT:
	{
		return this->getColours(Core::getConfig(), (HDC)wParam, COL_TYPE_EDIT, FNT_TYPE_EDIT);
	}
	case WM_CTLCOLORLISTBOX:
	{
		return this->getColours(Core::getConfig(), (HDC)wParam, COL_TYPE_LISTBOX, FNT_TYPE_LISTBOX);
	}
	case WM_NOTIFY:
	{
		NMHDR*eInfo = (NMHDR*)lParam;
		if (eInfo->hwndFrom == m_pageTabControl)
		{
			switch (eInfo->code)
			{
			case TCN_SELCHANGE:
			{
				const int page = this->getSelectedPageTab();
				Core::getPageManager()->load(page);
				this->updateMainWindowTitle(page + 1);

				break;
			}
			case TCN_SELCHANGING:
			{
				Core::getPageManager()->store(this->getSelectedPageTab());
			}
			}
		}
		break;
	}
	case WM_CREATE:
	{
		this->createMainControls(hwnd);
		break;
	}
	case WM_HOTKEY:
	{
		int id = wParam;

		unsigned short page = id / ROWS;
		unsigned char row = id % ROWS;

		char txt[MAX_TEXT_LEN];

		if (getSelectedPageTab() == page)
			Core::getGUI()->getTextAreaText(row, txt, sizeof(txt));
		else
			strcpy_s(txt, Core::getPageManager()->at(page).textFields()[row].c_str());

		if (!strcmp(txt, EMPTY_TEXT) || txt == NULL)
			break;

		KeySimulator::pop(HIWORD(lParam));
		KeySimulator::simulate(txt, strlen(txt));

		break;
	}
	case WM_COMMAND:
	{
		if (wmId >= ID_MAIN_EDIT && wmId < ID_MAIN_EDIT + ROWS)
		{
			switch (wmEvent)
			{
			case 768:
			{
				showHelpWindow(wmId);
				break;
			}
			case 512:
			{
				m_lastTextBoxIndex = wmId - 103;
				break;
			}
			case 256:
			{
				if (isHelpBoxShown() && m_lastTextBoxIndex != wmId - 103)
					hideHelpWindow();

				m_lastTextBoxIndex2 = wmId - 103;
				break;
			}
			}
		}
		else if (wmId >= ID_BUTTON && wmId < ID_BUTTON + ROWS)
		{
			this->toggleButton(wmId - ID_BUTTON, !Bitwise::Get<bool_16>(m_buttonStates, wmId - ID_BUTTON));

			KeySimulator::updateHotkey(this, wmId - ID_BUTTON);
		}
		else if (wmId >= ID_MAIN_COMBO && wmId < ID_MAIN_COMBO + ROWS && HIWORD(wParam) == CBN_SELCHANGE)
			KeySimulator::updateHotkey(this, wmId - ID_MAIN_COMBO);
		else if (wmId >= ID_CHECKBOX_CTRL && wmId < ID_CHECKBOX_ALT + ROWS && HIWORD(wParam) == BN_CLICKED)
			KeySimulator::updateHotkey(this, (wmId - ID_CHECKBOX_CTRL) % ROWS);

		switch (wmId)
		{
		case ID_FILE_EXIT:
		{
			DestroyWindow(hwnd);
			break;
		}
		case ID_EDIT_NEWP:
		{
			Core::getPageManager()->add();
			break;
		}
		case ID_EDIT_DELP:
		{
			Core::getPageManager()->destroyAndRecover(this->getSelectedPageTab());

			break;
		}
		case ID_EDIT_RANGE:
		{
			m_delPageRange = 0;
			m_gotoPage = 0;
			this->showPageWindow(true);

			break;
		}
		case ID_EDIT_DELRANGE:
		{
			m_gotoPage = 0;
			m_delPageRange = 1;
			this->showPageWindow(true);
			break;
		}
		case ID_EDIT_GOTO:
		{
			m_delPageRange = 0;
			m_gotoPage = 1;
			this->showPageWindow(true);
			break;
		}
		case ID_FILE_SAVE:
		{
			if (Core::getPageManager()->getFilePath().length() == 0)
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
				sfd.lpstrInitialDir = NULL;
				sfd.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLEHOOK;

				if (!GetSaveFileName(&sfd))
					MessageBoxEx(hwnd, (LPCSTR)Core::getLocale()->getLocaleString(51), MB_ERROR, MB_OK, MB_ICONWARNING);
				else
					Core::getPageManager()->saveToFile(buf);

			}
			else
				Core::getPageManager()->saveToFile(Core::getPageManager()->getFilePath().c_str());

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
			sfd.lpstrInitialDir = NULL;
			sfd.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLEHOOK;

			if (!GetSaveFileName(&sfd))
				MessageBoxEx(hwnd, (LPCSTR)Core::getLocale()->getLocaleString(51), MB_ERROR, MB_OK, MB_ICONWARNING);
			else
				Core::getPageManager()->saveToFile(buf);

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
			ofd.lpstrInitialDir = NULL;
			ofd.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

			if (!GetOpenFileName(&ofd))
				MessageBoxEx(hwnd, (LPCSTR)Core::getLocale()->getLocaleString(51), MB_ERROR, MB_OK, MB_ICONERROR);
			else
				Core::getPageManager()->loadFromFile(buf);

			break;
		}
		case ID_FILE_NEW:
		{
			Core::getPageManager()->reset();
			break;
		}
		case ID_TOOLS_SETTINGS:
		{
			this->showSettingsWindow(true);
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

void CGUI::createMainControls(HWND& hwnd)
{
	m_helpBoxControl = CreateWindowEx(0, WC_LISTBOX, NULL, WS_CHILD | WS_VSCROLL | WS_BORDER, 0, 0, 244, 100, hwnd, (HMENU)ID_HELPBOX, NULL, NULL);
	SetFont(m_helpBoxControl, DEFAULT_FONT);

	if (Core::getConfig()->getProp(23))
		CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 1, 50, 795, 1, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);

	for (unsigned i = 0; i < ROWS; ++i)
	{
		m_textAreaControls[i] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 10, (i * 30) + 56, 100 + TEXTBOX_SIZE, 20, hwnd, (HMENU)(ID_MAIN_EDIT + i), GetModuleHandle(NULL), NULL);

		SetFont(m_textAreaControls[i], DEFAULT_FONT);
		SetTextLimit(m_textAreaControls[i], MAX_TEXT_LEN);

		if (Core::getConfig()->getProp(23))
			CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, (i == ROWS - 1 ? 1 : 10), (i * 30) + 80, (i == ROWS - 1 ? 795 : 772), 1, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);

		m_buttonControls[i] = CreateWindowEx(0, WC_BUTTON, Core::getLocale()->getLocaleString(49), WS_CHILD | WS_VISIBLE, 645 - 24 + TEXTBOX_SIZE - (WHITESPACE_SIZE * 4) + 5, (i * 30) + 54, 80, 23, hwnd, (HMENU)(ID_BUTTON + i), GetModuleHandle(NULL), NULL);
		SetFont(m_buttonControls[i], DEFAULT_FONT);
		m_checkBoxControls[0][i] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 200 - 24 + TEXTBOX_SIZE - WHITESPACE_SIZE, (i * 30) + 56, 15, 20, hwnd, (HMENU)(ID_CHECKBOX_CTRL + i), GetModuleHandle(NULL), NULL);
		m_checkBoxControls[1][i] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 305 - 25 + TEXTBOX_SIZE - (WHITESPACE_SIZE * 2), (i * 30) + 56, 15, 20, hwnd, (HMENU)(ID_CHECKBOX_SHIFT + i), GetModuleHandle(NULL), NULL);
		m_checkBoxControls[2][i] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 410 - 25 + TEXTBOX_SIZE - (WHITESPACE_SIZE * 3), (i * 30) + 56, 15, 20, hwnd, (HMENU)(ID_CHECKBOX_ALT + i), GetModuleHandle(NULL), NULL);
		m_keyComboBoxControls[i] = CreateWindowEx(0, WC_COMBOBOX, NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_TABSTOP | WS_VSCROLL, 515 - 24 - 36 + 8 + 5 + TEXTBOX_SIZE - (WHITESPACE_SIZE * 4), (i * 30) + 55, 146, 335, hwnd, (HMENU)(ID_MAIN_COMBO + i), GetModuleHandle(NULL), NULL);
		SetFont(m_keyComboBoxControls[i], DEFAULT_FONT);

		unsigned j;
		for (j = 0; j < NORMAL_KEYS_SIZE; ++j)
			AddString_CB(m_keyComboBoxControls[i], Core::getLocale()->getKeysStrings()[j]);

		for (j = 0; j < MOUSE_BUTTONS_SIZE; ++j)
			AddString_CB(m_keyComboBoxControls[i], Core::getLocale()->getMouseStrings()[j]);

		if (Core::getConfig()->getProp(0))
			for (j = 0; j < MEDIA_KEYS_SIZE; ++j)
				AddString_CB(m_keyComboBoxControls[i], Core::getLocale()->getMediaStrings()[j]);

	}

	m_pageTabControl = CreateWindowEx(0, WC_TABCONTROL, NULL, WS_CHILD | WS_VISIBLE, -1, 550, 798, 35, hwnd, (HMENU)ID_MAIN_PAGE, GetModuleHandle(NULL), NULL);
	SetFont(m_pageTabControl, DEFAULT_FONT);

	if (!Core::getConfig()->getProp(23))
		CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 1, 42, 795, 1, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);

	CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 0, 548, 797, 1, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
	CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 255, 0, 1, (Core::getConfig()->getProp(23) ? 532 : 550), hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
	CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 345, 0, 1, (Core::getConfig()->getProp(23) ? 532 : 550), hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
	CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 435, 0, 1, (Core::getConfig()->getProp(23) ? 532 : 550), hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
	CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDVERT, 525, 0, 1, (Core::getConfig()->getProp(23) ? 532 : 550), hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
	const BYTE tmp = (Core::getConfig()->getProp(23) ? 15 : 12);
	HWND lbl = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(13), WS_CHILD | WS_VISIBLE, atoi(Core::getLocale()->getLocaleString(14)), tmp, 150, 20, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
	SetFont(lbl, m_headerFont);
	lbl = CreateWindowEx(0, WC_STATIC, "Control", WS_CHILD | WS_VISIBLE, 274, tmp, 60, 20, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
	SetFont(lbl, m_headerFont);
	lbl = CreateWindowEx(0, WC_STATIC, "Shift", WS_CHILD | WS_VISIBLE, 373, tmp, 60, 20, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
	SetFont(lbl, m_headerFont);
	lbl = CreateWindowEx(0, WC_STATIC, "Alt", WS_CHILD | WS_VISIBLE, 471, tmp, 20, 20, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
	SetFont(lbl, m_headerFont);
	lbl = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(15), WS_CHILD | WS_VISIBLE, atoi(Core::getLocale()->getLocaleString(16)), tmp, 300, 20, hwnd, (HMENU)ID_MAIN_LABELS, GetModuleHandle(NULL), NULL);
	SetFont(lbl, m_headerFont);
	Core::getPageManager()->add();
}

void CGUI::createSettingsControls(HWND& hwnd)
{
	m_hSettingsControls[0] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 4, 15, 20, hwnd, (HMENU)ID_SETT_CHECK, GetModuleHandle(NULL), NULL);
	m_hSettingsControls[1] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 24, 15, 20, hwnd, (HMENU)(ID_SETT_CHECK + 1), GetModuleHandle(NULL), NULL);
	m_hSettingsControls[2] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 44, 15, 20, hwnd, (HMENU)(ID_SETT_CHECK + 2), GetModuleHandle(NULL), NULL);
	m_hSettingsControls[3] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 64, 15, 20, hwnd, (HMENU)(ID_SETT_CHECK + 3), GetModuleHandle(NULL), NULL);
	m_hSettingsControls[22] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 84, 15, 20, hwnd, (HMENU)(ID_SETT_CHECK + 4), GetModuleHandle(NULL), NULL);
	m_hSettingsControls[23] = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 20, 104, 15, 20, hwnd, (HMENU)(ID_SETT_CHECK + 5), GetModuleHandle(NULL), NULL);
	for (unsigned i = 0; i < 6; ++i)
	{
		BYTE id = (3 * (i + 1));
		m_hSettingsControls[20 + id + 1] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(17), WS_CHILD | WS_VISIBLE, SETT_RGB_X, 155 + (SETT_RGB_Y_SPACE*i), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(m_hSettingsControls[20 + id + 1], DEFAULT_FONT);
		m_hSettingsControls[id + 1] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, SETT_RGB_X + 15, 153 + (SETT_RGB_Y_SPACE*i), 40, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(m_hSettingsControls[id + 1], DEFAULT_FONT);
		SetTextLimit(m_hSettingsControls[id + 1], 3);
		m_hSettingsControls[20 + id + 2] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(18), WS_CHILD | WS_VISIBLE, SETT_RGB_X + 73, 155 + (SETT_RGB_Y_SPACE*i), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(m_hSettingsControls[20 + id + 2], DEFAULT_FONT);
		m_hSettingsControls[id + 2] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, SETT_RGB_X + 88, 153 + (SETT_RGB_Y_SPACE*i), 40, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(m_hSettingsControls[id + 2], DEFAULT_FONT);
		SetTextLimit(m_hSettingsControls[id + 2], 3);
		m_hSettingsControls[20 + id + 3] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(19), WS_CHILD | WS_VISIBLE, SETT_RGB_X + 146, 155 + (SETT_RGB_Y_SPACE*i), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(m_hSettingsControls[20 + id + 3], DEFAULT_FONT);
		m_hSettingsControls[id + 3] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, SETT_RGB_X + 161, 153 + (SETT_RGB_Y_SPACE*i), 40, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(m_hSettingsControls[id + 3], DEFAULT_FONT);
		SetTextLimit(m_hSettingsControls[id + 3], 3);
	}
	m_hSettingsControls[42] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(20), WS_CHILD | WS_VISIBLE, 37, 7, 300, 20, hwnd, (HMENU)ID_SETT_CHECK, GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[42], DEFAULT_FONT);
	m_hSettingsControls[44] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(22), WS_CHILD | WS_VISIBLE, 37, 27, 300, 20, hwnd, (HMENU)(ID_SETT_CHECK + 1), GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[44], DEFAULT_FONT);
	m_hSettingsControls[45] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(23), WS_CHILD | WS_VISIBLE, 37, 47, 300, 20, hwnd, (HMENU)(ID_SETT_CHECK + 2), GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[45], DEFAULT_FONT);
	m_hSettingsControls[46] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(24), WS_CHILD | WS_VISIBLE, 37, 67, 300, 20, hwnd, (HMENU)(ID_SETT_CHECK + 3), GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[46], DEFAULT_FONT);
	m_hSettingsControls[43] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(21), WS_CHILD | WS_VISIBLE, 37, 87, 300, 20, hwnd, (HMENU)(ID_SETT_CHECK + 4), GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[43], DEFAULT_FONT);
	m_hSettingsControls[47] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(25), WS_CHILD | WS_VISIBLE, 37, 107, 300, 20, hwnd, (HMENU)(ID_SETT_CHECK + 5), GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[47], DEFAULT_FONT);
	m_hSettingsControls[48] = CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 5, 126, 386, 1, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	m_hSettingsControls[49] = CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ, 5, 435, 386, 1, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	m_hSettingsControls[50] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(26), WS_CHILD | WS_VISIBLE, atoi(Core::getLocale()->getLocaleString(27)), 133, 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[50], SETT_FONT);
	m_hSettingsControls[51] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(28), WS_CHILD | WS_VISIBLE, atoi(Core::getLocale()->getLocaleString(29)), 133 + SETT_RGB_Y_SPACE, 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[51], SETT_FONT);
	m_hSettingsControls[52] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(30), WS_CHILD | WS_VISIBLE, atoi(Core::getLocale()->getLocaleString(31)), 133 + (SETT_RGB_Y_SPACE * 2), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[52], SETT_FONT);
	m_hSettingsControls[53] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(32), WS_CHILD | WS_VISIBLE, atoi(Core::getLocale()->getLocaleString(33)), 133 + (SETT_RGB_Y_SPACE * 3), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[53], SETT_FONT);
	m_hSettingsControls[54] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(34), WS_CHILD | WS_VISIBLE, atoi(Core::getLocale()->getLocaleString(35)), 133 + (SETT_RGB_Y_SPACE * 4), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[54], SETT_FONT);
	m_hSettingsControls[55] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(36), WS_CHILD | WS_VISIBLE, atoi(Core::getLocale()->getLocaleString(37)), 133 + (SETT_RGB_Y_SPACE * 5), 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[55], SETT_FONT);
	m_hSettingsControls[56] = CreateWindowEx(0, WC_BUTTON, Core::getLocale()->getLocaleString(38), WS_CHILD | WS_VISIBLE, 319, 442, 70, 23, hwnd, (HMENU)ID_SETT_BUTT_ACCEPT, GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[56], DEFAULT_FONT);
	m_hSettingsControls[57] = CreateWindowEx(0, WC_BUTTON, Core::getLocale()->getLocaleString(39), WS_CHILD | WS_VISIBLE, 4, 442, 70, 23, hwnd, (HMENU)ID_SETT_BUTT_DEFAULT, GetModuleHandle(NULL), NULL);
	SetFont(m_hSettingsControls[57], DEFAULT_FONT);
}

void CGUI::createPageControls(HWND& hwnd)
{
	m_pageRangeControls[2] = CreateWindowEx(0, WC_BUTTON, Core::getLocale()->getLocaleString(40), WS_CHILD | WS_VISIBLE, 63, 45, 70, 23, hwnd, (HMENU)ID_PAGE_BUTTON_OK, GetModuleHandle(NULL), NULL);
	SetFont(m_pageRangeControls[2], DEFAULT_FONT);

	if (!m_delPageRange)
	{
		m_pageRangeControls[0] = CreateWindowEx(0, WC_STATIC, (m_gotoPage ? Core::getLocale()->getLocaleString(41) : Core::getLocale()->getLocaleString(42)), WS_CHILD | WS_VISIBLE, 16, 15, 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetFont(m_pageRangeControls[0], DEFAULT_FONT);
		m_pageRangeControls[1] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, 95, 13, 85, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
		SetTextLimit(m_pageRangeControls[1], 5);
		SetFont(m_pageRangeControls[1], DEFAULT_FONT);
		return;
	}

	m_pageRangeControls[0] = CreateWindowEx(0, WC_STATIC, Core::getLocale()->getLocaleString(43), WS_CHILD | WS_VISIBLE, 7, 15, 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(m_pageRangeControls[0], DEFAULT_FONT);
	m_pageRangeControls[1] = CreateWindowEx(0, WC_STATIC, "-", WS_CHILD | WS_VISIBLE, 137, 15, 300, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetFont(m_pageRangeControls[1], DEFAULT_FONT);
	m_pageRangeControls[3] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, 91, 13, 40, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetTextLimit(m_pageRangeControls[3], 5);
	SetFont(m_pageRangeControls[3], DEFAULT_FONT);
	m_pageRangeControls[4] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, 146, 13, 40, 20, hwnd, (HMENU)ID_OTHER, GetModuleHandle(NULL), NULL);
	SetTextLimit(m_pageRangeControls[4], 5);
	SetFont(m_pageRangeControls[4], DEFAULT_FONT);
}

void CGUI::populateSettingsWindow(CConfig* pConfig)
{
	unsigned i;
	for (i = 0; i < 4; ++i)
	{
		ToggleChecked(this->m_hSettingsControls[i], pConfig->getProps()[i]);
	}

	ToggleChecked(this->m_hSettingsControls[22], pConfig->getProps()[22]);
	ToggleChecked(this->m_hSettingsControls[23], pConfig->getProps()[23]);

	char buf[4];

	for (i = 4; i < 22; ++i)
	{
		sprintf_s(buf, "%i", pConfig->getProps()[i]);
		SetText(this->m_hSettingsControls[i], buf);
	}
}

bool CGUI::populateConfig(CConfig* pConfig)
{
	unsigned i;
	for (i = 0; i < 4; ++i)
	{
		pConfig->getProps()[i] = (BYTE)IsChecked(this->m_hSettingsControls[i]);
	}
	pConfig->getProps()[22] = (BYTE)IsChecked(this->m_hSettingsControls[22]);
	pConfig->getProps()[23] = (BYTE)IsChecked(this->m_hSettingsControls[23]);

	char buf[4];

	for (i = 4; i < 22; ++i)
	{
		GetText(this->m_hSettingsControls[i], buf, sizeof(buf));
		if (strlen(buf) == 0 || atoi(buf) > 255)
			return 0;

		pConfig->getProps()[i] = atoi(buf);
	}

	pConfig->save();
	return 1;
}

void CGUI::resetSettingsWindow()
{
	unsigned i;
	ToggleChecked(this->m_hSettingsControls[0], 1);
	ToggleChecked(this->m_hSettingsControls[1], 1);
	ToggleChecked(this->m_hSettingsControls[2], 0);
	ToggleChecked(this->m_hSettingsControls[3], 0);
	ToggleChecked(this->m_hSettingsControls[22], 1);
	ToggleChecked(this->m_hSettingsControls[22], 0);
	SetText(this->m_hSettingsControls[4], "236");
	SetText(this->m_hSettingsControls[5], "233");
	SetText(m_hSettingsControls[6], "216");

	for (i = 7; i < 13; ++i)
		SetText(this->m_hSettingsControls[i], "255");

	for (i = 13; i < 22; ++i)
		SetText(this->m_hSettingsControls[i], "0");

}

INT_PTR CGUI::getColours(CConfig* pConfig, HDC hdc, const BYTE type, const BYTE fontType)
{
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(pConfig->getProps()[fontType], pConfig->getProps()[fontType + 1], pConfig->getProps()[fontType + 2]));
	SetBkColor(hdc, RGB(pConfig->getProps()[type], pConfig->getProps()[type + 1], pConfig->getProps()[type + 2]));
	return (INT_PTR)m_brushes[((type - 1) / 3) - 1];
}

void CGUI::showSettingsWindow(const bool show)
{
	if (show)
	{
		m_hSettingsWindow = CreateWindowEx(0, PS_SETT, Core::getLocale()->getLocaleString(0), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, (GetSystemMetrics(SM_CXSCREEN) - 400) / 2, (GetSystemMetrics(SM_CYSCREEN) - 500) / 2, 400, 500, m_hMainWindow, NULL, m_hInstance, NULL);
		ShowWindow(m_hSettingsWindow, SW_SHOW);
		EnableWindow(m_hMainWindow, 0);
	}
	else
	{
		for (unsigned i = 0; i < sizeof(m_hSettingsControls) / sizeof(HWND); ++i)
		{
			DestroyWindow(m_hSettingsControls[i]);
		}

		EnableWindow(m_hMainWindow, 1);
		SetFocus(m_hMainWindow);
	}
}

void CGUI::showPageWindow(const bool show)
{
	if (show)
	{
		m_hPageWindow = CreateWindowEx(0, PS_PAGE, (!m_gotoPage ? (!m_delPageRange ? Core::getLocale()->getLocaleString(1) : Core::getLocale()->getLocaleString(2)) : Core::getLocale()->getLocaleString(3)), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, (GetSystemMetrics(SM_CXSCREEN) - 200) / 2, (GetSystemMetrics(SM_CYSCREEN) - 100) / 2, 200, 100, m_hMainWindow, NULL, m_hInstance, NULL);
		ShowWindow(m_hPageWindow, SW_SHOW);
		EnableWindow(m_hMainWindow, 0);
	}
	else
	{
		for (unsigned i = 0; i < sizeof(m_pageRangeControls) / sizeof(HWND); ++i)
		{
			DestroyWindow(m_pageRangeControls[i]);
		}

		EnableWindow(m_hMainWindow, 1);
		SetFocus(m_hMainWindow);
	}
}

void CGUI::toggleButton(unsigned int index, bool toggle)
{
	if (toggle)
	{
		SetText(m_buttonControls[index], Core::getLocale()->getLocaleString(50));
		Bitwise::True<bool_16>(m_buttonStates, index);
	}
	else
	{
		SetText(m_buttonControls[index], Core::getLocale()->getLocaleString(49));
		Bitwise::False<bool_16>(m_buttonStates, index);
	}
}

LRESULT CALLBACK CGUI::settingsWindowProc_static(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Core::getGUI()->settingsWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK CGUI::pageWindowProc_static(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Core::getGUI()->pageWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK CGUI::mainWindowProc_static(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Core::getGUI()->mainWindowProc(hwnd, message, wParam, lParam);
}

void CGUI::showHelpWindow(const unsigned char index)
{
	char buf[MAX_TEXT_LEN];
	const int len = GetText(m_textAreaControls[index - ID_MAIN_EDIT], buf, sizeof(buf));

	if ((len > m_lastSize && m_lastTextBoxIndex == index - ID_MAIN_EDIT) || m_lastTextBoxIndex != index - ID_MAIN_EDIT)
	{
		if (buf[strlen(buf) - 1] == 37)
		{
			if (strlen(buf) > 1 && buf[strlen(buf) - 2] == 37 && m_helpBoxShown)
				hideHelpWindow();
			else
			{
				unsigned i;
				ClearItems_LB(m_helpBoxControl);

				char _buf[0x60];

				for (i = 0; i < TOTAL_TOKENS; ++i)
				{
					sprintf_s(_buf, "%%%c   -   %s", KeySimulator::getTokens()[i][0], Core::getLocale()->getTokensDesc()[i]);
					AddString_LB(m_helpBoxControl, _buf);
				}

				for (i = 0; i < TOTAL_HOLD_TOKENS; ++i)
				{
					sprintf_s(_buf, "%%=%c   -   %s %s", KeySimulator::getHoldTokens()[i][0], Core::getLocale()->getLocaleString(45), Core::getLocale()->getHoldTokensDesc()[i]);
					AddString_LB(m_helpBoxControl, _buf);
				}

				for (i = 0; i < TOTAL_HOLD_TOKENS; ++i)
				{
					sprintf_s(_buf, "%%!%c   -   %s %s", KeySimulator::getHoldTokens()[i][0], Core::getLocale()->getLocaleString(46), Core::getLocale()->getHoldTokensDesc()[i]);
					AddString_LB(m_helpBoxControl, _buf);
				}

				for (i = 0; i < TOTAL_OTHER_TOKENS; ++i)
				{
					sprintf_s(_buf, "%%%c   -   %s", KeySimulator::getOtherTokens()[i], Core::getLocale()->getOtherTokensDesc()[i]);
					AddString_LB(m_helpBoxControl, _buf);
				}

				for (i = 0; i < TOTAL_MOUSE_TOKENS; ++i)
				{
					sprintf_s(_buf, "%%%c   -   %s", KeySimulator::getMouseTokens()[i][0], Core::getLocale()->getMouseTokensDesc()[i]);
					AddString_LB(m_helpBoxControl, _buf);
				}

				if (index - 103 >= 13)
				{
					SetWindowPos(m_helpBoxControl, 0, 10, ((index - 103) * 30) + 78 - 120, 235, 98, 0);
					ShowWindow(m_textAreaControls[index - 104], SW_HIDE);
					ShowWindow(m_textAreaControls[index - 105], SW_HIDE);
					ShowWindow(m_textAreaControls[index - 106], SW_HIDE);
					m_hiddenTextBoxIndex[0] = index - 104;
					m_hiddenTextBoxIndex[1] = index - 105;
					m_hiddenTextBoxIndex[2] = index - 106;
				}
				else
				{
					SetWindowPos(m_helpBoxControl, 0, 10, ((index - 103) * 30) + 78, 235, 98, 0);
					ShowWindow(m_textAreaControls[index - 102], SW_HIDE);
					ShowWindow(m_textAreaControls[index - 101], SW_HIDE);
					ShowWindow(m_textAreaControls[index - 100], SW_HIDE);
					m_hiddenTextBoxIndex[0] = index - 102;
					m_hiddenTextBoxIndex[1] = index - 101;
					m_hiddenTextBoxIndex[2] = index - 100;
				}
				ShowWindow(m_helpBoxControl, SW_SHOW);
				m_helpBoxShown = true;
			}
		}
		else if (strlen(buf) > 1 && buf[strlen(buf) - 2] == 37 && m_helpBoxShown)
		{
			if (buf[strlen(buf) - 1] == 61 || buf[strlen(buf) - 1] == 33)
			{
				ClearItems_LB(m_helpBoxControl);
				for (unsigned i = 0; i < TOTAL_HOLD_TOKENS; ++i)
				{
					char _buf[32];
					sprintf_s(_buf, "%%%c%c", buf[strlen(buf) - 1], KeySimulator::getHoldTokens()[i][0]);
					AddString_LB(m_helpBoxControl, _buf);
				}
			}
			else
			{
				hideHelpWindow();
				SetFocus(m_textAreaControls[m_lastTextBoxIndex]);
			}
		}
	}
	else if (m_helpBoxShown)
		hideHelpWindow();

	m_lastSize = len;
}

void CGUI::hideHelpWindow()
{
	ShowWindow(m_textAreaControls[m_hiddenTextBoxIndex[0]], SW_SHOW);
	ShowWindow(m_textAreaControls[m_hiddenTextBoxIndex[1]], SW_SHOW);
	ShowWindow(m_textAreaControls[m_hiddenTextBoxIndex[2]], SW_SHOW);
	ShowWindow(m_helpBoxControl, SW_HIDE);
	m_helpBoxShown = false;
}
