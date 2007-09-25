/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "soundsplayer.h"

#include <klocale.h>
#include <kstandarddirs.h>
#include <kurl.h>

#include <Phonon/Path>

#include "settings.h"

soundsPlayer::soundsPlayer()
    : m_audioOutput(Phonon::GameCategory)
{
	m_audioOutput.setVolume( 0.8f );
	Phonon::createPath(&m_mediaObject, &m_audioOutput);
	connect(&m_mediaObject, SIGNAL(finished()), this, SLOT(playEnded()));

	m_allSound = KStandardDirs::locate("appdata","sounds/lose.wav");
	m_greenSound = KStandardDirs::locate("appdata","sounds/1.wav");
	m_redSound = KStandardDirs::locate("appdata","sounds/2.wav");
	m_blueSound = KStandardDirs::locate("appdata","sounds/3.wav");
	m_yellowSound = KStandardDirs::locate("appdata","sounds/4.wav");

	connect(&m_warnTimer, SIGNAL(timeout()), this, SIGNAL(ended()));
	m_warnTimer.setSingleShot(true);
}

soundsPlayer::~soundsPlayer()
{
}

void soundsPlayer::play(blinkenGame::color c)
{
	if (blinkenSettings::playSounds())
	{
		QString soundFile;
		switch (c)
		{
			case blinkenGame::red:
				soundFile = m_redSound;
			break;
			
			case blinkenGame::green:
				soundFile = m_greenSound;
			break;
			
			case blinkenGame::blue:
				soundFile = m_blueSound;
			break;
			
			case blinkenGame::yellow:
				soundFile = m_yellowSound;
			break;
			
			case blinkenGame::all:
				soundFile = m_allSound;
			break;
			
			default:
			break;
		}
		if (!soundFile.isEmpty())
		{
			m_mediaObject.setCurrentSource(soundFile);
			m_mediaObject.play();
		}
	}
	else
	{
		m_warnTimer.start(250);
	}
}

void soundsPlayer::playEnded()
{
	if (blinkenSettings::playSounds())
	{
		m_warnTimer.start(250);
	}
}

#include "soundsplayer.moc"
