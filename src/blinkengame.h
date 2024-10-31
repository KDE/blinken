/*
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef BLINKENGAME_H
#define BLINKENGAME_H

#include <QObject>
#include <QList>

#ifdef QML_VERSION
#include <QQmlEngine>
#endif

#include <QtQml/qqmlregistration.h>

class QTimer;

class soundsPlayer;

class BlinkenGame : public QObject
{
Q_OBJECT
QML_ELEMENT
QML_SINGLETON
	public:
		BlinkenGame();
		~BlinkenGame() override;

		enum GamePhase { 
			Starting, 
			ChoosingLevel, 
			Waiting3, 
			Waiting2, 
			Waiting1, 
			LearningTheSequence, 
			TypingTheSequence };
		Q_ENUM(GamePhase)

		enum Color
		{
			None = 0,
			Red = 1,
			Green = 2,
			Blue = 4,
			Yellow = 8,
			All = 15};
		Q_ENUM(Color)
		Q_PROPERTY(GamePhase phase READ phase WRITE setPhase NOTIFY phaseChanged)

		Q_INVOKABLE int level() const;
		Q_INVOKABLE bool canType() const;
		Q_INVOKABLE GamePhase phase() const;
		Q_INVOKABLE int score() const;
		Q_INVOKABLE void clicked(Color c);
		Q_INVOKABLE void setPhase(GamePhase p);
		Q_INVOKABLE void start(int level);

		Q_SIGNALS:
			void gameEnded();
			void phaseChanged();
			void highlight(BlinkenGame::Color c, bool unhighlight);

	private Q_SLOTS:
		void nextSound();
		void soundEnded();
		void unhighlight();
		void waiting();
		
	private:
		void nextRound();
		Color generateColor();

		GamePhase m_phase;
		int m_level;
		int m_sequenceLength;
		
		QTimer *m_waitTimer;
		
		soundsPlayer *m_soundsPlayer;
		QList<Color> m_sequence;
		QList<Color>::const_iterator m_nextColor;
};

#endif
