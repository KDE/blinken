/***************************************************************************
 *   Copyright (C) 2005-2006 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef BLINKEN_H
#define BLINKEN_H

#include <QMap>
#include <kmainwindow.h>

#include "blinkengame.h"

class QSvgRenderer;
class QTimer;

class KHelpMenu;

class button;

class blinken : public KMainWindow
{
Q_OBJECT
	public:
		blinken();
		~blinken();

		QSize sizeHint() const override;
		
	protected:
		void paintEvent(QPaintEvent *) override;
		void mouseMoveEvent(QMouseEvent *e) override;
		void mousePressEvent(QMouseEvent *e) override;
		void keyPressEvent(QKeyEvent *e) override;
		void keyReleaseEvent(QKeyEvent *e) override;
		
	private Q_SLOTS:
		void checkHS();
		void highlight(blinkenGame::color c, bool unhighlight);
		void unhighlight();
		
		void pressedColor(blinkenGame::color c);

	private:
		void startGamePressed();
		void startGameAtLevel(int level);
		void selectButton(int button);
		bool insideGreen(const QPointF &p) const;
		bool insideYellow(const QPointF &p) const;
		bool insideRed(const QPointF &p) const;
		bool insideBlue(const QPointF &p) const;
		bool insideButtonsArea(const QPointF &p) const;
		void updateCursor(const QPoint &p);
	
		void drawMenuQuit(QPainter &p);
		void drawScoreAndCounter(QPainter &p);
		void drawStatusText(QPainter &p);
		void drawLevel(QPainter &p);
		void drawText(QPainter &p, const QString &text, const QPointF &center, bool withMargin, double xMargin, double yMargin, QRectF *rect, bool highlight, bool bold);
		void updateButtonHighlighting(const QPoint &p);
		QPixmap getPixmap(const QString &element, const QSize &imageSize);
		
		void togglePreferences();
		
		
		button *m_buttons[4];
		QSvgRenderer *m_renderer;
		bool m_overHighscore, m_overQuit, m_overCentralText, m_overMenu, m_overAboutKDE, m_overAboutBlinken, m_overSettings, m_overManual, m_overLevels[3], m_overCentralLetters, m_overCounter, m_overFont, m_overSound;
		// i obviously suck but m_levelsRect[0] is 2, m_levelsRect[1] is 1 and m_levelsRect[3] is ?
		QRect m_highscoreRect, m_quitRect, m_menuRect, m_aboutKDERect, m_aboutBlinkenRect, m_settingsRect, m_manualRect, m_soundRect, m_fontRect;
		QRectF m_centralTextRect, m_levelsRect[3], m_centralLettersRect, m_counterRect;
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
		
		QSize m_lastSize;
		QMap<QString, QPixmap> m_pixmapCache;
};

#endif
