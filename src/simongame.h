/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef SIMONGAME_H
#define SIMONGAME_H

#include <qvaluelist.h>

class simonGame
{
	public:
		simonGame();
		~simonGame();
		
		enum gamePhase { starting, choosingLevel, learningTheSequence, typingTheSequence };
		enum color { red, green, blue, yellow };
		
		gamePhase phase() const;
		
		void setPhase(gamePhase p);
		void start(int level);
	
	private:
		void generateSequence();
	
		gamePhase m_phase;
		int m_level;
		
		QValueList<color> m_sequence;
};

#endif
