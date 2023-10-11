/*
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "soundsplayer.h"

#include "settings.h"

#include <QStandardPaths>
#include <QSoundEffect>

soundsPlayer::soundsPlayer()
{
	createEffect(blinkenGame::all, QStringLiteral("sounds/lose.wav"));
	createEffect(blinkenGame::green, QStringLiteral("sounds/1.wav"));
	createEffect(blinkenGame::red, QStringLiteral("sounds/2.wav"));
	createEffect(blinkenGame::blue, QStringLiteral("sounds/3.wav"));
	createEffect(blinkenGame::yellow, QStringLiteral("sounds/4.wav"));

	connect(&m_warnTimer, &QTimer::timeout, this, &soundsPlayer::ended);
	m_warnTimer.setSingleShot(true);
}

soundsPlayer::~soundsPlayer()
{
}

void soundsPlayer::play(blinkenGame::color c)
{
	if (blinkenSettings::playSounds())
	{
		qDebug() << "play called with color: " << c;
		if (m_soundEffects.contains(c)) {
			m_soundEffects.value(c)->play();
		}
		else
		{
			qWarning() << "Was asked to play color " << c << " but it's not in my map somehow...";
		}
	}
	else
	{
		m_warnTimer.start(250);
	}
}

void soundsPlayer::playingChanged()
{
	QSoundEffect *effect = qobject_cast<QSoundEffect*>(sender());
	// We only need to do something if we are now not playing audio.
	if (!effect->isPlaying()) {
		if (blinkenSettings::playSounds())
		{
			m_warnTimer.start(250);
		}
	}
}

void soundsPlayer::createEffect(int value, const QString &path)
{
	QSoundEffect *effect = new QSoundEffect(this);
	effect->setSource(QUrl::fromLocalFile(QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, path)));
	effect->setVolume(0.8f);
	m_soundEffects.insert(value, effect);
	connect(effect, &QSoundEffect::playingChanged, this, &soundsPlayer::playingChanged);
}

#include "moc_soundsplayer.cpp"
