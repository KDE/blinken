/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

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
