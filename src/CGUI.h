#pragma once

#include <string>

#include "res/resource.h"
#include "util/controls.h"
#include "util/limits.h"
#include "Bitwise.h"
#include "CConfig.h"

const HFONT DEFAULT_FONT = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
const HFONT SETT_FONT = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, TEXT("Arial"));

class CPageManager;
class Core;

class CGUI
{
public:
	CGUI(HINSTANCE hInstance);
	void startMessageLoop(int nCmdShow);

	void updateMainWindowTitle(const unsigned short page);

	LRESULT CALLBACK settingsWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK pageWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK mainWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND getMainWindow() { return m_hMainWindow; }

	HWND getPageTabControl() { return m_pageTabControl; }
	void setSelectedPageTab(int index) { TabCtrl_SetCurSel(m_pageTabControl, index); }
	void changeSelectedPageTab(int index, TC_ITEM item) { ChangeItem(m_pageTabControl, index, item); }
	int getSelectedPageTab() { return TabCtrl_GetCurSel(m_pageTabControl); }
	void deletePageTab(unsigned short page) { TabCtrl_DeleteItem(m_pageTabControl, page); }
	int getPageTabCount() { return TabCtrl_GetItemCount(m_pageTabControl); }

	HWND* getTextAreaControls() { return m_textAreaControls; }
	HWND* getComboBoxControls() { return m_keyComboBoxControls; }
	HWND(&getCheckBoxControls())[3][ROWS] { return m_checkBoxControls; }

	void setTextAreaText(unsigned int index, const std::string text) { SetText(m_textAreaControls[index], text.c_str()); }
	void getTextAreaText(unsigned int index, char* out, const size_t len) { GetText(m_textAreaControls[index], out, len); }
	void setTextAreaCaret(unsigned int index, int position) { SetCaret(m_textAreaControls[index], position); }
	void setComboBoxSelectedItem(unsigned int index, char selectedItem) { SetSelectedItem_CB(m_keyComboBoxControls[index], selectedItem); }
	int getComboBoxSelectedItem(unsigned int index) { return GetSelectedItem_CB(m_keyComboBoxControls[index]); }

	void toggleButton(unsigned int index, bool toggle);

	void setSelectedTextArea(unsigned int index)
	{
		SetFocus(m_textAreaControls[index]);
		m_lastTextBoxIndex2 = index;
		SetCaret(m_textAreaControls[m_lastTextBoxIndex2], 0);
	}

	bool_16& getButtonStates() { return m_buttonStates; }

	BYTE& getLastTextBoxIndex() { return m_lastTextBoxIndex; }
	BYTE& getLastTextBoxIndex2() { return m_lastTextBoxIndex2; }

	bool isHelpBoxShown() { return m_helpBoxShown; }

private:
	HBRUSH m_brushes[3];
	HFONT m_headerFont;
	HINSTANCE m_hInstance;
	ULONG_PTR m_actCookie;

	HWND m_hMainWindow;
	HWND m_hSettingsWindow;
	HWND m_hPageWindow;

	HWND m_hSettingsControls[58];
	HWND m_pageRangeControls[5];
	HWND m_pageTabControl;
	HWND m_textAreaControls[ROWS];
	HWND m_keyComboBoxControls[ROWS];
	HWND m_buttonControls[ROWS];
	HWND m_checkBoxControls[3][ROWS];
	HWND m_helpBoxControl;

	bool m_delPageRange = false;
	bool m_gotoPage = false;

	bool_16 m_buttonStates = 0;

	int m_lastSize = 0;
	int m_lastItem;

	bool m_helpBoxShown = false;

	BYTE m_lastTextBoxIndex;
	BYTE m_lastTextBoxIndex2;
	BYTE m_hiddenTextBoxIndex[3];

	bool registerClass(LPCSTR className, const WNDPROC callback, const UINT style, const UINT iconResource, LPCSTR cursor, const UINT menu, const HBRUSH backgr);
	void unregisterClasses();
	void populateSettingsWindow(CConfig* pConfig);
	bool populateConfig(CConfig* pConfig);
	void resetSettingsWindow();
	INT_PTR getColours(CConfig* pConfig, HDC hdc, const BYTE type, const BYTE fontType);

	void createMainControls(HWND& hwnd);
	void createSettingsControls(HWND& hwnd);
	void createPageControls(HWND& hwnd);

	void showSettingsWindow(const bool show);
	void showPageWindow(const bool show);
	
	void showHelpWindow(const unsigned char index);
	void hideHelpWindow();
	
	static LRESULT CALLBACK settingsWindowProc_static(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK pageWindowProc_static(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK mainWindowProc_static(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

};