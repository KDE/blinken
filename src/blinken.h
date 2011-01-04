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

#include <tqwidget.h>

#include "blinkengame.h"

class TQTimer;

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
		void paintEvent(TQPaintEvent *);
		void mouseMoveEvent(TQMouseEvent *e);
		void mousePressEvent(TQMouseEvent *e);
		void keyPressEvent(TQKeyEvent *e);
		void keyReleaseEvent(TQKeyEvent *e);
		
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
		bool insideGreen(const TQPoint &p) const;
		bool insideYellow(const TQPoint &p) const;
		bool insideRed(const TQPoint &p) const;
		bool insideBlue(const TQPoint &p) const;
		bool insideButtonsArea(const TQPoint &p) const;
		void updateCursor(const TQPoint &p);
	
		void drawMenuQuit(TQPainter &p);
		void drawScoreAndCounter(TQPainter &p);
		void drawtqStatusText(TQPainter &p);
		void drawLevel(TQPainter &p);
		void drawText(TQPainter &p, const TQString &text, const TQPoint &center, bool withMargin, int xMargin, int yMargin, TQRect *rect, bool highlight, bool bold);
		void updateButtonHighlighting(const TQPoint &p);
		
		
		button *m_buttons[4];
		TQPixmap *m_back, *m_highscore, *m_highscoreHover, *m_quit, *m_quitHover, *m_menu, *m_menuHover, *m_mark;
		bool m_overHighscore, m_overQuit, m_overCentralText, m_overMenu, m_overAboutKDE, m_overAboutBlinken, m_overManual, m_overLevels[3], m_overCentralLetters, m_overCounter, m_overFont, m_overSound;
		// i obviously suck but m_levelsRect[0] is 2, m_levelsRect[1] is 1 and m_levelsRect[3] is ?
		TQRect m_highscoreRect, m_quitRect, m_centralTextRect, m_menuRect, m_aboutKDERect, m_aboutBlinkenRect, m_manualRect, m_levelsRect[3], m_centralLettersRect, m_counterRect, m_soundRect, m_fontRect;
		TQColor m_fillColor, m_fontColor, m_fontHighlightColor, m_countDownColor;
		
		// Preferences setting handling
		bool m_showPreferences;
		
		// if should update the highlighting after the next tqrepaint
		bool m_updateButtonHighlighting;
		
		// use always the non-cool font?
		bool m_alwaysUseNonCoolFont;
		
		blinkenGame::color m_highlighted;
		TQTimer *m_unhighlighter;
		
		TQString m_lastName;
		
		blinkenGame m_game;
		
		KHelpMenu *m_helpMenu;
};

#endif
