/*
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SOUNDSPLAYER_H
#define SOUNDSPLAYER_H

#include <phonon/MediaObject>
#include <phonon/audiooutput.h>

#include <QTimer>

#include "blinkengame.h"

class soundsPlayer : public QObject
{
Q_OBJECT
	public:
		soundsPlayer();
		~soundsPlayer();
		
		void play(blinkenGame::color c);
		
	Q_SIGNALS:
		void ended();
		
	private Q_SLOTS:
		void playEnded();
		
	private:
		QString m_greenSound, m_redSound, m_blueSound, m_yellowSound, m_allSound;
		Phonon::MediaObject m_mediaObject;
		Phonon::AudioOutput m_audioOutput;
		
		QTimer m_warnTimer;
};

#endif
