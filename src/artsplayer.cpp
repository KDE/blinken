/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qtimer.h>

#include <kstandarddirs.h>

#include "artsplayer.h"

artsPlayer::artsPlayer() : m_playobj(0)
{
	m_dispatcher = new KArtsDispatcher;
	m_server = new KArtsServer;
	m_factory = new KDE::PlayObjectFactory(m_server->server());
	
	m_endChecker = new QTimer(this);
	connect(m_endChecker, SIGNAL(timeout()), this, SLOT(checkEnded()));
	
	m_greenPath = locate("appdata","sounds/1.wav");
	m_redPath = locate("appdata","sounds/2.wav");
	m_bluePath = locate("appdata","sounds/3.wav");
	m_yellowPath = locate("appdata","sounds/4.wav");
}

artsPlayer::~artsPlayer()
{
	delete m_factory;
	delete m_server;
	delete m_dispatcher;
}

void artsPlayer::play(simonGame::color c, bool stopCurrent)
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
	if (!m_endChecker -> isActive()) m_endChecker -> start(50);
}

void artsPlayer::play()
{
	QString path;
	simonGame::color c = m_nextSounds.first();
	m_nextSounds.pop_front();
	switch (c)
	{
		case simonGame::red:
			path = m_redPath;
		break;
		
		case simonGame::green:
			path = m_greenPath;
		break;
		
		case simonGame::blue:
			path = m_bluePath;
		break;
		
		case simonGame::yellow:
			path = m_yellowPath;
		break;
		
		case simonGame::none:
		break;
		
		case simonGame::all:
			// TODO we need a sound for that
		break;
	}
	m_playobj = m_factory -> createPlayObject(path, true);
	m_playobj -> play();
}

void artsPlayer::checkEnded()
{
	if (m_playobj -> state() != Arts::posPlaying)
	{
		m_endChecker -> stop();
		emit ended();
		if (m_nextSounds.size() > 0) play();
	}
}

#include "artsplayer.moc"
