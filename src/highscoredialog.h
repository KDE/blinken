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

#include <kdialog.h>

class highScoreDialog : public KDialog
{
	public:
		highScoreDialog(QWidget *parent, QValueList< QPair<int, QString> > scores);
	
	protected:
		void mouseMoveEvent(QMouseEvent *e);
		void mousePressEvent(QMouseEvent *);
		void paintEvent(QPaintEvent *);
	
	private:
		bool m_overClose;
		QValueList< QPair<int, QString> > m_scores;
		QRect closeRect;
};

#endif
