/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef HIGHSCOREDIALOG_H
#define HIGHSCOREDIALOG_H

#include <qpair.h>
#include <qvaluelist.h>

#include <kdialogbase.h>

class myTabWidget;

class highScoreDialog : private KDialogBase
{
	public:
		highScoreDialog(QWidget *parent);
		
		bool scoreGoodEnough(int level, int score);
		void addScore(int level, int score, const QString &name);
		void showLevel(int level);
	
		QValueList< QPair<int, QString> > m_scores[3];
		myTabWidget *m_tw;
};

#endif
