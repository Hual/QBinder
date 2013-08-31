/******************************************************************************************************/
/*************************** Misc: Misc. functions required by the project ****************************/
/******************************************************************************************************/

#include "misc.h"

/************* split: Splits a string by a delimiter, stores the strings in a vector ******************/
std::vector<std::string> split(const std::string str, const char delimiter)
{
    std::string next;
    std::vector<std::string> result;
    for (std::string::const_iterator it = str.begin();it != str.end();++it)
    {
        if (*it == delimiter)
        {
            if (!next.empty())
            {
                result.push_back(next);
                next.clear();
            }
        }
		else
            next += *it;
    }
    if (!next.empty())
        result.push_back(next);
    return result;
}

/****************** random: Generates a random integer in the range of (min;max) **********************/
int random(const int min, const int max)
{
    srand((unsigned)time(NULL));
    return rand()%(max-min+1)+min;
}

/********** GetSystemTime: Inserts the current system date/time into the char array "out" *************/
void GetSystemTime(char out[], const bool date=0)
{
    time_t rawtime;
    struct tm*timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if(!date)
		sprintf(out, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    else
		sprintf(out, "%02d/%02d/%d", timeinfo->tm_mday, timeinfo->tm_mon + 1, 1900 + timeinfo->tm_year);
}

/******* ToggleVisualStyles: Toggles visual styles by de/activating ActCtX for the application ********/
void ToggleVisualStyles(const bool toggle, ULONG_PTR&cookie)
{
    if(toggle)
    {
        char dir[MAX_PATH];
        ACTCTX aX = { sizeof(aX), ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID, TEXT("shell32.dll"), 0, 0, dir, (LPCSTR)124 };
        UINT cch = GetSystemDirectory(dir, sizeof(dir) / sizeof(*dir));
        if (cch >= sizeof(dir) / sizeof(*dir))
			return;
        dir[cch] = TEXT(0);
        ActivateActCtx(CreateActCtx(&aX), &cookie);
        return;
    }
    DeactivateActCtx(0, cookie);
}

/***************** LoadLanguageFile: Loads a .qbl language file into the application ******************/
bool LoadLanguageFile(const char name[], std::vector<std::string>&vec, char m_s[][MOUSE_STRING_SIZE], char mm_s[][MEDIA_STRING_SIZE], char ot_s[][OTHER_TOKENS_STRING_SIZE], char mt_s[][MOUSE_TOKENS_STRING_SIZE])
{
	try
	{
		char path[MAX_PATH+strlen(name)+5];
		GetCurrentDirectory(sizeof(path), path);
		strcat(path, "\\lang\\");
		strcat(path, name);
		strcat(path, ".qbl");
		std::ifstream file;
		file.open(path, std::ios::in);
		if (file.is_open())
		{
			std::string _line;
			std::getline(file,_line);
			vec = split(_line, (char)5);
			if(vec.size() != LANG_PARAMS)
				return 0;
			unsigned i=0;
			while(i < MOUSE_BUTTONS_SIZE)
			{
				strcpy(m_s[i], vec.at(i).c_str());
				++i;
			}
			while(i < MOUSE_BUTTONS_SIZE+MEDIA_KEYS_SIZE)
			{
				strcpy(mm_s[i-MOUSE_BUTTONS_SIZE], vec.at(i).c_str());
				++i;
			}
			while(i < MOUSE_BUTTONS_SIZE+MEDIA_KEYS_SIZE+TOTAL_OTHER_TOKENS)
			{
				strcpy(ot_s[i-(MOUSE_BUTTONS_SIZE+MEDIA_KEYS_SIZE)], vec.at(i).c_str());
				++i;
			}
			while(i < MOUSE_BUTTONS_SIZE+MEDIA_KEYS_SIZE+TOTAL_OTHER_TOKENS+TOTAL_MOUSE_TOKENS)
			{
				strcpy(mt_s[i-(MOUSE_BUTTONS_SIZE+MEDIA_KEYS_SIZE+TOTAL_OTHER_TOKENS)], vec.at(i).c_str());
				++i;
			}
			return 1;
		}
		return 0;
	}
	catch(std::exception)
	{
		return 0;
	}
}

/************** CreateDefaultLanguageFile: Creates the default (English) language file ****************/
void CreateDefaultLanguageFile(void)
{
		char path[MAX_PATH+13];
		GetCurrentDirectory(sizeof(path), path);
		strcat(path, "\\lang\\");
		mkdir(path);
		strcat(path, "english.qbl");
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
			<<(char)5 << "Invalid page specified" << (char)5 << "Invalid range of pages specified" << (char)5 << "Text to simulate" << (char)5
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
