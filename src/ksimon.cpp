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

#include "ksimon.h"

KSimon::KSimon() : QWidget(0, 0, WStaticContents | WNoAutoErase), m_overHelp(false), m_overQuit(false), m_overStart(false), m_gamePhase(0)
{
	m_back = new QPixmap(locate("appdata", "images/ksimon.png"));
	setMouseTracking(true);
	setFixedSize(644, 525);
	show();
}

KSimon::~KSimon()
{
	delete m_back;
}

void KSimon::paintEvent(QPaintEvent *)
{
	QPixmap buf(width(), height());
	QPainter p(&buf);
	
	p.drawPixmap(0, 0, *m_back);
	
	putFont(p, i18n("Help"), i18n("Quit"), 100, 50);
	drawHelp(p);
	drawQuit(p);
	switch (m_gamePhase)
	{
		case 0:
			drawStart(p);
		break;
		
		case 1:
			drawLevel(p);
		break;
	}
	drawText(p);
	
	bitBlt(this, 0, 0, &buf);
}

void KSimon::mouseMoveEvent(QMouseEvent *e)
{
	if (m_helpRect.contains(e -> pos()))
	{
		if (!m_overHelp)
		{
			m_overHelp = true;
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
	else if (m_gamePhase == 0 && m_startRect.contains(e -> pos()))
	{
		if (!m_overStart)
		{
			m_overStart = true;
			update();
		}
	}
	else
	{
		if (m_overHelp)
		{
			m_overHelp = false;
			update();
		}
		if (m_overQuit)
		{
			m_overQuit = false;
			update();
		}
		if (m_gamePhase == 0 && m_overStart)
		{
			m_overStart = false;
			update();
		}
	}
}

void KSimon::mousePressEvent(QMouseEvent *e)
{
	if (m_overHelp) KMessageBox::information(this, i18n("This is a code mockup for KSimon project"), i18n("Help"));
	else if (m_overQuit) kapp->quit();
	else if (m_gamePhase == 0 && m_overStart)
	{
		m_overStart = false;
		m_gamePhase = 1;
		update();
	}
	else
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
				if (x > 6 && y > 6) KMessageBox::information(this, i18n("Green"), i18n("Green"));
				else if (x < -6 && y > 6) KMessageBox::information(this, i18n("Blue"), i18n("Blue"));
				else if (x < -6 && y < -6) KMessageBox::information(this, i18n("Yellow"), i18n("Yellow"));
				else if (x > 6 && y < -6) KMessageBox::information(this, i18n("Red"), i18n("Red"));
			}
		}
	}
}

void KSimon::drawHelp(QPainter &p)
{
	if (!m_overHelp) p.setPen(Qt::red);
	else p.setPen(Qt::blue);
	
	m_helpRect = p.boundingRect(QRect(), Qt::AlignAuto, i18n("Help"));
	m_helpRect.moveBy(10, 10);
	p.fillRect(m_helpRect, Qt::white);
	p.drawText(m_helpRect, Qt::AlignAuto, i18n("Help"));
}

void KSimon::drawQuit(QPainter &p)
{
	if (!m_overQuit) p.setPen(Qt::red);
	else p.setPen(Qt::blue);
	
	m_quitRect = p.boundingRect(QRect(), Qt::AlignAuto, i18n("Quit"));
	m_quitRect.moveBy(width() - 10 - m_quitRect.width(), 10);
	p.fillRect(m_quitRect, Qt::white);
	p.drawText(m_quitRect, Qt::AlignAuto, i18n("Quit"));
}

void KSimon::drawStart(QPainter &p)
{
	QFont f = p.font();
	f.setPointSize(25);
	p.setFont(f);
	
	if (!m_overStart) p.setPen(Qt::red);
	else p.setPen(Qt::blue);
	
	m_startRect = p.boundingRect(QRect(), Qt::AlignAuto, i18n("Start"));
	m_startRect.moveBy(322 - m_startRect.width() / 2, 316 - m_startRect.height() / 2);
	p.fillRect(m_startRect, Qt::white);
	p.drawText(m_startRect, Qt::AlignAuto, i18n("Start"));
}

void KSimon::drawText(QPainter &p)
{
	QFont f = p.font();
	f.setPointSize(20);
	p.setFont(f);
	
	p.translate(25, 505);
	p.rotate(-3.29);
	p.setPen(black);
	switch (m_gamePhase)
	{
		case 0:
			p.drawText(0, 0, i18n("Press Start to begin!"));
		break;
		
		case 1:
			p.drawText(0, 0, i18n("Set the Difficulty Level..."));
		break;
	}
}

void KSimon::drawLevel(QPainter &p)
{
	QFont f = p.font();
	f.setPointSize(25);
	p.setFont(f);
	p.setPen(Qt::red);
	
	QRect aux;
	
	aux = p.boundingRect(QRect(), Qt::AlignAuto, i18n("Level"));
	aux.moveBy(322 - aux.width() / 2, 275 - aux.height() / 2);
	p.fillRect(aux, Qt::white);
	p.drawText(aux, Qt::AlignAuto, i18n("Level"));
}

void KSimon::putFont(QPainter &p, const QString &s1, const QString &s2, int w, int h)
{
	int fontSize;
	QRect aux1, aux2;
	QFont f = p.font();
	f.setPointSize(28);
	p.setFont(f);
	
	aux1 = p.boundingRect(QRect(), Qt::AlignAuto, s1);
	aux2 = p.boundingRect(QRect(), Qt::AlignAuto, s2);
	
	fontSize = QMIN(w * 28 / aux1.width(), w * 28 / aux2.width());
	fontSize = QMIN(fontSize, h * 28 / aux1.height());
	fontSize = QMIN(fontSize, h * 28 / aux2.height());
	
	f.setPointSize(fontSize);
	p.setFont(f);
}
