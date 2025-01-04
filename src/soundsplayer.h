/*
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SOUNDSPLAYER_H
#define SOUNDSPLAYER_H

#include <QMediaPlayer>

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

		void soundEffectPlayEnded();
	private:
		void addSoundsFile();
		QString m_greenSound, m_redSound, m_blueSound, m_yellowSound, m_allSound;

		QMediaPlayer m_soundsPlayer;
		QTimer m_warnTimer;
};

#endif
