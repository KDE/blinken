/*
    SPDX-FileCopyrightText: 2005-2006 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef HIGHSCOREDIALOG_H
#define HIGHSCOREDIALOG_H

#include <QDialog>

#include <QPair>
#include <QList>

class QSvgRenderer;

class myTabWidget;

class highScoreDialog : private QDialog
{
	public:
		highScoreDialog(QWidget *parent, QSvgRenderer *renderer);
		
		void showLevel(int level);
	
		myTabWidget *m_tw;
};

class highScoreManager : public QObject
{
Q_OBJECT
	public:
		highScoreManager();
		~highScoreManager();

		bool scoreGoodEnough(int level, int score);
		void addScore(int level, int score, const QString &name);

		QList< QPair<int, QString> > scores(int level) const;

		Q_INVOKABLE int score(int level, int position) const;
		Q_INVOKABLE QString name(int level, int position) const;

	private:
		void update();

		QList< QPair<int, QString> > m_scores[3];
};

#endif
