/*
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "soundsplayer.h"

#include "settings.h"

#include <QAudioOutput>

soundsPlayer::soundsPlayer()
{
	addSoundsFile();
	m_soundsPlayer.setAudioOutput(new QAudioOutput);
	connect(&m_soundsPlayer, &QMediaPlayer::playingChanged,this,&soundsPlayer::soundEffectPlayEnded);

	connect(&m_warnTimer, &QTimer::timeout, this, &soundsPlayer::ended);
	m_warnTimer.setSingleShot(true);
}

void soundsPlayer::addSoundsFile()
{
	m_allSound=QStringLiteral("qrc:lose.wav");
	m_greenSound=QStringLiteral("qrc:1.wav");
	m_redSound=QStringLiteral("qrc:2.wav");
	m_blueSound=QStringLiteral("qrc:3.wav");
	m_yellowSound=QStringLiteral("qrc:4.wav");
}

soundsPlayer::~soundsPlayer()
{
}

void soundsPlayer::play(BlinkenGame::Color c)
{
	//QML version
	if(m_soundsPlayer.isPlaying())return;
	if(BlinkenSettings::playSounds())
	{
		QString soundFile;
		switch (c)
		{
			case BlinkenGame::Red:
				soundFile = m_redSound;
			break;

			case BlinkenGame::Green:
				soundFile = m_greenSound;
			break;

			case BlinkenGame::Blue:
				soundFile = m_blueSound;
			break;

			case BlinkenGame::Yellow:
				soundFile = m_yellowSound;
			break;

			case BlinkenGame::All:
				soundFile = m_allSound;
			break;

			default:
			break;
		}

		if (!soundFile.isEmpty())
		{
			m_soundsPlayer.setSource(QUrl(soundFile));
			m_soundsPlayer.setLoops(1);
			m_soundsPlayer.play();
		}else{
			
			qDebug()<<"can't find sound file "<<soundFile;
		}
	}else{
		m_warnTimer.start(250);
	}
}

void soundsPlayer::soundEffectPlayEnded()
{
	if(!m_soundsPlayer.isPlaying())
	{
		m_warnTimer.start(250);
	}
}

#include "moc_soundsplayer.cpp"
