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

class artsPlayer;

class simonGame : public QObject
{
Q_OBJECT
	public:
		simonGame();
		~simonGame();
		
		enum gamePhase { starting, choosingLevel, learningTheSequence, typingTheSequence };
		enum color { none, red, green, blue, yellow, all };
		
		gamePhase phase() const;
		
		void clicked(color c);
		void setPhase(gamePhase p);
		void start(int level);
	
	signals:
		void phaseChanged();
		void highlight(simonGame::color c, bool unhighlight);
		
	private slots:
		void nextSound();
		void soundEnded();
		void unhighlight();
		
	private:
		void generateSequence();
	
		gamePhase m_phase;
		int m_level;
		
		artsPlayer *m_artsPlayer;
		QValueList<color> m_sequence;
		QValueList<color>::const_iterator m_nextSound;
};

#endif
