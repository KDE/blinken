/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef BLINKEN_H
#define BLINKEN_H

#include <qwidget.h>

#include "blinkengame.h"

class QTimer;

class KAction;
class KHelpMenu;

class button;
class highScoreDialog;

class blinken : public QWidget
{
Q_OBJECT
	public:
		blinken();
		~blinken();
		
	protected:
		void paintEvent(QPaintEvent *);
		void mouseMoveEvent(QMouseEvent *e);
		void mousePressEvent(QMouseEvent *e);
		void keyPressEvent(QKeyEvent *e);
		void keyReleaseEvent(QKeyEvent *e);
		
	private slots:
		void checkHS();
		void highlight(blinkenGame::color c, bool unhighlight);
		void unhighlight();
		
		void pressedYellow();
		void pressedRed();
		void pressedGreen();
		void pressedBlue();
		
	private:
		void selectButton(int button);
		bool insideGreen(const QPoint &p) const;
		bool insideYellow(const QPoint &p) const;
		bool insideRed(const QPoint &p) const;
		bool insideBlue(const QPoint &p) const;
		bool insideButtonsArea(const QPoint &p) const;
		void updateCursor(const QPoint &p);
	
		void drawMenuQuit(QPainter &p);
		void drawScoreAndCounter(QPainter &p);
		void drawStatusText(QPainter &p);
		void drawLevel(QPainter &p);
		void drawText(QPainter &p, const QString &text, const QPoint &center, bool withMargin, int xMargin, int yMargin, QRect *rect, bool highlight, bool bold);
		void updateButtonHighlighting(const QPoint &p);
		
		
		button *m_buttons[4];
		QPixmap *m_back, *m_highscore, *m_highscoreHover, *m_quit, *m_quitHover, *m_menu, *m_menuHover, *m_mark;
		bool m_overHighscore, m_overQuit, m_overCentralText, m_overMenu, m_overAboutKDE, m_overAboutBlinken, m_overManual, m_overLevels[3], m_overCentralLetters, m_overCounter, m_overFont, m_overSound;
		// i obviously suck but m_levelsRect[0] is 2, m_levelsRect[1] is 1 and m_levelsRect[3] is ?
		QRect m_highscoreRect, m_quitRect, m_centralTextRect, m_menuRect, m_aboutKDERect, m_aboutBlinkenRect, m_manualRect, m_levelsRect[3], m_centralLettersRect, m_counterRect, m_soundRect, m_fontRect;
		QColor m_fillColor, m_fontColor, m_fontHighlightColor, m_countDownColor;
		
		// Preferences setting handling
		bool m_showPreferences;
		
		// if should update the highlighting after the next repaint
		bool m_updateButtonHighlighting;
		
		// use always the non-cool font?
		bool m_alwaysUseNonCoolFont;
		
		blinkenGame::color m_highlighted;
		QTimer *m_unhighlighter;
		
		QString m_lastName;
		
		blinkenGame m_game;
		
		KHelpMenu *m_helpMenu;
};

#endif
