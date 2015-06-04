#include <direct.h>

#include "misc.h"

std::vector<std::string> split(const std::string str, const char delimiter)
{
	std::string next;
	std::vector<std::string> result;

	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
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

int random(const int min, const int max)
{
	srand((unsigned)time(NULL));
	return rand() % (max - min + 1) + min;
}

void GetSystemTime(char* out, const bool date = 0)
{
	time_t rawtime;
	struct tm*timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	if (!date)
		sprintf(out, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	else
		sprintf(out, "%02d/%02d/%d", timeinfo->tm_mday, timeinfo->tm_mon + 1, 1900 + timeinfo->tm_year);
}

void ToggleVisualStyles(const bool toggle, ULONG_PTR&cookie)
{
	if (toggle)
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
