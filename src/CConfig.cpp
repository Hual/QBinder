#include <fstream>
#include <vector>
#include <string>

#include "util/misc.h"
#include "CConfig.h"
#include "Core.h"
#include "CGUI.h"
#include "CLocale.h"

CConfig::CConfig()
{
	if (!load())
		if (!createDefault())
			MessageBoxEx(NULL, Core::getLocale()->getLocaleString(6), MB_WARNING, MB_OK, MB_ICONWARNING);

}

bool CConfig::save()
{
	try
	{
		char dir[CONFIG_PATH];
		GetConfigFilePath(dir);
		std::ofstream file(dir);

		if (file.is_open())
		{
			for (unsigned i = 0; i < CONFIG_PARAM_COUNT; ++i)
			{
				file << (unsigned short)m_configProps[i] << (char)8;
			}

			file.close();
			return 1;
		}

		return 0;
	}
	catch (std::exception)
	{
		return 0;
	}

	return 0;
}

bool CConfig::load()
{
	try
	{
		char dir[CONFIG_PATH];
		GetConfigFilePath(dir);
		std::ifstream file;
		file.open(dir, std::ios::in);

		if (file.is_open())
		{
			std::string line;
			std::getline(file, line);

			if (line.length() == 0)
				return 0;

			const std::vector<std::string> cfg = split(line, (char)8);
			if (cfg.size() != CONFIG_PARAM_COUNT)
				return 0;

			for (unsigned i = 0; i < CONFIG_PARAM_COUNT; ++i)
				m_configProps[i] = atoi(cfg.at(i).c_str());

			file.close();
			return 1;
		}
		return 0;
	}
	catch (std::exception)
	{
		return 0;
	}
	return 0;
}

void CConfig::reset()
{
	unsigned short i;
	m_configProps[0] = m_configProps[1] = m_configProps[22] = 1;
	m_configProps[2] = m_configProps[3] = m_configProps[23] = 0;
	m_configProps[4] = 236;
	m_configProps[5] = 233;
	m_configProps[6] = 216;

	for (i = 7; i < 13; ++i)
		m_configProps[i] = 255;

	for (i = 13; i < 22; ++i)
		m_configProps[i] = 0;

}

bool CConfig::createDefault()
{
	this->reset();
	return this->save();
}
