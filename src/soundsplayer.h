/*
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SOUNDSPLAYER_H
#define SOUNDSPLAYER_H

#include <QtSystemDetection>

#ifdef QML_VERSION
#include <QMediaPlayer>
#include <QAudioOutput>
#else
#include <phonon/MediaObject>
#include <phonon/audiooutput.h>
#endif

#include <QTimer>

#include "blinkengame.h"

class soundsPlayer : public QObject
{
Q_OBJECT
	public:
		soundsPlayer();
		~soundsPlayer() override;
		
		void play(BlinkenGame::Color c);
		
	Q_SIGNALS:
		void ended();
		
	private Q_SLOTS:

#ifdef QML_VERSION
		void soundEffectPlayEnded();
#else
		void playEnded();
#endif
	private:
		void addSoundsFile();
		QString m_greenSound, m_redSound, m_blueSound, m_yellowSound, m_allSound;

#ifdef QML_VERSION
		QMediaPlayer m_soundsPlayer;
#else
		Phonon::MediaObject m_mediaObject;
		Phonon::AudioOutput m_audioOutput;
#endif
		QTimer m_warnTimer;
};

#endif
