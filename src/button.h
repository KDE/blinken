/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef BUTTON_H
#define BUTTON_H

#include <qobject.h>

#include "simongame.h"

class KAction;
class KActionCollection;
class KShortcut;

class button : public QObject
{
Q_OBJECT
	public:
		button(simonGame::color c, KActionCollection *ac);
		~button();
	
		void setShortcut(const KShortcut &key);
		QString shortcut() const;
		void setSelected(bool b);
		bool selected() const;
		QPixmap *pixmap() const;
	
	signals:
		void pressed();
	
	private:
		QString getColorString() const;
		
		QPixmap *m_highlighted;
		bool m_selected;
		KAction *m_action;
		simonGame::color m_color;
};

#endif
