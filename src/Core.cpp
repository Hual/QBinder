#include "Core.h"
#include "KeySimulator.h"

CPageManager* Core::m_pPageManager = NULL;
CGUI* Core::m_pGUI = NULL;
CConfig* Core::m_pConfig = NULL;
CLocale* Core::m_pLocale = NULL;

void Core::initialize(HINSTANCE hInstance, int nCmdShow)
{
	KeySimulator::initialize();

	m_pLocale = new CLocale();
	m_pConfig = new CConfig();
	m_pGUI = new CGUI(hInstance);
	m_pPageManager = new CPageManager(m_pGUI);

	m_pGUI->startMessageLoop(nCmdShow);
}
