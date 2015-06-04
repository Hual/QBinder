#include <fstream>

#include "util/misc.h"
#include "CPageManager.h"
#include "Core.h"
#include "KeySimulator.h"

CPageManager::CPageManager(CGUI* pGUI)
{
	m_pGUI = pGUI;
	m_filePath = EMPTY_TEXT;
}

unsigned short CPageManager::add()
{
	char buf[12];
	TC_ITEM new_item;
	sprintf_s(buf, "Page %i", ++m_uiNumPages);
	new_item.pszText = buf;
	new_item.cchTextMax = strlen(buf);
	new_item.mask = TCIF_TEXT;

	m_pages.push_back(CPage());
	InsertItem(m_pGUI->getPageTabControl(), m_uiNumPages - 1, new_item);
	m_pGUI->setSelectedPageTab(m_uiNumPages);

	return m_uiNumPages;
}

void CPageManager::store(const unsigned int page)
{
	bool_16 new_bool[4];
	
	for (BYTE i = 0; i < ROWS; ++i)
	{
		char buf[MAX_TEXT_LEN];
		GetText(m_pGUI->getTextAreaControls()[i], buf, sizeof(buf));
		m_pages.at(page).textFields()[i] = std::string(buf);
		m_pages.at(page).comboBoxSelectedItems()[i] = (BYTE)GetSelectedItem_CB(m_pGUI->getComboBoxControls()[i]);
		
		if (Bitwise::Get<bool_16>(m_pGUI->getButtonStates(), i))
			Bitwise::True<bool_16>(new_bool[0], i);
		else
			Bitwise::False<bool_16>(new_bool[0], i);
		
		if (IsChecked(m_pGUI->getCheckBoxControls()[0][i]))
			Bitwise::True<bool_16>(new_bool[1], i);
		else
			Bitwise::False<bool_16>(new_bool[1], i);

		if (IsChecked(m_pGUI->getCheckBoxControls()[1][i]))
			Bitwise::True<bool_16>(new_bool[2], i);
		else
			Bitwise::False<bool_16>(new_bool[2], i);

		if (IsChecked(m_pGUI->getCheckBoxControls()[2][i]))
			Bitwise::True<bool_16>(new_bool[3], i);
		else
			Bitwise::False<bool_16>(new_bool[3], i);

	}

	m_pages.at(page).lastTextField() = m_pGUI->getLastTextBoxIndex2();
	m_pages.at(page).buttonStates() = new_bool[0];
	m_pages.at(page).ctrlStates() = new_bool[1];
	m_pages.at(page).shiftStates() = new_bool[2];
	m_pages.at(page).altStates() = new_bool[3];
}

void CPageManager::load(const unsigned int page)
{
	for (unsigned i = 0; i < ROWS; ++i)
	{
		m_pGUI->setTextAreaText(i, m_pages.at(page).textFields()[i]);
		m_pGUI->setComboBoxSelectedItem(i, m_pages.at(page).comboBoxSelectedItems()[i]);
		
		ToggleChecked(m_pGUI->getCheckBoxControls()[0][i], Bitwise::Get<bool_16>(m_pages.at(page).ctrlStates(), i));
		ToggleChecked(m_pGUI->getCheckBoxControls()[1][i], Bitwise::Get<bool_16>(m_pages.at(page).shiftStates(), i));
		ToggleChecked(m_pGUI->getCheckBoxControls()[2][i], Bitwise::Get<bool_16>(m_pages.at(page).altStates(), i));

		m_pGUI->toggleButton(i, Bitwise::Get<bool_16>(m_pages.at(page).buttonStates(), i));
	}

	m_pGUI->setSelectedTextArea(m_pages.at(page).lastTextField());
}

void CPageManager::destroy(const unsigned short page)
{
	--m_uiNumPages;
	m_pGUI->deletePageTab(page);
	m_pages.erase(m_pages.begin() + page);
	const unsigned int size = m_pGUI->getPageTabCount();
	unsigned i = 0;

	KeySimulator::unregisterHotkeys(m_pGUI, page);

	while (i < size)
	{
		char buf[12];
		TC_ITEM new_item;
		sprintf_s(buf, "Page %i", ++i);
		new_item.pszText = (LPSTR)buf;
		new_item.cchTextMax = strlen(buf);
		new_item.mask = TCIF_TEXT;
		m_pGUI->changeSelectedPageTab(i - 1, new_item);
	}
}

void CPageManager::destroyAndRecover(unsigned short sel)
{
	if (sel == 0)
	{
		MessageBoxEx(m_pGUI->getMainWindow(), (LPCSTR)Core::getLocale()->getLocaleString(44), MB_ERROR, MB_OK, MB_ICONINFORMATION);
		return;
	}
	this->destroy(sel);

	if (sel >= m_pGUI->getPageTabCount())
		sel = m_pGUI->getPageTabCount() -1;
	
	m_pGUI->setSelectedPageTab(sel);
	this->load(sel);
}

void CPageManager::reset()
{
	while (m_pGUI->getPageTabCount() != 0) this->destroy(0);
	this->add();

	for (unsigned i = 0; i < ROWS; ++i)
	{
		m_pGUI->setTextAreaText(i, EMPTY_TEXT);
		m_pGUI->setComboBoxSelectedItem(i, -1);
		ToggleChecked(m_pGUI->getCheckBoxControls()[0][i], 0);
		ToggleChecked(m_pGUI->getCheckBoxControls()[1][i], 0);
		ToggleChecked(m_pGUI->getCheckBoxControls()[2][i], 0);
		m_pGUI->toggleButton(i, false);
	}

	m_pGUI->setSelectedTextArea(m_pages.at(0).lastTextField());
	m_filePath = EMPTY_TEXT;
	m_pGUI->updateMainWindowTitle(1);
}

void CPageManager::loadFromFile(const char* path)
{
	try
	{
		std::ifstream file;
		file.open(path, std::ios::in);
		if (file.is_open())
		{
			std::string _line;
			std::getline(file, _line);

			if (_line.length() == 0)
				return;

			while (m_pGUI->getPageTabCount())
				destroy(0);

			for (int i = 0; i < atoi(_line.c_str()); ++i)
				add();

			std::getline(file, _line);

			if (_line.length() == 0)
				return;
			
			unsigned curr_page = atoi(_line.c_str()), j = 0;
			while (file.good())
			{
				std::getline(file, _line);

				if (_line.length() == 0)
					continue;

				std::vector<std::string> line_exp = split(_line, (char)3);
				if (j == curr_page)
				{
					m_pGUI->getLastTextBoxIndex2() = atoi(line_exp.at(16).c_str());
					SetFocus(m_pGUI->getTextAreaControls()[m_pGUI->getLastTextBoxIndex2()]);
				}
				else
					m_pages.at(j).lastTextField() = atoi(line_exp.at(16).c_str());

				for (unsigned i = 0; i < line_exp.size() - 1; ++i)
				{
					std::vector<std::string> params = split(line_exp[i], (char)2);
					bool buttonState = atoi(params.at(2).c_str());

					if (j == curr_page)
					{
						m_pGUI->setSelectedPageTab(curr_page);
						m_pGUI->setTextAreaText(i, ((params.at(0).length() == 1 && params.at(0).at(0) == 32) ? (EMPTY_TEXT) : (params.at(0))));

						if (i == m_pGUI->getLastTextBoxIndex2())
							m_pGUI->setTextAreaCaret(m_pGUI->getLastTextBoxIndex2(), params.at(0).length());

						m_pGUI->setComboBoxSelectedItem(i, (char)atoi(params.at(1).c_str()));
						m_pGUI->toggleButton(i, buttonState);

						ToggleChecked(m_pGUI->getCheckBoxControls()[0][i], atoi(params.at(3).c_str()));
						ToggleChecked(m_pGUI->getCheckBoxControls()[1][i], atoi(params.at(4).c_str()));
						ToggleChecked(m_pGUI->getCheckBoxControls()[2][i], atoi(params.at(5).c_str()));

						KeySimulator::updateHotkey(m_pGUI, i);

						continue;
					}
					if (params.size() == 6)
					{
						m_pages.at(j).textFields()[i] = ((params.at(0).length() == 1 && params.at(0).at(0) == 32) ? (EMPTY_TEXT) : (params.at(0)));
						m_pages.at(j).comboBoxSelectedItems()[i] = (char)atoi(params.at(1).c_str());

						if (atoi(params.at(2).c_str()))
							Bitwise::True<bool_16>(m_pages.at(j).buttonStates(), i);
						else
							Bitwise::False<bool_16>(m_pages.at(j).buttonStates(), i);
						if (atoi(params.at(3).c_str()))
							Bitwise::True<bool_16>(m_pages.at(j).ctrlStates(), i);
						else
							Bitwise::False<bool_16>(m_pages.at(j).ctrlStates(), i);
						if (atoi(params.at(4).c_str()))
							Bitwise::True<bool_16>(m_pages.at(j).shiftStates(), i);
						else
							Bitwise::False<bool_16>(m_pages.at(j).shiftStates(), i);
						if (atoi(params.at(5).c_str()))
							Bitwise::True<bool_16>(m_pages.at(j).altStates(), i);
						else
							Bitwise::False<bool_16>(m_pages.at(j).altStates(), i);

						KeySimulator::updateHotkeyFromPage(m_pGUI, this, j, i);
					}
				}
				++j;
			}

			file.close();
			m_filePath = std::string(path);

			m_pGUI->updateMainWindowTitle(curr_page + 1);
		}
	}
	catch (std::exception)
	{
		reset();
		MessageBoxEx(m_pGUI->getMainWindow(), (LPCSTR)Core::getLocale()->getLocaleString(47), MB_ERROR, MB_OK, MB_ICONERROR);
	}
}

void CPageManager::saveToFile(const char* path)
{
	try
	{
		std::ofstream file(path);

		if (file.is_open())
		{
			const unsigned short currPage = m_pGUI->getSelectedPageTab();
			file << m_uiNumPages << std::endl << currPage << std::endl;

			for (short i = 0; i < m_uiNumPages; ++i)
			{
				bool is_curr_page = i == currPage;

				for (unsigned j = 0; j < ROWS; ++j)
				{
					char txt_buf[MAX_TEXT_LEN];

					if (is_curr_page)
						GetText(m_pGUI->getTextAreaControls()[j], txt_buf, sizeof(txt_buf));
					else
						strcpy_s(txt_buf, m_pages.at(i).textFields()[j].c_str());

					if (strlen(txt_buf) == 0)
					{
						txt_buf[0] = 32;
						txt_buf[1] = 0;
					}
					
					const int sel_item = (is_curr_page ? (m_pGUI->getComboBoxSelectedItem(j)) : (m_pages.at(i).comboBoxSelectedItems()[j]));
					bool is_enabled = (is_curr_page ? (Bitwise::Get<bool_16>(m_pGUI->getButtonStates(), j)) : (Bitwise::Get<bool_16>(m_pages.at(i).buttonStates(), j))),
						is_ctrl = (is_curr_page ? (IsChecked(m_pGUI->getCheckBoxControls()[0][j])) : (Bitwise::Get<bool_16>(m_pages.at(i).ctrlStates(), j))),
						is_shift = (is_curr_page ? (IsChecked(m_pGUI->getCheckBoxControls()[1][j])) : (Bitwise::Get<bool_16>(m_pages.at(i).shiftStates(), j))),
						is_alt = (is_curr_page ? (IsChecked(m_pGUI->getCheckBoxControls()[2][j])) : (Bitwise::Get<bool_16>(m_pages.at(i).altStates(), j)));
					file << txt_buf << (char)2 << sel_item << (char)2 << is_enabled << (char)2 << is_ctrl <<
						(char)2 << is_shift << (char)2 << is_alt << (char)3;
				}
				file << (int)(is_curr_page ? m_pGUI->getLastTextBoxIndex2() : m_pages.at(i).lastTextField()) << std::endl;
			}
			file.close();

			m_filePath = std::string(path);
			m_pGUI->updateMainWindowTitle(currPage + 1);
		}
	}
	catch (std::exception)
	{
		MessageBoxEx(m_pGUI->getMainWindow(), (LPCSTR)Core::getLocale()->getLocaleString(48), MB_ERROR, MB_OK, MB_ICONERROR);
	}
}