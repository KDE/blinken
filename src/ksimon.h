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
class highScoreDialog;

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
		void checkHS();
		void highlight(simonGame::color c, bool unhighlight);
		void unhighlight();
		
	private:
		void drawMenuQuit(QPainter &p);
		void drawCentralText(QPainter &p, const QString &text);
		void drawScoreAndCounter(QPainter &p);
		void drawStatusText(QPainter &p);
		void drawLevel(QPainter &p);
		void updateButtonHighlighting(const QPoint &p);
		
		QPixmap *m_back, *m_blueh, *m_yellowh, *m_redh, *m_greenh, *m_menu, *m_menuHover, *m_quit, *m_quitHover;
		bool m_overMenu, m_overQuit, m_overCentralText, m_overLevels[3];
		// i obviously suck but m_levelsRect[0] is 2, m_levelsRect[1] is 1 and m_levelsRect[3] is ?
		QRect m_menuRect, m_quitRect, m_centralTextRect, m_levelsRect[3];
		QColor m_fillColor, m_fontColor, m_fontHighlightColor, m_countDownColor;
		
		// if should update the highlighting after the next repaint
		bool m_updateButtonHighlighting;
		
		simonGame::color m_highlighted;
		QTimer *m_unhighlighter;
		
		QString m_lastName;
		
		artsPlayer *m_artsPlayer;
		simonGame m_game;
};

#endif
