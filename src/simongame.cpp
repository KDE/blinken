/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <kapplication.h>

#include "simongame.h"

simonGame::simonGame() : m_phase(starting)
{
}

simonGame::~simonGame()
{
}

simonGame::gamePhase simonGame::phase() const
{
	return m_phase;
}

void simonGame::setPhase(gamePhase p)
{
	m_phase = p;
}

void simonGame::start(int level)
{
	m_level = level;
	
	generateSequence();
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
