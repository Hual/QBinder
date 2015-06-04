#pragma once

#include "util/controls.h"
#include "Bitwise.h"

class CPage
{
public:
	CPage()
	{
		for (BYTE i = 0; i < 0x10; ++i)
		{
			m_textFields[i] = EMPTY_TEXT;
			m_comboBoxSelectedItems[i] = -1;
		}

		m_lastTextField = 0;
		m_buttonStates = m_ctrlStates = m_shiftStates = m_altStates = 0;
	}

	inline std::string* textFields() { return m_textFields; }
	inline char* comboBoxSelectedItems() { return m_comboBoxSelectedItems; }
	inline bool_16& buttonStates() { return m_buttonStates; }
	inline bool_16& ctrlStates() { return m_ctrlStates; }
	inline bool_16& shiftStates() { return m_shiftStates; }
	inline bool_16& altStates() { return m_altStates; }
	inline BYTE& lastTextField() { return m_lastTextField; }

private:
	std::string m_textFields[0x10];
	BYTE m_lastTextField;
	char m_comboBoxSelectedItems[0x10];
	bool_16 m_buttonStates;
	bool_16 m_ctrlStates;
	bool_16 m_shiftStates;
	bool_16 m_altStates;

};