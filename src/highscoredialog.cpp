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

#include <klocale.h>

#include "highscoredialog.h"
#include "counter.h"
#include "fontutils.h"

static const int blackFrameMargin = 4;
static const int hsMargin = 5;
static const int roundBoxMargin = 13;
static const int roundBoxPen = 4;
static const int minLevelSep = 5;
static const int counterMargin = 25;
static const int textDown = 20;
static const int textRight = 37;
static const int moveDown = 58;
static const int namesFontSize = 25;

static const QString hs = i18n("HighScores");
static const QString l1 = i18n("Level 1");
static const QString l2 = i18n("Level 2");
static const QString l3 = i18n("Level ?");
static const QString cl = i18n("Close");

highScoreDialog::highScoreDialog(QWidget *parent, QValueList< QPair<int, QString> > scores) : KDialog(parent, 0, true, WNoAutoErase), m_overClose(false), m_scores(scores)
{
	setCaption("HighScores");
	
	int mw, mh, lt, th;
	QRect r, r1, r2, r3, r4, r5;
	QPainter p(this);
	QFont f;
	
	r1 = p.boundingRect(QRect(), Qt::AlignAuto, hs);
	r1 = p.boundingRect(QRect(), Qt::AlignAuto, l1);
	r2 = p.boundingRect(QRect(), Qt::AlignAuto, l2);
	r3 = p.boundingRect(QRect(), Qt::AlignAuto, l3);
	r5 = p.boundingRect(QRect(), Qt::AlignAuto, cl);
	
	mw = blackFrameMargin + roundBoxMargin + roundBoxPen + minLevelSep + r1.width() + minLevelSep + r2.width() + minLevelSep + r3.width() + minLevelSep + roundBoxPen + roundBoxMargin + blackFrameMargin;
	
	f = QFont("Steve");
	f.setPointSize(fontUtils::fontSize(p, "A", 1000, namesFontSize));
	p.setFont(f);
	lt = 0;
	QValueList< QPair<int, QString> >::const_iterator it;
	for (it = m_scores.begin(); it != m_scores.end(); ++it)
	{
		r4 = p.boundingRect(QRect(), Qt::AlignAuto, (*it).second);
		lt = QMAX(lt, r4.width());
	}
	
	mw = QMAX(mw, (blackFrameMargin + roundBoxMargin + roundBoxPen + 2 * counterMargin + textRight + lt + roundBoxPen + roundBoxMargin + blackFrameMargin));
	
	th = QMAX(r1.height(), r2.height());
	th = QMAX(th, r3.height());
	
	
	
	mh = blackFrameMargin + hsMargin + r.height() + hsMargin + roundBoxMargin + roundBoxPen + th + counterMargin + moveDown * m_scores.count() + roundBoxPen + roundBoxMargin + hsMargin + r5.height() + hsMargin + blackFrameMargin;
	
	setMinimumSize(mw, mh);
	resize(mw, mh);
	setMouseTracking(true);
}

void highScoreDialog::mouseMoveEvent(QMouseEvent *e)
{
	if (closeRect.contains(e-> pos()) && !m_overClose)
	{
		m_overClose = true;
		update();
	}
	else if (!closeRect.contains(e-> pos()) && m_overClose)
	{
		m_overClose = false;
		update();
	}
}

void highScoreDialog::mousePressEvent(QMouseEvent *e)
{
	if (m_overClose) close();
}

void highScoreDialog::paintEvent(QPaintEvent *)
{
	int w = width();
	int h = height();
	QFont f;
	QPixmap buf(w, h);
	QPainter p(&buf);
	QRect r, r1, r2, r3;
	QColor bg = QColor(238, 238, 238);
 
	// bg color
	p.fillRect(0, 0, w, h, bg);
	
	// highscore title and it's bg color
	f = p.font();
	f.setBold(true);
	p.setFont(f);
	r = p.boundingRect(QRect(), Qt::AlignAuto, hs);
	r.moveBy(w / 2 - r.width() / 2, hsMargin + blackFrameMargin);
	p.fillRect(0, 0, w, r.bottom() + hsMargin, QColor(122, 122, 122));
	p.drawText(r, Qt::AlignCenter, hs);
	
	// calc Close button height
	closeRect = p.boundingRect(QRect(), Qt::AlignAuto, cl);
	f.setBold(false);
	p.setFont(f);
	
	// round box around scores
	p.setPen(QPen(QColor(176, 176, 176), roundBoxPen));
	p.drawRoundRect(roundBoxMargin, 
	                r.bottom() + roundBoxMargin + hsMargin, 
	                w - 2 * roundBoxMargin, 
	                h - r.bottom() - hsMargin - 2 * roundBoxMargin - blackFrameMargin - closeRect.height(), 10, 10);
	
	int textsWidth, tw, th;
	r1 = p.boundingRect(QRect(), Qt::AlignAuto, l1);
	r2 = p.boundingRect(QRect(), Qt::AlignAuto, l2);
	r3 = p.boundingRect(QRect(), Qt::AlignAuto, l3);
	textsWidth = minLevelSep + r1.width() + minLevelSep + r2.width() + minLevelSep + r3.width() + minLevelSep;
	
	tw = (w - 2 * roundBoxMargin - 2 * blackFrameMargin) / 3;
	
	p.setPen(black);
	th = QMAX(r1.height(), r2.height());
	th = QMAX(th, r3.height());
	r1.moveBy(blackFrameMargin + roundBoxMargin + tw / 2 - r1.width() / 2, r.bottom() + roundBoxMargin + th);
	p.drawText(r1, Qt::AlignCenter, l1);
	r2.moveBy(blackFrameMargin + roundBoxMargin + tw + tw / 2 - r2.width() / 2, r.bottom() + roundBoxMargin + th);
	p.drawText(r2, Qt::AlignCenter, l2);
	r3.moveBy(blackFrameMargin + roundBoxMargin + tw + tw + tw / 2 - r3.width() / 2, r.bottom() + roundBoxMargin + th);
	p.drawText(r3, Qt::AlignCenter, l3);
	
	// black frame
	p.setPen(QPen(black, 2 * blackFrameMargin));
	p.drawRect(0, 0, w, h);
	
	f = QFont("Steve");
	f.setPointSize(fontUtils::fontSize(p, "A", 1000, namesFontSize));
	p.setFont(f);
	
	p.translate(blackFrameMargin + roundBoxMargin + roundBoxPen + 2 * counterMargin, r1.bottom() + counterMargin);
	
	QValueList< QPair<int, QString> >::const_iterator it;
	for (it = m_scores.begin(); it != m_scores.end(); ++it)
	{
		counter::paint(p, true, (*it).first, false, QColor(), QColor(), QColor());
		p.setPen(black);
		p.drawText(textRight, textDown, (*it).second);
		p.translate(0, moveDown);
	}
	
	p.translate(0, -(moveDown * (int)m_scores.count()));
	p.translate(-(blackFrameMargin + roundBoxMargin + roundBoxPen + 2 * counterMargin), -(r1.bottom() + counterMargin));
	
	closeRect = QRect(0, 0, closeRect.width() + 10, closeRect.height() + 2);
	closeRect.moveBy(w / 2 - closeRect.width() / 2, (int)(h - 1.75 * closeRect.height()) + 2);
	
	m_firstPaint = false;
	
	f = font();
	f.setBold(true);
	p.setFont(f);
	p.setPen(QPen(black, 2));
	if (m_overClose) p.setBrush(QColor(200, 200, 200));
	else p.setBrush(bg);
	p.drawRect(closeRect.left(), closeRect.top(), closeRect.width(), closeRect.height());
	p.drawText(closeRect, Qt::AlignCenter, cl);
	
	bitBlt(this, 0, 0, &buf);
}
