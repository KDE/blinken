/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef BLINKENGAME_H
#define BLINKENGAME_H

#include <QObject>
#include <QList>

class QTimer;

class soundsPlayer;

class blinkenGame : public QObject
{
Q_OBJECT
	public:
		blinkenGame();
		~blinkenGame();
		
		enum gamePhase { starting, choosingLevel, waiting3, waiting2, waiting1, learningTheSequence, typingTheSequence };
		enum color
		{
			none = 0,
			red = 1,
			green = 2,
			blue = 4,
			yellow = 8,
			all = 15};
		
		int level() const;
		bool canType() const;
		gamePhase phase() const;
		int score() const;
		
		void clicked(color c);
		void setPhase(gamePhase p);
		void start(int level);
	
	Q_SIGNALS:
		void gameEnded();
		void phaseChanged();
		void highlight(blinkenGame::color c, bool unhighlight);
		
	private Q_SLOTS:
		void nextSound();
		void soundEnded();
		void unhighlight();
		void waiting();
		
	private:
		void nextRound();
		color generateColor();
	
		gamePhase m_phase;
		int m_level;
		int m_sequenceLength;
		
		QTimer *m_waitTimer;
		
		soundsPlayer *m_soundsPlayer;
		QList<color> m_sequence;
		QList<color>::const_iterator m_nextColor;
};

#endif
