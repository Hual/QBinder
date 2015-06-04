#pragma once

#define CONFIG_PATH	MAX_PATH+0x0C
#define CONFIG_PARAM_COUNT 0x18

#define GetConfigFilePath(p)	do { \
									GetCurrentDirectory(sizeof(p), p); \
									strcat_s(p, "\\config.qbs"); \
								} while(0)


class CConfig
{
public:
	CConfig();
	bool save();
	bool load();
	void reset();
	bool createDefault();
	inline BYTE* getProps() { return m_configProps; }
	inline BYTE getProp(unsigned int index) { return m_configProps[index]; }

private:
	BYTE m_configProps[CONFIG_PARAM_COUNT];

};