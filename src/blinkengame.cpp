/*
    SPDX-FileCopyrightText: 2005-2006 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "blinkengame.h"

#include <QRandomGenerator>
#include <QTimer>

#include "soundsplayer.h"

BlinkenGame::BlinkenGame() : m_phase(Starting)
{
	m_soundsPlayer = new soundsPlayer;
	m_waitTimer = new QTimer(this);
	connect(m_waitTimer, &QTimer::timeout, this, &BlinkenGame::waiting);
}

BlinkenGame::~BlinkenGame()
{
	delete m_soundsPlayer;
}

int BlinkenGame::level() const
{
	return m_level;
}

bool BlinkenGame::canType() const
{
	return m_phase == TypingTheSequence || m_phase == Starting;
}

BlinkenGame::GamePhase BlinkenGame::phase() const
{
	return m_phase;
}

int BlinkenGame::score() const
{
	if (m_phase == Starting || m_phase == ChoosingLevel) return 0;
	return m_sequenceLength - 1;
}

void BlinkenGame::clicked(Color c)
{
	if (m_phase == Starting)
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
		m_soundsPlayer -> play(All);
		Q_EMIT highlight(All, true);
		Q_EMIT gameEnded();
		setPhase(ChoosingLevel);
	}
}

void BlinkenGame::setPhase(GamePhase p)
{
	if (p != Waiting3 && p != Waiting2 && p != Waiting1) m_waitTimer -> stop();
	m_phase = p;
	Q_EMIT phaseChanged();
}

void BlinkenGame::start(int level)
{
	m_level = level;
	m_sequenceLength = 1;
	
	nextRound();
	
	m_sequence.clear();
}

void BlinkenGame::nextSound()
{
	if (m_nextColor != m_sequence.constEnd())
	{
		Color c;
		c = *m_nextColor;
		++m_nextColor;
		m_soundsPlayer -> play(c);
		Q_EMIT highlight(c, false);
	}
	else
	{
		setPhase(TypingTheSequence);
		m_nextColor = m_sequence.constBegin();
		Q_EMIT highlight(None, false);
		m_soundsPlayer->disconnect();
	}
}

void BlinkenGame::soundEnded()
{
	QTimer::singleShot(100, this, &BlinkenGame::nextSound);
	QTimer::singleShot(50, this, &BlinkenGame::unhighlight);
}

void BlinkenGame::unhighlight()
{
	Q_EMIT highlight(None, false);
}

void BlinkenGame::waiting()
{
	if (m_phase == Waiting1)
	{
		setPhase(BlinkenGame::LearningTheSequence);
		if (m_level == 3) 
		{
			m_sequence.clear();
			for (int i = 0; i < m_sequenceLength; i++) m_sequence.append(generateColor());
		}
		else m_sequence.append(generateColor());
	
		connect(m_soundsPlayer, &soundsPlayer::ended, this, &BlinkenGame::soundEnded);
		m_nextColor = m_sequence.constBegin();
		soundEnded();
	}
	else if (m_phase == Waiting3) setPhase(Waiting2);
	else /* m_phase == waiting2 */ setPhase(Waiting1);
}

void BlinkenGame::nextRound()
{
	if (m_level == 1) setPhase(Waiting3);
	else setPhase(Waiting2);
	m_waitTimer -> start(1000);
}

BlinkenGame::Color BlinkenGame::generateColor()
{
	// make the compiler happy :-D
	Color c = None;

	const int r = QRandomGenerator::global()->bounded(1, 5); // rand [1, 5)
	switch(r)
	{
		case 1:
			c = Red;
		break;
		
		case 2:
			c = Green;
		break;
		
		case 3:
			c = Blue;
		break;
		
		case 4:
			c = Yellow;
		break;
	}
	return c;
}

#include "moc_blinkengame.cpp"
