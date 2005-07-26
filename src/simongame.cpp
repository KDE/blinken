/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <stdlib.h> // for RAND_MAX

#include <qtimer.h>

#include <kapplication.h>

#include "artsplayer.h"
#include "simongame.h"

simonGame::simonGame() : m_phase(starting)
{
	m_artsPlayer = new artsPlayer;
	m_waitTimer = new QTimer(this);
	connect(m_waitTimer, SIGNAL(timeout()), this, SLOT(waiting()));
}

simonGame::~simonGame()
{
	delete m_artsPlayer;
}

int simonGame::level() const
{
	return m_level;
}

bool simonGame::canType() const
{
	return m_phase == typingTheSequence || m_phase == starting;
}

simonGame::gamePhase simonGame::phase() const
{
	return m_phase;
}

int simonGame::score() const
{
	if (m_phase == starting || m_phase == choosingLevel) return 0;
	return m_sequenceLength - 1;
}

void simonGame::clicked(color c)
{
	if (m_phase == starting)
	{
		m_artsPlayer -> play(c);
		return;
	}
	if (c == *m_nextColor)
	{
		++m_nextColor;
		m_artsPlayer -> play(c);
		
		if (m_nextColor == m_sequence.end())
		{
			m_sequenceLength++;
			nextRound();
		}
	}
	else
	{
		m_artsPlayer -> play(all, true);
		emit highlight(all, true);
		emit gameEnded();
		setPhase(choosingLevel);
	}
}

void simonGame::setPhase(gamePhase p)
{
	if (p != waiting3 && p != waiting2 && p != waiting1) m_waitTimer -> stop();
	m_phase = p;
	emit phaseChanged();
}

void simonGame::start(int level)
{
	m_level = level;
	m_sequenceLength = 1;
	
	nextRound();
	
	m_sequence.clear();
}

void simonGame::nextSound()
{
	if (m_nextColor != m_sequence.end())
	{
		color c;
		c = *m_nextColor;
		++m_nextColor;
		m_artsPlayer -> play(c);
		emit highlight(c, false);
	}
	else
	{
		m_nextColor = m_sequence.begin();
		emit highlight(none, false);
		m_artsPlayer->disconnect();
		setPhase(typingTheSequence);
	}
}

void simonGame::soundEnded()
{
	QTimer::singleShot(100, this, SLOT(nextSound()));
	QTimer::singleShot(50, this, SLOT(unhighlight()));
}

void simonGame::unhighlight()
{
	emit highlight(none, false);
}

void simonGame::waiting()
{
	if (m_phase == waiting1)
	{
		setPhase(simonGame::learningTheSequence);
		if (m_level == 3) 
		{
			m_sequence.clear();
			for (int i = 0; i < m_sequenceLength; i++) m_sequence.append(generateColor());
		}
		else m_sequence.append(generateColor());
	
		connect(m_artsPlayer, SIGNAL(ended()), this, SLOT(soundEnded()));
		m_nextColor = m_sequence.begin();
		soundEnded();
	}
	else if (m_phase == waiting3) setPhase(waiting2);
	else /* m_phase == waiting2 */ setPhase(waiting1);
}

void simonGame::nextRound()
{
	if (m_level == 1) setPhase(waiting3);
	else setPhase(waiting2);
	m_waitTimer -> start(1000);
}

simonGame::color simonGame::generateColor()
{
	int r;
	// make the compiler happy :-D
	color c = none;

	r = 1 + (int)(4.0 * kapp -> random() / (RAND_MAX + 1.0));
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

#include "simongame.moc"
