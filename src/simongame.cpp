/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qtimer.h>

#include <kapplication.h>

#include "artsplayer.h"
#include "simongame.h"

simonGame::simonGame() : m_phase(starting)
{
	m_artsPlayer = new artsPlayer;
}

simonGame::~simonGame()
{
	delete m_artsPlayer;
}

simonGame::gamePhase simonGame::phase() const
{
	return m_phase;
}

void simonGame::clicked(color c)
{
	printf("%d %d\n", c, m_sequence.first());
	if (c == m_sequence.first())
	{
		m_sequence.pop_front();
		m_artsPlayer -> play(c);
		
		if (m_sequence.size() == 0)
		{
			m_phase = starting;
			emit phaseChanged();
		}
	}
}

void simonGame::setPhase(gamePhase p)
{
	m_phase = p;
}

void simonGame::start(int level)
{
	m_level = level;
	
	generateSequence();
	
	connect(m_artsPlayer, SIGNAL(ended()), this, SLOT(soundEnded()));
	m_nextSound = m_sequence.begin();
	soundEnded();
}

void simonGame::nextSound()
{
	printf("simonGame::nextSound\n");
	if (m_nextSound != m_sequence.end())
	{
		color c;
		c = *m_nextSound;
		++m_nextSound;
		m_artsPlayer -> play(c);
	}
	else
	{
		m_artsPlayer->disconnect();
		m_phase = typingTheSequence;
		emit phaseChanged();
	}
}

void simonGame::soundEnded()
{
	QTimer::singleShot(100, this, SLOT(nextSound()));
}

void simonGame::generateSequence()
{
	m_sequence.clear();
	
	int r;
	// TODO do something with the level :-D
	for (int i = 0; i < 5; i++)
	{
		r = 1 + (int)(4.0 * kapp -> random() / (RAND_MAX + 1.0));
		switch(r)
		{
			case 1:
				m_sequence.append(red);
			break;
			
			case 2:
				m_sequence.append(green);
			break;
			
			case 3:
				m_sequence.append(blue);
			break;
			
			case 4:
				m_sequence.append(yellow);
			break;
		}
	}
}

#include "simongame.moc"
