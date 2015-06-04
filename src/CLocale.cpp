#include <Windows.h>
#include <direct.h>
#include <fstream>
#include <string>

#include "util/misc.h"
#include "CLocale.h"

const char CLocale::m_keysStrings[NORMAL_KEYS_SIZE][13] = {
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

const char CLocale::m_tokensDesc[TOTAL_TOKENS][14] = {
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
};

const char CLocale::m_holdTokensDesc[TOTAL_HOLD_TOKENS][8] = {
	"Shift",
	"Control",
	"Alt"
};

CLocale::CLocale()
{
	if (!this->load("english"))
	{
		this->createDefault();
		this->load("english");
	}
}

bool CLocale::load(const char* name)
{
	try
	{
		char path[MAX_PATH];
		GetCurrentDirectory(sizeof(path), path);
		strcat_s(path, "\\lang\\");
		strcat_s(path, name);
		strcat_s(path, ".qbl");

		std::ifstream file;
		file.open(path, std::ios::in);

		if (file.is_open())
		{
			std::string _line;
			std::getline(file, _line);
			m_localeVector = split(_line, (char)5);

			if (m_localeVector.size() != LANG_PARAM_COUNT)
				return 0;

			unsigned i = 0;

			while (i < MOUSE_BUTTONS_SIZE)
			{
				strcpy_s(m_mouseStrings[i], m_localeVector.at(i).c_str());
				++i;
			}

			while (i < MOUSE_BUTTONS_SIZE + MEDIA_KEYS_SIZE)
			{
				strcpy_s(m_mediaStrings[i - MOUSE_BUTTONS_SIZE], m_localeVector.at(i).c_str());
				++i;
			}

			while (i < MOUSE_BUTTONS_SIZE + MEDIA_KEYS_SIZE + TOTAL_OTHER_TOKENS)
			{
				strcpy_s(m_otherTokensDesc[i - (MOUSE_BUTTONS_SIZE + MEDIA_KEYS_SIZE)], m_localeVector.at(i).c_str());
				++i;
			}

			while (i < MOUSE_BUTTONS_SIZE + MEDIA_KEYS_SIZE + TOTAL_OTHER_TOKENS + TOTAL_MOUSE_TOKENS)
			{
				strcpy_s(m_mouseTokensDesc[i - (MOUSE_BUTTONS_SIZE + MEDIA_KEYS_SIZE + TOTAL_OTHER_TOKENS)], m_localeVector.at(i).c_str());
				++i;
			}

			return 1;
		}

		return 0;
	}
	catch (std::exception)
	{
		return 0;
	}
}

void CLocale::createDefault()
{
	char path[MAX_PATH + 13];
	GetCurrentDirectory(sizeof(path), path);
	strcat_s(path, "\\lang\\");
	_mkdir(path);
	strcat_s(path, "english.qbl");
	std::ofstream file(path);

	if (file.is_open())
	{
		file << "Left Mouse Button" << (char)5 << "Right Mouse Button" << (char)5 << "Middle Mouse Button" << (char)5 << "X Mouse Button" << (char)5 <<
			"Media: Prev. Track" << (char)5 << "Media: Play/Pause" << (char)5 << "Media: Stop" << (char)5 << "Media: Next Track" << (char)5 <<
			"Simulate a random key press" << (char)5 << "Press the XXth key (%xXX)" << (char)5 << "Press the XXth F key (%fXX)" << (char)5 <<
			"Echo the time" << (char)5 << "Echo the date" << (char)5 << "Wait XXXX seconds before simulating next key (%CXXXX)" << (char)5 << "Simulate %"
			<< (char)5 << "Press Left Mouse Button" << (char)5 << "Press Right Mouse Button" << (char)5 << "Press Middle Mouse Button" << (char)5 <<
			"Press X Mouse Button" << (char)5 << "Settings (BETA)" << (char)5 << "Add Pages" << (char)5 << "Delete Pages" << (char)5 << "Go to Page"
			<< (char)5 << "Page" << (char)5 << "New Project" << (char)5 << "Could not create configuration file. Does the application have the required permissions?"
			<< (char)5 << "The application uses RGB colour codes, which means you can only input numbers 1-255!" << (char)5 << "The application must be restarted for some of the changes to take effect."
			<< (char)5 << "Adding more than 100 pages at once is not recommended. Do you want to continue?" << (char)5 << "Invalid number of pages specified"
			<< (char)5 << "Invalid page specified" << (char)5 << "Invalid range of pages specified" << (char)5 << "Text to simulate" << (char)5
			<< "75" << (char)5 << "Hotkey && toggle" << (char)5 << "601" << (char)5 << "R:" << (char)5 << "G:" << (char)5 << "B:" << (char)5 << "Default multimedia key support enabled"
			<< (char)5 << "Text antialiasing enabled" << (char)5 << "Visual styles enabled" << (char)5 << "Bound key processing enabled" << (char)5 <<
			"Bound mouse button processing enabled" << (char)5 << "Grid view enabled" << (char)5 << "Background colour" << (char)5 << "141" << (char)5 <<
			"Text box background colour" << (char)5 << "117" << (char)5 << "List box background colour" << (char)5 << "117" << (char)5 << "Label text colour"
			<< (char)5 << "148" << (char)5 << "Text box text colour" << (char)5 << "141" << (char)5 << "List box text colour" << (char)5 << "141" << (char)5 <<
			"Accept" << (char)5 << "Default" << (char)5 << "Accept" << (char)5 << "Page to go to:" << (char)5 << "Pages to add:" << (char)5 << "Pages to delete:"
			<< (char)5 << "The first page cannot be removed!" << (char)5 << "Hold" << (char)5 << "Release" << (char)5 << "Erroneous file selected."
			<< (char)5 << "Could not write to disk. Does the application have the required permissions?" << (char)5 << "Disabled" << (char)5 << "Enabled" << (char)5 <<
			"Could not open project file." << (char)5 << "Could not save project to disk.";

		file.close();
	}
}
