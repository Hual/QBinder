/******************************************************************************************************/
/********* Controls.h: Contains macros, definitions and inclusions related to WinAPI controls *********/
/******************************************************************************************************/

#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501
#define WINVER 0x0501
#define _WIN32_WINDOWS 0x0501
#define _WIN32_IE 0x0501

#define GetSelectedItem_CB(hwnd) SendMessage(hwnd, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0)
#define SetSelectedItem_CB(hwnd, index) SendMessage(hwnd, (UINT)CB_SETCURSEL, (WPARAM)index, (LPARAM)0)
#define GetSelectedItem_LB(hwnd) SendMessage(hwnd, (UINT)LB_GETCURSEL, (WPARAM)0, (LPARAM)0)
#define ClearItems_LB(hwnd) SendMessage(hwnd, LB_RESETCONTENT, 0, 0)
#define ToggleChecked(hwnd, toggle) SendMessage(hwnd, BM_SETCHECK, toggle, NULL)
#define IsChecked(hwnd) (bool)(SendMessage(hwnd, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL))
#define SetText(hwnd, text) SendMessage(hwnd, WM_SETTEXT, (WPARAM)NULL, (LPARAM)text)
#define GetText(hwnd, buf) SendMessage(hwnd, WM_GETTEXT, sizeof(buf), (LPARAM)buf)
#define SetCaret(hwnd, index) SendMessage(hwnd, EM_SETSEL, index, index)
#define AddString_LB(hwnd, str) SendMessage(hwnd, LB_ADDSTRING, (WPARAM)NULL, (LPARAM)str)
#define AddString_CB(hwnd, str) SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)str)
#define SetFont(hwnd, font) SendMessage(hwnd, WM_SETFONT, (WPARAM)font, MAKELPARAM(FALSE, 0))
#define SetTextLimit(hwnd, limit) SendMessage(hwnd, EM_SETLIMITTEXT, (WPARAM)limit, 0)
#define InsertItem(hwnd, index, item) SendMessage(hwnd, TCM_INSERTITEM, index, (LPARAM)&item)
#define ChangeItem(hwnd, index, item) SendMessage(hwnd, TCM_SETITEM, index, (LPARAM)&item)

#define GetLangString(i) 		langStr.at(MOUSE_BUTTONS_SIZE+MEDIA_KEYS_SIZE+TOTAL_OTHER_TOKENS+TOTAL_MOUSE_TOKENS+i).c_str()
#define SetButtonDisabled(id)	SetText(button[id], GetLangString(49)); \
								SetFalse(enabled, id)
#define SetButtonEnabled(id)	SetText(button[id], GetLangString(50)); \
								SetTrue(enabled, id)
#define GetConfigFilePath(p) 	GetCurrentDirectory(sizeof(p), p); \
								strcat(p, "\\config.qbs")

#define EMPTY_TEXT ""
#define ROWS 0x10
#define MAX_TEXT_LEN 0x200
#define TEXTBOX_SIZE 0x86
#define MOUSE_STRING_SIZE 0x20
#define MEDIA_STRING_SIZE 0x20
#define NORMAL_KEYS_SIZE 0x67
#define MEDIA_KEYS_SIZE 0x04
#define MOUSE_BUTTONS_SIZE 0x04

#define OTHER_TOKENS_STRING_SIZE 0x46
#define MOUSE_TOKENS_STRING_SIZE 0x2A
#define TOTAL_TOKENS 0x14
#define TOTAL_HOLD_TOKENS 0x03
#define TOTAL_OTHER_TOKENS 0x07
#define TOTAL_MOUSE_TOKENS 0x04

#define WHITESPACE_SIZE 0x0F
#define SETT_RGB_X 0x55
#define SETT_RGB_Y_SPACE 0x32

#define COL_TYPE_STATIC 0x04
#define COL_TYPE_EDIT 0x07
#define COL_TYPE_LISTBOX 0x0A
#define FNT_TYPE_STATIC 0x0D
#define FNT_TYPE_EDIT 0x10
#define FNT_TYPE_LISTBOX 0x13

#define PS_MAIN "qbmain"
#define PS_SETT "qbsett"
#define PS_PAGE "qbpage"
#define MB_INFO "Information"
#define MB_WARNING "Warning"
#define MB_ERROR "Error"

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
