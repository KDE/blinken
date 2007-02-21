/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <kaction.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kstandarddirs.h>
#include <kdebug.h>

#include "button.h"

button::button(blinkenGame::color c) : m_selected(false), m_color(c)
{
	KConfig *kc = kapp->config();
	kc->setGroup("General");
	QString cs = getColorString();
	QString pixmap = QString("images/%1h.png").arg(cs);
	
	switch (c)
	{
		case blinkenGame::blue:
			m_key = kc->readNumEntry(cs, Qt::Key_3);
		break;
		
		case blinkenGame::yellow:
			m_key = kc->readNumEntry(cs, Qt::Key_1);
		break;
		
		case blinkenGame::red:
			m_key = kc->readNumEntry(cs, Qt::Key_2);
		break;
		
		case blinkenGame::green:
			m_key = kc->readNumEntry(cs, Qt::Key_4);
		break;
		
		default:
			// never happens
		break;
	}
	
	m_highlighted = new QPixmap(locate("appdata", pixmap));
}

button::~button()
{
	delete m_highlighted;
}

void button::setShortcut(int key)
{
	m_key = key;
	m_selected = false;
	
	KConfig *kc = kapp->config();
	kc->setGroup("General");
	kc->writeEntry(getColorString(), key);
	kc->sync();
}

QString button::shortcut() const
{
	return KShortcut(m_key).toString();
}

int button::key() const
{
	return m_key;
}

void button::setSelected(bool b)
{
	m_selected = b;
}

bool button::selected() const
{
	return m_selected;
}

QPixmap *button::pixmap() const
{
	return m_highlighted;
}

QString button::getColorString() const
{
	switch (m_color)
	{
		case blinkenGame::blue:
			return "blue";
		break;
		
		case blinkenGame::yellow:
			return "yellow";
		break;
		
		case blinkenGame::red:
			return "red";
		break;
		
		case blinkenGame::green:
			return "green";
		break;
		
		default:
			// never happens
		break;
	}
	
	// never happens
	return QString::null;
}
