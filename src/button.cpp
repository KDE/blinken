/*
    SPDX-FileCopyrightText: 2005-2006 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "button.h"

#include <KConfig>
#include <KSharedConfig>
#include <KConfigGroup>

#include <QKeySequence>

button::button(BlinkenGame::Color c) : m_selected(false), m_color(c)
{
	KConfigGroup kc(KSharedConfig::openConfig(), QStringLiteral("General"));
	QString cs = getColorString();
	
	switch (c)
	{
		case BlinkenGame::Blue:
			m_key = kc.readEntry(cs, int(Qt::Key_3));
		break;
		
		case BlinkenGame::Yellow:
			m_key = kc.readEntry(cs, int(Qt::Key_1));
		break;
		
		case BlinkenGame::Red:
			m_key =kc.readEntry(cs, int(Qt::Key_2));
		break;
		
		case BlinkenGame::Green:
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
	
	KConfigGroup kc(KSharedConfig::openConfig(), QStringLiteral("General"));
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
		case BlinkenGame::Blue:
			return QStringLiteral("blue");
		
		case BlinkenGame::Yellow:
			return QStringLiteral("yellow");
		
		case BlinkenGame::Red:
			return QStringLiteral("red");
		
		case BlinkenGame::Green:
			return QStringLiteral("green");
		
		default:
			// never happens
		break;
	}
	
	// never happens
	return QString();
}
