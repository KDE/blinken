/*
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef BUTTON_H
#define BUTTON_H

#include "blinkengame.h"

class button
{
	public:
		explicit button(blinkenGame::color c);
		~button();
	
		void setShortcut(int key);
		QString shortcut() const;
		int key() const;
		void setSelected(bool b);
		bool selected() const;
	
	private:
		QString getColorString() const;
		
		bool m_selected;
		int m_key;
		blinkenGame::color m_color;
};

#endif
