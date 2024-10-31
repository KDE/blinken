/*
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "soundsplayer.h"

#include "settings.h"

#include <QStandardPaths>

#ifdef QML_VERSION
//for QML version
soundsPlayer::soundsPlayer()
{
	addSoundsFile();
	m_soundsPlayer.setAudioOutput(new QAudioOutput);
	connect(&m_soundsPlayer, &QMediaPlayer::playingChanged,this,&soundsPlayer::soundEffectPlayEnded);

	connect(&m_warnTimer, &QTimer::timeout, this, &soundsPlayer::ended);
	m_warnTimer.setSingleShot(true);
}
#else
soundsPlayer::soundsPlayer()
    : m_audioOutput(Phonon::GameCategory)
{
	addSoundsFile();
	m_audioOutput.setVolume( 0.8f );
	Phonon::createPath(&m_mediaObject, &m_audioOutput);
	connect(&m_mediaObject, &Phonon::MediaObject::finished, this, &soundsPlayer::playEnded);

	connect(&m_warnTimer, &QTimer::timeout, this, &soundsPlayer::ended);
	m_warnTimer.setSingleShot(true);
}
#endif



void soundsPlayer::addSoundsFile()
{
#if defined(Q_OS_ANDROID) || defined(QML_VERSION)
	m_allSound=QStringLiteral("qrc:lose.wav");
	m_greenSound=QStringLiteral("qrc:1.wav");
	m_redSound=QStringLiteral("qrc:2.wav");
	m_blueSound=QStringLiteral("qrc:3.wav");
	m_yellowSound=QStringLiteral("qrc:4.wav");
#else
	m_allSound = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, QStringLiteral("sounds/lose.wav"));
	m_greenSound = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, QStringLiteral("sounds/1.wav"));
	m_redSound = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, QStringLiteral("sounds/2.wav"));
	m_blueSound = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, QStringLiteral("sounds/3.wav"));
	m_yellowSound = QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, QStringLiteral("sounds/4.wav"));
#endif
}

soundsPlayer::~soundsPlayer()
{
}

void soundsPlayer::play(BlinkenGame::Color c)
{
#ifdef QML_VERSION
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
#else
	if (BlinkenSettings::playSounds())
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
			m_mediaObject.setCurrentSource(QUrl::fromLocalFile(soundFile));
			m_mediaObject.play();
		}
	}
	else
	{
		m_warnTimer.start(250);
	}
#endif
}

#ifdef QML_VERSION

void soundsPlayer::soundEffectPlayEnded()
{
	if(!m_soundsPlayer.isPlaying())
	{
		m_warnTimer.start(250);
	}
}

#else
void soundsPlayer::playEnded()
{
	if (BlinkenSettings::playSounds())
	{
		m_warnTimer.start(250);
	}
}
#endif


#include "moc_soundsplayer.cpp"
