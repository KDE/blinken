/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qtimer.h>

#include <config.h>

#ifdef WITHOUT_ARTS
#include <klocale.h>
#include <kmessagebox.h>
#endif
#include <kstandarddirs.h>

#include "artsplayer.h"
#include "settings.h"

artsPlayer::artsPlayer() : m_playobj(0)
{
	m_endChecker = new QTimer(this);
	connect(m_endChecker, SIGNAL(timeout()), this, SLOT(checkEnded()));
	
#ifndef WITHOUT_ARTS
	m_dispatcher = new KArtsDispatcher;
	m_server = new KArtsServer;
	m_factory = new KDE::PlayObjectFactory(m_server->server());
	
	m_allPath = locate("appdata","sounds/lose.wav");
	m_greenPath = locate("appdata","sounds/1.wav");
	m_redPath = locate("appdata","sounds/2.wav");
	m_bluePath = locate("appdata","sounds/3.wav");
	m_yellowPath = locate("appdata","sounds/4.wav");
#else
	KMessageBox::information(0, i18n("aRts was not found, therefore the sounds will be disabled."), i18n("Sounds Disabled"), "infoaboutartsnotfound");
#endif
}

artsPlayer::~artsPlayer()
{
#ifndef WITHOUT_ARTS
	delete m_playobj;
	delete m_factory;
	delete m_server;
	delete m_dispatcher;
#endif
}

void artsPlayer::play(blinkenGame::color c, bool stopCurrent)
{
	int check;
#ifndef WITHOUT_ARTS
	if (blinkenSettings::playSounds())
	{
		if (m_playobj && m_playobj -> state() == Arts::posPlaying)
		{
			if (stopCurrent)
			{
				m_nextSounds.clear();
				m_nextSounds.append(c);
				m_playobj -> halt();
				play();
			}
			else m_nextSounds.append(c);
		}
		else
		{
			m_nextSounds.append(c);
			play();
		}
		check = 50;
	}
	else check = 250;
#else
	//shut up gcc
	(void)c;
	(void)stopCurrent;
	check = 250;
#endif
	if (!m_endChecker -> isActive()) m_endChecker -> start(check);
}

void artsPlayer::play()
{
#ifndef WITHOUT_ARTS
	QString path;
	blinkenGame::color c = m_nextSounds.first();
	m_nextSounds.pop_front();
	switch (c)
	{
		case blinkenGame::red:
			path = m_redPath;
		break;
		
		case blinkenGame::green:
			path = m_greenPath;
		break;
		
		case blinkenGame::blue:
			path = m_bluePath;
		break;
		
		case blinkenGame::yellow:
			path = m_yellowPath;
		break;
		
		case blinkenGame::all:
			path = m_allPath;
		break;
		
		case blinkenGame::none:
		break;
	}
	delete m_playobj;
	m_playobj = m_factory -> createPlayObject(path, true);
	m_playobj -> play();
#endif
}

void artsPlayer::checkEnded()
{
#ifndef WITHOUT_ARTS
	if (blinkenSettings::playSounds())
	{
		if (m_playobj -> state() != Arts::posPlaying)
		{
			m_endChecker -> stop();
			emit ended();
			if (m_nextSounds.size() > 0) play();
		}
	}
	else
	{
		m_endChecker -> stop();
		emit ended();
	}
#else
	m_endChecker -> stop();
	emit ended();
#endif
}

#include "artsplayer.moc"
