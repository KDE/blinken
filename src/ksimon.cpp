/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qpainter.h>
#include <qpixmap.h>

#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>

#include "artsplayer.h"
#include "ksimon.h"

KSimon::KSimon() : QWidget(0, 0, WStaticContents | WNoAutoErase), m_number1(0), m_number2(0), m_number3(0), m_overMenu(false), m_overQuit(false), m_overStart(false)
{
	m_back = new QPixmap(locate("appdata", "images/ksimon.png"));
	setMouseTracking(true);
	setFixedSize(644, 525);
	show();
	
	m_artsPlayer = new artsPlayer;
}

KSimon::~KSimon()
{
	delete m_back;
	delete m_artsPlayer;
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
	drawStatusText(p);
	
	bitBlt(this, 0, 0, &buf);
}

void KSimon::mouseMoveEvent(QMouseEvent *e)
{
	if (m_menuRect.contains(e -> pos()))
	{
		if (!m_overMenu)
		{
			m_overMenu = true;
			update();
		}
	}
	else if (m_quitRect.contains(e -> pos()))
	{
		if (!m_overQuit)
		{
			m_overQuit = true;
			update();
		}
	}
	else if (m_game.phase() == 0 && m_startRect.contains(e -> pos()))
	{
		if (!m_overStart)
		{
			m_overStart = true;
			update();
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

void KSimon::mousePressEvent(QMouseEvent *e)
{
	if (m_overMenu) KMessageBox::information(this, i18n("This is a code mockup for KSimon project"), i18n("Help"));
	else if (m_overQuit) kapp->quit();
	else if (m_game.phase() == 0 && m_overStart)
	{
		m_overStart = false;
		m_game.setPhase(simonGame::choosingLevel);
		update();
	}
	else if (m_game.phase() == simonGame::choosingLevel)
	{
		if (m_number1Rect.contains(e -> pos()))
		{
			m_game.setPhase(simonGame::learningTheSequence);
			m_game.start(1);
			update();
		}
		else if (m_number2Rect.contains(e -> pos()))
		{
			m_game.setPhase(simonGame::learningTheSequence);
			m_game.start(2);
			update();
		}
		else if (m_number3Rect.contains(e -> pos()))
		{
			m_game.setPhase(simonGame::learningTheSequence);
			m_game.start(3);
			update();
		}
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
					m_artsPlayer -> play(locate("appdata","sounds/1.wav"));
				}
				else if (x < -6 && y > 6)
				{
					m_artsPlayer -> play(locate("appdata","sounds/2.wav"));
				}
				else if (x < -6 && y < -6)
				{
					m_artsPlayer -> play(locate("appdata","sounds/3.wav"));
				}
				else if (x > 6 && y < -6)
				{
					m_artsPlayer -> play(locate("appdata","sounds/4.wav"));
				}
			}
		}
	}
}

void KSimon::drawMenuQuit(QPainter &p)
{
	int fs;
	QString menu, quit;
	
	menu = i18n("Menu");
	quit = i18n("Quit");
	
	fs = QMIN(fontSize(p, menu, 100, 50), fontSize(p, quit, 100, 50));
	QFont f = p.font();
	f.setPointSize(fs);
	p.setFont(f);
	
	if (!m_overMenu) p.setPen(Qt::red);
	else p.setPen(Qt::blue);
	m_menuRect = p.boundingRect(QRect(), Qt::AlignAuto, menu);
	m_menuRect.moveBy(10, 10);
	p.fillRect(m_menuRect, Qt::white);
	p.drawText(m_menuRect, Qt::AlignAuto, menu);
	
	if (!m_overQuit) p.setPen(Qt::red);
	else p.setPen(Qt::blue);
	m_quitRect = p.boundingRect(QRect(), Qt::AlignAuto, quit);
	m_quitRect.moveBy(width() - 10 - m_quitRect.width(), 10);
	p.fillRect(m_quitRect, Qt::white);
	p.drawText(m_quitRect, Qt::AlignAuto, quit);
}

void KSimon::drawStart(QPainter &p)
{
	QString start = i18n("Start");

	QFont f = p.font();
	f.setPointSize(fontSize(p, start, 190, 30));
	p.setFont(f);
	p.setPen(Qt::red);
	
	if (!m_overStart) p.setPen(Qt::red);
	else p.setPen(Qt::blue);
	
	m_startRect = p.boundingRect(QRect(), Qt::AlignAuto, start);
	m_startRect.moveBy(322 - m_startRect.width() / 2, 316 - m_startRect.height() / 2);
	p.fillRect(m_startRect, Qt::white);
	p.drawText(m_startRect, Qt::AlignAuto, start);
}

void KSimon::drawStatusText(QPainter &p)
{
	QFont f = p.font();
	f.setPointSize(20);
	p.setFont(f);
	
	p.translate(25, 505);
	p.rotate(-3.29);
	p.setPen(black);
	switch (m_game.phase())
	{
		case simonGame::starting:
			p.drawText(0, 0, i18n("Press Start to begin!"));
		break;
		
		case simonGame::choosingLevel:
			p.drawText(0, 0, i18n("Set the Difficulty Level..."));
		break;
		
		case simonGame::learningTheSequence:
			p.drawText(0, 0, i18n("Remember this sequence..."));
		break;
		
		case simonGame::typingTheSequence:
			p.drawText(0, 0, i18n("Type the sequence!"));
		break;
	}
}

void KSimon::drawLevel(QPainter &p)
{
	QString level = i18n("Level");
	
	QFont f = p.font();
	f.setPointSize(fontSize(p, level, 190, 30));
	p.setFont(f);
	p.setPen(Qt::red);
	
	QRect aux;
	aux = p.boundingRect(QRect(), Qt::AlignAuto, level);
	aux.moveBy(322 - aux.width() / 2, 285 - aux.height() / 2);
	p.fillRect(aux, Qt::white);
	p.drawText(aux, Qt::AlignAuto, level);
	
	if (!m_number1)
	{
		m_number1 = new QPixmap(locate("appdata", "images/1.png"));
		m_number2 = new QPixmap(locate("appdata", "images/2.png"));
		m_number3 = new QPixmap(locate("appdata", "images/3.png"));
	}
	
	int x2, x1, x3;
	x2 = 319 - m_number2 -> width() / 2;
	x1 = x2 - (int)(m_number1 -> width() * 1.5);
	x3 = x2 + (int)(m_number3 -> width() * 1.5);
	p.drawPixmap(x1, 305, *m_number1);
	p.drawPixmap(x2, 305, *m_number2);
	p.drawPixmap(x3, 305, *m_number3);
	
	m_number1Rect = QRect(x1, 305, m_number1 -> width(), m_number1 -> height());
	m_number2Rect = QRect(x2, 305, m_number2 -> width(), m_number2 -> height());
	m_number3Rect = QRect(x3, 305, m_number3 -> width(), m_number3 -> height());
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
