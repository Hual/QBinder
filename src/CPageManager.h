#pragma once

#include <vector>

#include "util/limits.h"
#include "CPage.h"
#include "CGUI.h"

class CGUI;
class Core;

class CPageManager
{
public:
	CPageManager(CGUI* pGUI);
	unsigned short add();
	unsigned short pages() { return m_uiNumPages; }
	void store(const unsigned int page);
	void load(const unsigned int page);
	void destroy(const unsigned short page);
	void destroyAndRecover(unsigned short sel);
	void reset();
	CPage& at(unsigned int index) { return m_pages.at(index); }
	void loadFromFile(const char* path);
	void saveToFile(const char* path);
	std::string& getFilePath() { return m_filePath; }

private:
	CGUI* m_pGUI;
	
	std::vector<CPage> m_pages;
	unsigned short m_uiNumPages;
	std::string m_filePath;

};
