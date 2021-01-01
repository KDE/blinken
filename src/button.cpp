/*
    SPDX-FileCopyrightText: 2005-2006 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "button.h"

#include <KConfig>
#include <KSharedConfig>
#include <KConfigGroup>

#include <QKeySequence>

button::button(blinkenGame::color c) : m_selected(false), m_color(c)
{
	KConfigGroup kc(KSharedConfig::openConfig(), "General");
	QString cs = getColorString();
	
	switch (c)
	{
		case blinkenGame::blue:
			m_key = kc.readEntry(cs, int(Qt::Key_3));
		break;
		
		case blinkenGame::yellow:
			m_key = kc.readEntry(cs, int(Qt::Key_1));
		break;
		
		case blinkenGame::red:
			m_key =kc.readEntry(cs, int(Qt::Key_2));
		break;
		
		case blinkenGame::green:
			m_key = kc.readEntry(cs, int(Qt::Key_4));
		break;
		
		default:
			// never happens
		break;
	}
}

button::~button()
{
}

void button::setShortcut(int key)
{
	m_key = key;
	m_selected = false;
	
	KConfigGroup kc(KSharedConfig::openConfig(), "General");
	kc.writeEntry(getColorString(), key);
	kc.sync();
}

QString button::shortcut() const
{
	return QKeySequence(m_key).toString();
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

QString button::getColorString() const
{
	switch (m_color)
	{
		case blinkenGame::blue:
			return QStringLiteral("blue");
		
		case blinkenGame::yellow:
			return QStringLiteral("yellow");
		
		case blinkenGame::red:
			return QStringLiteral("red");
		
		case blinkenGame::green:
			return QStringLiteral("green");
		
		default:
			// never happens
		break;
	}
	
	// never happens
	return QString();
}
