/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KSIMON_H
#define KSIMON_H

#include <qwidget.h>

#include "simongame.h"

class QTimer;

class artsPlayer;

class KSimon : public QWidget
{
Q_OBJECT
	public:
		KSimon();
		~KSimon();
		
	protected:
		void paintEvent(QPaintEvent *);
		void mouseMoveEvent(QMouseEvent *e);
		void mousePressEvent(QMouseEvent *e);
		
	private slots:
		void highlight(simonGame::color c, bool unhighlight);
		void unhighlight();
		
	private:
		void drawMenuQuit(QPainter &p);
		void drawScoreAndCounter(QPainter &p);
		void drawStart(QPainter &p);
		void drawStatusText(QPainter &p);
		void drawLevel(QPainter &p);
		int fontSize(QPainter &p, const QString &s1, int w, int h);
		
		QPixmap *m_back, *m_number1, *m_number2, *m_number3, *m_blueh, *m_yellowh, *m_redh, *m_greenh, *m_menu, *m_menuHover, *m_quit, *m_quitHover;
		bool m_overMenu, m_overQuit, m_overStart;
		QRect m_menuRect, m_quitRect, m_startRect, m_number1Rect, m_number2Rect, m_number3Rect;
		
		simonGame::color m_highlighted;
		QTimer *m_unhighlighter;
		
		artsPlayer *m_artsPlayer;
		simonGame m_game;
};

#endif
