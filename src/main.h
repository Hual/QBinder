/******************************************************************************************************/
/******************** Main.h: The main header, contains all the spicy declarations ********************/
/******************************************************************************************************/


/********************** Definition, inclusion, other preprocessor directives **************************/
#pragma once

#define VERSION 0x010100
#define CONFIG_PATH	MAX_PATH+0x0C
#define CONFIG_PARAM 0x18

#include "res/resource.h"
#include "util/misc.h"
#include "util/mem.h"


/****************************************** Method declaration ****************************************/
INT_PTR GetColours(HDC, const BYTE, const BYTE);
DWORD WINAPI KeyArraySimThread(LPVOID);
LRESULT CALLBACK
	MainWindowProc(HWND, UINT, WPARAM, LPARAM),
	SettWindowProc(HWND, UINT, WPARAM, LPARAM),
	PageWindowProc(HWND, UINT, WPARAM, LPARAM),
	LowLevelKeyboardProc(int, WPARAM, LPARAM),
	LowLevelMouseProc(int, WPARAM, LPARAM);
bool
	RegClass(HINSTANCE&, LPCSTR, const WNDPROC, const UINT, const UINT, LPCSTR, const UINT, const HBRUSH),
	SaveConfigFile(void),
	CreateDefaultConfigFile(void),
	LoadConfigFile(void),
	LoadConfigFromSettWindow(void);
void
	ShowSettWindow(const bool),
	ShowPageWindow(const bool),
	UpdateMainWindowTitle(const unsigned short),
	__SimulateKeyArray(const char[], const size_t),
	SimulateKeyArray(const char[], const size_t, INPUT*),
	CreateMainControls(HWND&),
	CreateSettControls(HWND&),
	CreatePageControls(HWND&),
	AddNewPage(void),
	DestroyPage(const unsigned short),
	_DestroyPage(HWND&, unsigned short),
	ResetPages(void),
	StoreContents(const int),
	LoadContents(const int),
	ShowHelpBox(const unsigned),
	HideHelpBox(void),
	UpdateText(void),
	LoadProjectFile(const char[]),
	SaveProjectFile(const char[]),
	ResetConfig(void),
	LoadConfigIntoSettWindow(void),
	ResetSettWindowToDefault(void);


/********************************* Field declaration and assignment ***********************************/
HINSTANCE instance;
INPUT*keyInput = new INPUT[4];
HANDLE keyEvent;
size_t eventLen;
ULONG_PTR _actCookie;
HHOOK inputHook[2];
unsigned short pages=0;
bool_16 enabled=0;
HBRUSH brushes[3];
HFONT HEADER_FONT;
const HFONT
	DEFAULT_FONT=(HFONT)GetStockObject(DEFAULT_GUI_FONT),
	SETT_FONT=CreateFont(15,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,RUSSIAN_CHARSET,OUT_OUTLINE_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,VARIABLE_PITCH,TEXT("Arial"));
bool
	helpBoxShown=0,
	gotoPage=0,
	delPageRange=0;
BYTE
	textBoxHidden[3],
	lastTextBox,
	_lastTextBox,
	configProps[CONFIG_PARAM];
HWND
	textBox[ROWS],
	keyComboBox[ROWS],
	button[ROWS],
	checkBox[3][ROWS],
	pageRangeCtrl[5],
	settCtrl[58],
	pageTab,
	helpBox,
	mainWindow,
	settWindow,
	pageWindow;
DWORD
	lastMousePress,
	simThreadID;
int
	lastSize,
	lastItem;
char
	eventStr[MAX_TEXT_LEN],
	filePath[MAX_PATH]=EMPTY_TEXT;
std::vector<std::vector<std::string> > _TextBox;
std::vector<std::vector<char> > _Get_Item;
std::vector<BYTE> _sTextBoxIndex;
std::vector<std::string> langStr;
std::vector<bool_16>
	_Button,
	_Check_Ctrl,
	_Check_Shift,
	_Check_Alt;


/*************************************** Key-related arrays *******************************************/
const char keys_string[NORMAL_KEYS_SIZE][13] = {
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

char mouse_string[MOUSE_BUTTONS_SIZE][MOUSE_STRING_SIZE],
media_string[MEDIA_KEYS_SIZE][MEDIA_STRING_SIZE];

const DWORD mouse_value[MOUSE_BUTTONS_SIZE] = {
    0x0201,
    0x0204,
    0x0207,
    0x020B
},
keys_value[NORMAL_KEYS_SIZE] = {
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
},
media_keys_value[MEDIA_KEYS_SIZE]= {
	0xB1,
	0xB3,
	0xB2,
	0xB0
};

/************************************** Token-related arrays ******************************************/
const char tokens_desc[TOTAL_TOKENS][14] = {
    "Return",
    "Left Shift",
    "Right Shift",
    "Backspace",
    "Tab",
	"Left Control",
	"Right Control",
    "Left Alt",
    "Right Alt",
    "Caps Lock",
    "Escape",
    "End",
    "Page Up",
    "Page Down",
    "Home",
    "Insert",
    "Up Arrow",
    "Down Arrow",
    "Left Arrow",
    "Right Arrow"
},
hold_tokens_desc[TOTAL_HOLD_TOKENS][8] = {
    "Shift",
    "Control",
    "Alt"
};

char other_tokens_desc[TOTAL_OTHER_TOKENS][OTHER_TOKENS_STRING_SIZE],
mouse_tokens_desc[TOTAL_MOUSE_TOKENS][MOUSE_TOKENS_STRING_SIZE];

const BYTE tokens[TOTAL_TOKENS][2] = {
	{0x65, VK_RETURN},
	{0x73, VK_LSHIFT},
	{0x53, VK_RSHIFT},
	{0x62, VK_BACK},
	{0x74, VK_TAB},
	{0x63, VK_LCONTROL},
	{0x6F, VK_RCONTROL},
	{0x61, VK_LMENU},
	{0x41, VK_RMENU},
	{0x6C, VK_CAPITAL},
	{0x70, VK_ESCAPE},
	{0x6E, VK_END},
	{0x75, VK_PRIOR},
	{0x64, VK_NEXT},
	{0x68, VK_HOME},
	{0x69, VK_INSERT},
	{0x55, VK_UP},
	{0x44, VK_DOWN},
	{0x4C, VK_LEFT},
	{0x52, VK_RIGHT}
},
other_tokens[TOTAL_OTHER_TOKENS] = {
    0x72,
    0x78,
	0x66,
    0x54,
    0x4A,
    0x43,
    0x25
},
hold_tokens[TOTAL_TOKENS][2] = {
	{0x73, VK_SHIFT},
	{0x63, VK_CONTROL},
	{0x61, VK_MENU}
};

const unsigned short mouse_tokens[TOTAL_MOUSE_TOKENS][3] = {
    {0x42, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP},
    {0x49, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP},
    {0x4D, MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP},
    {0x47, MOUSEEVENTF_XDOWN, MOUSEEVENTF_XUP}
};
