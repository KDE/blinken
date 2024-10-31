/*
    SPDX-FileCopyrightText: 2005-2006 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <QObject>

#include <QPair>
#include <QList>
#include <QtQml/qqmlregistration.h>

class HighScoreManager : public QObject
{
Q_OBJECT
QML_ELEMENT
QML_SINGLETON
	public:
		HighScoreManager();
		~HighScoreManager() override;

		QList< QPair<int, QString> > scores(int level) const;

		Q_INVOKABLE bool scoreGoodEnough(int level, int score);
		Q_INVOKABLE void addScore(int level, int score, const QString &name);
		Q_INVOKABLE int score(int level, int position) const;
		Q_INVOKABLE QString name(int level, int position) const;

	private:
		void update();

		QList< QPair<int, QString> > m_scores[3];
};

#endif
