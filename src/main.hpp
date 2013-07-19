#pragma once

#define _WIN32_WINNT 0x501
#define TOTAL_TOKENS 17
#define TOTAL_HOLD_TOKENS 3
#define GetSelectedItem(listbox_hwnd) SendMessage(listbox_hwnd, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0)
#define SetSelectedItem(listbox_hwnd, index) SendMessage(listbox_hwnd, (UINT)CB_SETCURSEL, (WPARAM)index, (LPARAM)0)
#define IsChecked(checkbox_hwnd) (bool)(SendMessage(checkbox_hwnd, BM_GETCHECK, (WPARAM)NULL, (LPARAM)NULL))
#define SetText(hwnd, text) SendMessage(hwnd, WM_SETTEXT, (WPARAM)NULL, (LPARAM)text)
#define GetText(hwnd, buf) SendMessage(hwnd, WM_GETTEXT, sizeof(buf), (LPARAM)buf)

#define NORMAL_KEYS_SIZE 103
#define TEXTBOX_SIZE 134
#define WHITESPACE_SIZE 15

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "resource.h"

bool SimulateKeyArray(char[], size_t, INPUT*);
ULONG_PTR ToggleVisualStyles(void);
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK LowLevelKeyboardProc(int, WPARAM, LPARAM);

const unsigned _BOXES = 16;

HWND TextBox[_BOXES];
HWND KeyComboBox[_BOXES];
HWND button[_BOXES];
HWND checkBox[3][_BOXES];
bool enabled[_BOXES];
bool key_down[_BOXES];

char keys_string[NORMAL_KEYS_SIZE][16] = {
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "Num 0",
    "Num 1",
    "Num 2",
    "Num 3",
    "Num 4",
    "Num 5",
    "Num 6",
    "Num 7",
    "Num 8",
    "Num 9",
    "Num *",
    "Num +",
    "Num Ret",
    "Num -",
    "Num .",
    "Num /",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    ";'",
    "+",
    ",",
    "-",
    ".",
    "/?",
    "`~",
    "[{",
    "\\|",
    "]}",
    "'\"",
    "Backspace",
    "Tab",
    "Enter",
    "Escape",
    "Page Up",
    "Page Down",
    "End",
    "Home",
    "Left Arrow",
    "Up Arrow",
    "Right Arrow",
    "Down Arrow",
    "Num Enter",
    "Print Screen",
    "Insert",
    "Delete"
};

DWORD keys_value[NORMAL_KEYS_SIZE] = {
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

short tokens[TOTAL_TOKENS][2] = {
	{'e', VK_RETURN},
	{'s', VK_SHIFT},
	{'b', VK_BACK},
	{'t', VK_TAB},
	{'c', VK_CONTROL},
	{'a', VK_MENU},
	{'l', VK_CAPITAL},
	{'p', VK_ESCAPE},
	{'n', VK_END},
	{'u', VK_PRIOR},
	{'d', VK_NEXT},
	{'h', VK_HOME},
	{'i', VK_INSERT},
	{'U', VK_UP},
	{'D', VK_DOWN},
	{'L', VK_LEFT},
	{'R', VK_RIGHT}
};

short hold_tokens[TOTAL_TOKENS][2] = {
	{'s', VK_SHIFT},
	{'c', VK_CONTROL},
	{'a', VK_MENU}
};

const HBRUSH BackgroundBrush = CreateSolidBrush(RGB(236,233,216));
const HFONT DefaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
const HFONT HeaderFont = CreateFont(20,0,0,0,FW_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY, VARIABLE_PITCH,TEXT("Arial"));

INPUT *keyInput = new INPUT[4];
