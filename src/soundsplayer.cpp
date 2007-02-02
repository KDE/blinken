/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <klocale.h>
#include <kstandarddirs.h>
#include <kurl.h>

#include <phonon/audiopath.h>
#include <phonon/audiooutput.h>

#include "soundsplayer.h"
#include "settings.h"

soundsPlayer::soundsPlayer() : m_currentSound(0)
{
	Phonon::AudioPath* audioPath = new Phonon::AudioPath( this );
	Phonon::AudioOutput* audioOutput = new Phonon::AudioOutput( Phonon::MusicCategory, this );
	audioOutput->setVolume( 0.8f );
	audioPath->addOutput( audioOutput );

	m_allSound.setUrl(KStandardDirs::locate("appdata","sounds/lose.wav"));
	m_greenSound.setUrl(KStandardDirs::locate("appdata","sounds/1.wav"));
	m_redSound.setUrl(KStandardDirs::locate("appdata","sounds/2.wav"));
	m_blueSound.setUrl(KStandardDirs::locate("appdata","sounds/3.wav"));
	m_yellowSound.setUrl(KStandardDirs::locate("appdata","sounds/4.wav"));
	
	m_allSound.addAudioPath( audioPath );
	m_greenSound.addAudioPath( audioPath );
	m_redSound.addAudioPath( audioPath );
	m_blueSound.addAudioPath( audioPath );
	m_yellowSound.addAudioPath( audioPath );
	
	connect(&m_allSound, SIGNAL(finished()), this, SLOT(playEnded()));
	connect(&m_greenSound, SIGNAL(finished()), this, SLOT(playEnded()));
	connect(&m_redSound, SIGNAL(finished()), this, SLOT(playEnded()));
	connect(&m_blueSound, SIGNAL(finished()), this, SLOT(playEnded()));
	connect(&m_yellowSound, SIGNAL(finished()), this, SLOT(playEnded()));
	
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
		switch (c)
		{
			case blinkenGame::red:
				m_currentSound = &m_redSound;
			break;
			
			case blinkenGame::green:
				m_currentSound = &m_greenSound;
			break;
			
			case blinkenGame::blue:
				m_currentSound = &m_blueSound;
			break;
			
			case blinkenGame::yellow:
				m_currentSound = &m_yellowSound;
			break;
			
			case blinkenGame::all:
				m_currentSound = &m_allSound;
			break;
			
			case blinkenGame::none:
				m_currentSound = 0;
			break;
		}
		if (m_currentSound) m_currentSound -> play();
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
		m_currentSound = NULL;
		m_warnTimer.start(250);
	}
}

#include "soundsplayer.moc"
