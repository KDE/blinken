/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qtimer.h>

#include "artsplayer.h"

artsPlayer::artsPlayer() : m_playobj(0)
{
	m_dispatcher = new KArtsDispatcher;
	m_server = new KArtsServer;
	m_factory = new KDE::PlayObjectFactory(m_server->server());
}

artsPlayer::~artsPlayer()
{
	delete m_factory;
	delete m_server;
	delete m_dispatcher;
}

void artsPlayer::play(const QString &path, bool stopCurrent)
{
	if (m_playobj && m_playobj -> state() == Arts::posPlaying)
	{
		if (stopCurrent)
		{
			m_nextSounds.clear();
			m_nextSounds.append(path);
			m_playobj -> halt();
			play();
		}
		else
		{
			m_nextSounds.append(path);
			QTimer::singleShot(150, this, SLOT(play()));
		}
	}
	else
	{
		m_nextSounds.append(path);
		play();
	}
}

void artsPlayer::play()
{
	if (m_playobj && m_playobj -> state() == Arts::posPlaying)
	{
		QTimer::singleShot(150, this, SLOT(play()));
	}
	else
	{
		QString path = m_nextSounds.first();
		m_nextSounds.pop_front();
		m_playobj = m_factory -> createPlayObject(path, true);
		m_playobj -> play();
	}
}

#include "artsplayer.moc"
