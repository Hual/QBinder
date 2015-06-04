#pragma once

#include "CGUI.h"
#include "CPageManager.h"
#include "CConfig.h"
#include "CLocale.h"

#define VERSION 0x020000

class CGUI;
class CPageManager;

class Core
{
public:
	static void initialize(HINSTANCE hInstance, int nCmdShow);
	static CPageManager* getPageManager() { return m_pPageManager; }
	static CGUI* getGUI() { return m_pGUI; }
	static CConfig* getConfig() { return m_pConfig; }
	static CLocale* getLocale() { return m_pLocale; }

private:
	static CPageManager* m_pPageManager;
	static CGUI* m_pGUI;
	static CConfig* m_pConfig;
	static CLocale* m_pLocale;

};
