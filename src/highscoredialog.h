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

#endif
