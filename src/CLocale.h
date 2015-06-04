#pragma once

#include <vector>

#include "util/limits.h"

#define LANG_PARAM_COUNT 0x48

class CLocale
{
public:
	CLocale();
	bool load(const char* name);
	void createDefault();
	char(&getMouseStrings())[MOUSE_BUTTONS_SIZE][MOUSE_STRING_SIZE] { return m_mouseStrings; }
	char(&getMediaStrings())[MEDIA_KEYS_SIZE][MEDIA_STRING_SIZE] { return m_mediaStrings; }
	const char(&getKeysStrings())[NORMAL_KEYS_SIZE][13] { return m_keysStrings; }
	char(&getOtherTokensDesc())[TOTAL_OTHER_TOKENS][OTHER_TOKENS_STRING_SIZE] { return m_otherTokensDesc; }
	char(&getMouseTokensDesc())[TOTAL_MOUSE_TOKENS][MOUSE_TOKENS_STRING_SIZE] { return m_mouseTokensDesc; }
	const char(&getTokensDesc())[TOTAL_TOKENS][14] { return m_tokensDesc; }
	const char(&getHoldTokensDesc())[TOTAL_HOLD_TOKENS][8] { return m_holdTokensDesc; }

	const char* getLocaleString(unsigned char index)
	{
		return m_localeVector.at(MOUSE_BUTTONS_SIZE + MEDIA_KEYS_SIZE + TOTAL_OTHER_TOKENS + TOTAL_MOUSE_TOKENS + index).c_str();
	}

private:
	std::vector<std::string> m_localeVector;

	char m_mouseStrings[MOUSE_BUTTONS_SIZE][MOUSE_STRING_SIZE];
	char m_mediaStrings[MEDIA_KEYS_SIZE][MEDIA_STRING_SIZE];
	char m_otherTokensDesc[TOTAL_OTHER_TOKENS][OTHER_TOKENS_STRING_SIZE];
	char m_mouseTokensDesc[TOTAL_MOUSE_TOKENS][MOUSE_TOKENS_STRING_SIZE];
	static const char m_keysStrings[NORMAL_KEYS_SIZE][13];

	static const char m_tokensDesc[TOTAL_TOKENS][14];
	static const char m_holdTokensDesc[TOTAL_HOLD_TOKENS][8];
};
