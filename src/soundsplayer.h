/*
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SOUNDSPLAYER_H
#define SOUNDSPLAYER_H

#include <QMap>
#include <QTimer>
#include <QUrl>

#include "blinkengame.h"

class QSoundEffect;

class soundsPlayer : public QObject
{
Q_OBJECT
	public:
		soundsPlayer();
		~soundsPlayer() override;

		void play(blinkenGame::color c);

	Q_SIGNALS:
		void ended();

	private Q_SLOTS:
		void playingChanged();

	private:
		void createEffect(int value, const QString &path);
		QMap<int, QSoundEffect*> m_soundEffects;

		QTimer m_warnTimer;
};

#endif
