/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qcursor.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qtimer.h>

#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include "ksimon.h"
#include "number.h"

KSimon::KSimon() : QWidget(0, 0, WStaticContents | WNoAutoErase), m_overMenu(false), m_overQuit(false), m_overStart(false), m_updateHighlight(false), m_highlighted(simonGame::none)
{
	m_back = new QPixmap(locate("appdata", "images/ksimon.png"));
	m_blueh = new QPixmap(locate("appdata", "images/blueh.png"));
	m_yellowh = new QPixmap(locate("appdata", "images/yellowh.png"));
	m_redh = new QPixmap(locate("appdata", "images/redh.png"));
	m_greenh = new QPixmap(locate("appdata", "images/greenh.png"));
	
	m_menu = new QPixmap(locate("appdata", "images/menu.png"));
	m_menuHover = new QPixmap(locate("appdata", "images/menu_hover.png"));
	m_quit = new QPixmap(locate("appdata", "images/quit.png"));
	m_quitHover = new QPixmap(locate("appdata", "images/quit_hover.png"));
	m_menuRect = QRect(10, 10, 72, 73);
	m_quitRect = QRect(560, 10, 72, 73);
	
	m_fillColor = QColor(40, 40, 40);
	m_fontColor = QColor(126, 126, 126);
	m_fontHighlightColor  = QColor(230, 230, 230);
	m_countDownColor = QColor(55, 55, 55);
	
	setMouseTracking(true);
	setFixedSize(644, 525);
	show();
	
	m_unhighlighter = new QTimer(this);
	connect(m_unhighlighter, SIGNAL(timeout()), this, SLOT(unhighlight()));
	
	connect(&m_game, SIGNAL(phaseChanged()), this, SLOT(update()));
	connect(&m_game, SIGNAL(highlight(simonGame::color, bool)), this, SLOT(highlight(simonGame::color, bool)));
}

KSimon::~KSimon()
{
	delete m_back;
	delete m_blueh;
	delete m_greenh;
	delete m_redh;
	delete m_yellowh;
	delete m_menu;
	delete m_menuHover;
	delete m_quit;
	delete m_quitHover;
}

void KSimon::paintEvent(QPaintEvent *)
{
	QPixmap buf(width(), height());
	QPainter p(&buf);
	
	p.drawPixmap(0, 0, *m_back);
	
	drawMenuQuit(p);
	switch (m_game.phase())
	{
		case simonGame::starting:
			drawStart(p);
		break;
		
		case simonGame::choosingLevel:
			drawLevel(p);
		break;
		
		default:
		break;
	}
	
	drawScoreAndCounter(p);
	
	switch(m_highlighted)
	{
		case simonGame::none:
		break;
		
		case simonGame::blue:
			p.drawPixmap(14, 225, *m_blueh);
		break;
		
		case simonGame::yellow:
			p.drawPixmap(14, 16, *m_yellowh);
		break;
		
		case simonGame::red:
			p.drawPixmap(322, 16, *m_redh);
		break;
		
		case simonGame::green:
			p.drawPixmap(322, 225, *m_greenh);
		break;
		
		case simonGame::all:
			p.drawPixmap(14, 225, *m_blueh);
			p.drawPixmap(14, 16, *m_yellowh);
			p.drawPixmap(322, 16, *m_redh);
			p.drawPixmap(322, 225, *m_greenh);
		break;
	}
	
	drawStatusText(p);
	
	bitBlt(this, 0, 0, &buf);
	
	if (m_updateHighlight) updateHighlighting(mapFromGlobal(QCursor::pos()));
}

void KSimon::mouseMoveEvent(QMouseEvent *e)
{
	updateHighlighting(e->pos());
}

void KSimon::mousePressEvent(QMouseEvent *e)
{
	if (m_overMenu) KMessageBox::information(this, i18n("This is a code mockup for KSimon project"), i18n("Help"));
	else if (m_overQuit) kapp->quit();
	else if (m_game.phase() == 0 && m_overStart)
	{
		m_overStart = false;
		for(int i = 0; i < 3; i++) m_overLevels[i] = false;
		m_game.setPhase(simonGame::choosingLevel);
		m_updateHighlight = true;
	}
	else if (m_game.phase() == simonGame::choosingLevel)
	{
		int level = 0;
		if (m_levelsRect[1].contains(e -> pos())) level = 1;
		else if (m_levelsRect[0].contains(e -> pos())) level = 2;
		else if (m_levelsRect[2].contains(e -> pos())) level = 3;
		if (level) m_game.start(level);
	}
	else if (m_game.phase() == simonGame::typingTheSequence)
	{
		double x, y, x2, y2;
		x = e -> x() - 319;
		y = e -> y() - 221.5;
		x2 = x * x;
		y2 = y * y;
		if (x2 + y2 > 162.5 * 162.5)
		{
			// Outside the circle
			double x3, y3;
			x3 = x2 / (301 * 301);
			y3 = y2 / (201 * 201);
			if (x3 + y3 < 1)
			{
				// Outside the circle and inside the ellipse
				if (x > 6 && y > 6)
				{
					highlight(simonGame::green, true);
					m_game.clicked(simonGame::green);
				}
				else if (x < -6 && y > 6)
				{
					highlight(simonGame::blue, true);
					m_game.clicked(simonGame::blue);
				}
				else if (x < -6 && y < -6)
				{
					highlight(simonGame::yellow, true);
					m_game.clicked(simonGame::yellow);
				}
				else if (x > 6 && y < -6)
				{
					highlight(simonGame::red, true);
					m_game.clicked(simonGame::red);
				}
			}
		}
	}
}

void KSimon::highlight(simonGame::color c, bool unhighlight)
{
	m_highlighted = c;
	update();
	if (unhighlight) m_unhighlighter -> start(250);
	else if (c == simonGame::none) m_unhighlighter -> stop();
}

void KSimon::unhighlight()
{
	highlight(simonGame::none, false);
}

void KSimon::drawMenuQuit(QPainter &p)
{
	if (!m_overMenu) p.drawPixmap(10, 10, *m_menu);
	else p.drawPixmap(10, 10, *m_menuHover);
	
	if (!m_overQuit) p.drawPixmap(560, 10, *m_quit);
	else p.drawPixmap(560, 10, *m_quitHover);
}

void KSimon::drawScoreAndCounter(QPainter &p)
{
	QColor c1, c2, c3;
	p.translate(313, 125);
	p.setPen(QPen(black, 3));
	p.fillRect(-44, -13, 98, 48, m_fillColor);
	p.drawRoundRect(-45, -15, 100, 50, 15, 15);
	
	if (m_game.phase() != simonGame::starting)
	{
		number n(m_game.score());
		n.paint(p, 2);
	}
	
	switch (m_game.phase())
	{
		case simonGame::waiting3:
			c1 = red;
			c2 = red;
			c3 = red;
		break;
		
		case simonGame::waiting2:
			c1 = m_countDownColor;
			c2 = red;
			c3 = red;
		break;
		
		case simonGame::waiting1:
			c1 = m_countDownColor;
			c2 = c1;
			c3 = red;
		break;
		
		default:
			c1 = m_countDownColor;
			c2 = c1;
			c3 = c1;
		break;
	}
	
	p.fillRect(35, -6, 11, 9, c1);
	p.fillRect(35, 6, 11, 9, c2);
	p.fillRect(35, 18, 11, 9, c3);
	p.translate(-313, -125);
}

void KSimon::drawStart(QPainter &p)
{
	QString start = i18n("Start");

	QFont f = p.font();
	f.setPointSize(fontSize(p, start, 190, 30));
	p.setFont(f);
	
	m_startRect = p.boundingRect(QRect(), Qt::AlignAuto, start);
	m_startRect = QRect(0, 0, m_startRect.width() + 10, m_startRect.height() + 5);
	m_startRect.moveBy(318 - m_startRect.width() / 2, 316 - m_startRect.height() / 2);
	
	p.fillRect(m_startRect, m_fillColor);
	p.setPen(QPen(black, 3));
	p.drawRoundRect(m_startRect.left(), m_startRect.top(), m_startRect.width(), m_startRect.height(), 15, 15);
	
	if (!m_overStart) p.setPen(m_fontColor);
	else p.setPen(m_fontHighlightColor);
	p.drawText(m_startRect, Qt::AlignCenter, start);
}

void KSimon::drawStatusText(QPainter &p)
{
	QFont f = QFont("Steve");
	f.setPointSize(20);
	p.setFont(f);
	
	p.translate(25, 505);
	p.rotate(-3.29);
	p.setPen(blue);
	switch (m_game.phase())
	{
		case simonGame::starting:
			p.drawText(0, 0, i18n("Press Start to begin!"));
		break;
		
		case simonGame::choosingLevel:
			p.drawText(0, 0, i18n("Set the Difficulty Level..."));
		break;
		
		case simonGame::waiting3:
			p.drawText(0, 0, i18n("Next sequence in 3..."));
		break;
		
		case simonGame::waiting2:
			if (m_game.level() == 1) p.drawText(0, 0, i18n("Next sequence in 3, 2..."));
			else p.drawText(0, 0, i18n("Next sequence in 2..."));
		break;
		
		case simonGame::waiting1:
			if (m_game.level() == 1) p.drawText(0, 0, i18n("Next sequence in 3, 2, 1..."));
			else p.drawText(0, 0, i18n("Next sequence in 2, 1..."));
		break;
		
		case simonGame::learningTheSequence:
			p.drawText(0, 0, i18n("Remember this sequence..."));
		break;
		
		case simonGame::typingTheSequence:
			p.drawText(0, 0, i18n("Repeat the sequence!"));
		break;
	}
}

void KSimon::drawLevel(QPainter &p)
{
	QString level = i18n("Level");
	QString n[3];
	n[0] = i18n("2");
	n[1] = i18n("1");
	n[2] = i18n("?");
	
	QFont oldFont, f = p.font();
	oldFont = f;
	f.setPointSize(fontSize(p, level, 190, 30));
	f.setBold(true);
	p.setFont(f);
	p.setPen(m_fontColor);
	
	QRect aux;
	aux = p.boundingRect(QRect(), Qt::AlignAuto, level);
	aux.moveBy(322 - aux.width() / 2, 281 - aux.height() / 2);
	p.drawText(aux, Qt::AlignAuto, level);
	
	for (int i = 0; i < 3; i++)
	{
		m_levelsRect[i] = p.boundingRect(QRect(), Qt::AlignAuto, n[i]);
		m_levelsRect[i] = QRect(0, 0, m_levelsRect[i].width() + 20, m_levelsRect[i].height() + 5);
		if (i == 0) m_levelsRect[0].moveBy(319 - m_levelsRect[0].width() / 2, 315 - m_levelsRect[0].height() / 2);
		else if (i == 1) m_levelsRect[1].moveBy(m_levelsRect[0].left() - m_levelsRect[0].width() - m_levelsRect[1].width() / 2, 315 - m_levelsRect[1].height() / 2);
		else if (i == 2) m_levelsRect[2].moveBy(m_levelsRect[0].left() + 2 * m_levelsRect[0].width() - m_levelsRect[2].width() / 2, 315 - m_levelsRect[2].height() / 2);
	
		p.fillRect(m_levelsRect[i], m_fillColor);
		p.setPen(QPen(black, 3));
		p.drawRoundRect(m_levelsRect[i].left(), m_levelsRect[i].top(), m_levelsRect[i].width(), m_levelsRect[i].height(), 15, 15);
	
		if (m_overLevels[i]) p.setPen(m_fontHighlightColor);
		else p.setPen(m_fontColor);
		p.drawText(m_levelsRect[i], Qt::AlignCenter, n[i]);
	}
	
	p.setFont(oldFont);
}

int KSimon::fontSize(QPainter &p, const QString &s1, int w, int h)
{
	QRect aux1;
	QFont f = p.font();
	f.setPointSize(28);
	p.setFont(f);
	
	aux1 = p.boundingRect(QRect(), Qt::AlignAuto, s1);
	
	return QMIN(w * 28 / aux1.width(), h * 28 / aux1.height());
}

void KSimon::updateHighlighting(const QPoint &p)
{
	m_updateHighlight = false;
	if (m_menuRect.contains(p))
	{
		if (!m_overMenu)
		{
			m_overMenu = true;
			update();
		}
	}
	else if (m_quitRect.contains(p))
	{
		if (!m_overQuit)
		{
			m_overQuit = true;
			update();
		}
	}
	else if (m_game.phase() == simonGame::starting && m_startRect.contains(p))
	{
		if (!m_overStart)
		{
			m_overStart = true;
			update();
		}
	}
	else if (m_game.phase() == simonGame::choosingLevel)
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_levelsRect[i].contains(p))
			{
				if (!m_overLevels[i])
				{
					m_overLevels[i] = true;
					update();
				}
			}
			else
			{
				if (m_overLevels[i])
				{
					m_overLevels[i] = false;
					update();
				}
			}
		}
	}
	else
	{
		if (m_overMenu)
		{
			m_overMenu = false;
			update();
		}
		if (m_overQuit)
		{
			m_overQuit = false;
			update();
		}
		if (m_game.phase() == 0 && m_overStart)
		{
			m_overStart = false;
			update();
		}
	}
}

#include "ksimon.moc"
