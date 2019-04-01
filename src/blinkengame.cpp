/***************************************************************************
 *   Copyright (C) 2005-2006 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "blinkengame.h"

#include <stdlib.h> // for RAND_MAX

#include <QTimer>

#include <krandom.h>
#include "soundsplayer.h"

blinkenGame::blinkenGame() : m_phase(starting)
{
	m_soundsPlayer = new soundsPlayer;
	m_waitTimer = new QTimer(this);
	connect(m_waitTimer, &QTimer::timeout, this, &blinkenGame::waiting);
}

blinkenGame::~blinkenGame()
{
	delete m_soundsPlayer;
}

int blinkenGame::level() const
{
	return m_level;
}

bool blinkenGame::canType() const
{
	return m_phase == typingTheSequence || m_phase == starting;
}

blinkenGame::gamePhase blinkenGame::phase() const
{
	return m_phase;
}

int blinkenGame::score() const
{
	if (m_phase == starting || m_phase == choosingLevel) return 0;
	return m_sequenceLength - 1;
}

void blinkenGame::clicked(color c)
{
	if (m_phase == starting)
	{
		m_soundsPlayer -> play(c);
		return;
	}
	if (c == *m_nextColor)
	{
		++m_nextColor;
		m_soundsPlayer -> play(c);
		
		if (m_nextColor == m_sequence.constEnd())
		{
			m_sequenceLength++;
			nextRound();
		}
	}
	else
	{
		m_soundsPlayer -> play(all);
		emit highlight(all, true);
		emit gameEnded();
		setPhase(choosingLevel);
	}
}

void blinkenGame::setPhase(gamePhase p)
{
	if (p != waiting3 && p != waiting2 && p != waiting1) m_waitTimer -> stop();
	m_phase = p;
	emit phaseChanged();
}

void blinkenGame::start(int level)
{
	m_level = level;
	m_sequenceLength = 1;
	
	nextRound();
	
	m_sequence.clear();
}

void blinkenGame::nextSound()
{
	if (m_nextColor != m_sequence.constEnd())
	{
		color c;
		c = *m_nextColor;
		++m_nextColor;
		m_soundsPlayer -> play(c);
		emit highlight(c, false);
	}
	else
	{
		setPhase(typingTheSequence);
		m_nextColor = m_sequence.constBegin();
		emit highlight(none, false);
		m_soundsPlayer->disconnect();
	}
}

void blinkenGame::soundEnded()
{
	QTimer::singleShot(100, this, &blinkenGame::nextSound);
	QTimer::singleShot(50, this, &blinkenGame::unhighlight);
}

void blinkenGame::unhighlight()
{
	emit highlight(none, false);
}

void blinkenGame::waiting()
{
	if (m_phase == waiting1)
	{
		setPhase(blinkenGame::learningTheSequence);
		if (m_level == 3) 
		{
			m_sequence.clear();
			for (int i = 0; i < m_sequenceLength; i++) m_sequence.append(generateColor());
		}
		else m_sequence.append(generateColor());
	
		connect(m_soundsPlayer, &soundsPlayer::ended, this, &blinkenGame::soundEnded);
		m_nextColor = m_sequence.constBegin();
		soundEnded();
	}
	else if (m_phase == waiting3) setPhase(waiting2);
	else /* m_phase == waiting2 */ setPhase(waiting1);
}

void blinkenGame::nextRound()
{
	if (m_level == 1) setPhase(waiting3);
	else setPhase(waiting2);
	m_waitTimer -> start(1000);
}

blinkenGame::color blinkenGame::generateColor()
{
	int r;
	// make the compiler happy :-D
	color c = none;

	r = 1 + (int)(4.0 * KRandom::random() / (RAND_MAX + 1.0));
	switch(r)
	{
		case 1:
			c = red;
		break;
		
		case 2:
			c = green;
		break;
		
		case 3:
			c = blue;
		break;
		
		case 4:
			c = yellow;
		break;
	}
	return c;
}

