/******************************************************************************************************/
/*************************** Misc: Misc. functions required by the project ****************************/
/******************************************************************************************************/

#include "../res/controls.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <string>
#include <vector>
#include <fstream>

#define LANG_PARAMS 0x48

std::vector<std::string> split(const std::string, const char);
int random(const int, const int);
bool
	LoadLanguageFile(const char name[], std::vector<std::string>&, char[][MOUSE_STRING_SIZE], char[][MEDIA_STRING_SIZE], char[][OTHER_TOKENS_STRING_SIZE], char[][MOUSE_TOKENS_STRING_SIZE]);
void
	GetSystemTime(char[], const bool),
	ToggleVisualStyles(const bool, ULONG_PTR&),
	CreateDefaultLanguageFile(void);
