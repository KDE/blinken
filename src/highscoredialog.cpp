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
#include <kconfig.h>
#include <klocale.h>

#include "highscoredialog.h"
#include "counter.h"
#include "fontutils.h"

static const int blackFrameMargin = 4;
static const int hsMargin = 5;
static const int roundBoxMargin = 13;
static const int roundBoxPen = 4;
static const int minLevelSep = 15;
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

highScoreDialog::highScoreDialog(QWidget *parent) : KDialog(parent, 0, true, WNoAutoErase), m_overClose(false)
{
	setCaption("HighScores");
	
	KConfig *cfg = kapp -> config();
	for (int i = 1; i <= 3; i++)
	{
		cfg -> setGroup(QString("Level%1").arg(i));
		for (int j = 1; j <= 5; j++)
		{
			m_scores[i-1].append(qMakePair(cfg->readNumEntry(QString("Score%1").arg(j)), cfg->readEntry(QString("Name%1").arg(j))));
		}
	}
}

bool highScoreDialog::scoreGoodEnough(int level, int score)
{
	level--;
	QValueList< QPair<int, QString> >::iterator it, itEnd;
	it = m_scores[level].begin();
	itEnd = m_scores[level].end();
	while (it != itEnd && (*it).first >= score) it++;
	
	return (it != itEnd);
}

void highScoreDialog::addScore(int level, int score, const QString &name)
{
	level--;
	QValueList< QPair<int, QString> >::iterator it, itEnd;
	it = m_scores[level].begin();
	itEnd = m_scores[level].end();
	while (it != itEnd && (*it).first >= score) it++;
	
	if (it != itEnd)
	{
		m_scores[level].insert(it, qMakePair(score, name));
		m_scores[level].remove(--m_scores[level].end());
		
		KConfig *cfg = kapp -> config();
		cfg -> setGroup(QString("Level%1").arg(level + 1));
		int j;
		for (it = m_scores[level].begin(), j = 1; it != m_scores[level].end(); ++it, j++)
		{
			cfg->writeEntry(QString("Score%1").arg(j), (*it).first);
			cfg->writeEntry(QString("Name%1").arg(j), (*it).second);
		}
		cfg -> sync();
	}
}

void highScoreDialog::showLevel(int level)
{
	m_level = level - 1;
	calcSize();
	exec();
	delete this;
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
	
	if (level1Rect.contains(e-> pos()) && !m_overLevel1)
	{
		m_overLevel1 = true;
		update();
	}
	else if (!level1Rect.contains(e-> pos()) && m_overLevel1)
	{
		m_overLevel1 = false;
		update();
	}
	
	if (level2Rect.contains(e-> pos()) && !m_overLevel2)
	{
		m_overLevel2 = true;
		update();
	}
	else if (!level2Rect.contains(e-> pos()) && m_overLevel2)
	{
		m_overLevel2 = false;
		update();
	}
	
	if (level3Rect.contains(e-> pos()) && !m_overLevel3)
	{
		m_overLevel3 = true;
		update();
	}
	else if (!level3Rect.contains(e-> pos()) && m_overLevel3)
	{
		m_overLevel3 = false;
		update();
	}
}

void highScoreDialog::mousePressEvent(QMouseEvent *)
{
	if (m_overClose) close();
	if (m_overLevel1 && m_level != 0)
	{
		m_level = 0;
		update();
	}
	if (m_overLevel2 && m_level != 1)
	{
		m_level = 1;
		update();
	}
	if (m_overLevel3 && m_level != 2)
	{
		m_level = 2;
		update();
	}
}

void highScoreDialog::paintEvent(QPaintEvent *)
{
	int w = width();
	int h = height();
	QFont f;
	QPixmap buf(w, h);
	QPainter p(&buf);
	QRect r;
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
	if (m_level == 0) f.setBold(true);
	else f.setBold(false);
	p.setFont(f);
	level1Rect = p.boundingRect(QRect(), Qt::AlignAuto, l1);
	if (m_level == 1) f.setBold(true);
	else f.setBold(false);
	p.setFont(f);
	level2Rect = p.boundingRect(QRect(), Qt::AlignAuto, l2);
	if (m_level == 2) f.setBold(true);
	else f.setBold(false);
	p.setFont(f);
	level3Rect = p.boundingRect(QRect(), Qt::AlignAuto, l3);
	textsWidth = minLevelSep + level1Rect.width() + minLevelSep + level2Rect.width() + minLevelSep + level3Rect.width() + minLevelSep;
	
	tw = (w - 2 * roundBoxMargin - 2 * blackFrameMargin) / 3;
	
	p.setPen(black);
	th = QMAX(level1Rect.height(), level2Rect.height());
	th = QMAX(th, level3Rect.height());
	
	level1Rect = QRect(0, 0, level1Rect.width() + 10, level1Rect.height() + 2);
	level1Rect.moveBy(blackFrameMargin + roundBoxMargin + tw / 2 - level1Rect.width() / 2, r.bottom() + roundBoxMargin + th);
	if (m_level == 0) f.setBold(true);
	else f.setBold(false);
	p.setFont(f);
	p.drawText(level1Rect, Qt::AlignCenter, l1);
	if (m_overLevel1) p.drawRect(level1Rect);
	
	level2Rect = QRect(0, 0, level2Rect.width() + 10, level2Rect.height() + 2);
	level2Rect.moveBy(blackFrameMargin + roundBoxMargin + tw + tw / 2 - level2Rect.width() / 2, r.bottom() + roundBoxMargin + th);
	if (m_level == 1) f.setBold(true);
	else f.setBold(false);
	p.setFont(f);
	p.drawText(level2Rect, Qt::AlignCenter, l2);
	if (m_overLevel2) p.drawRect(level2Rect);
	
	level3Rect = QRect(0, 0, level3Rect.width() + 10, level3Rect.height() + 2);
	level3Rect.moveBy(blackFrameMargin + roundBoxMargin + tw + tw + tw / 2 - level3Rect.width() / 2, r.bottom() + roundBoxMargin + th);
	if (m_level == 2) f.setBold(true);
	else f.setBold(false);
	p.setFont(f);
	p.drawText(level3Rect, Qt::AlignCenter, l3);
	if (m_overLevel3) p.drawRect(level3Rect);
	
	// black frame
	p.setPen(QPen(black, 2 * blackFrameMargin));
	p.drawRect(0, 0, w, h);
	
	f = QFont("Steve");
	f.setPointSize(fontUtils::fontSize(p, "A", 1000, namesFontSize));
	p.setFont(f);
	
	p.translate(blackFrameMargin + roundBoxMargin + roundBoxPen + 2 * counterMargin, level1Rect.bottom() + counterMargin);
	
	QValueList< QPair<int, QString> >::const_iterator it;
	for (it = m_scores[m_level].begin(); it != m_scores[m_level].end(); ++it)
	{
		counter::paint(p, !(*it).second.isEmpty(), (*it).first, false, QColor(), QColor(), QColor());
		p.setPen(black);
		p.drawText(textRight, textDown, (*it).second);
		p.translate(0, moveDown);
	}
	
	p.translate(0, -(moveDown * 5));
	p.translate(-(blackFrameMargin + roundBoxMargin + roundBoxPen + 2 * counterMargin), -(level1Rect.bottom() + counterMargin));
	
	closeRect = QRect(0, 0, closeRect.width() + 10, closeRect.height() + 2);
	closeRect.moveBy(w / 2 - closeRect.width() / 2, (int)(h - 1.75 * closeRect.height()) + 2);
	
	f = font();
	f.setBold(true);
	p.setFont(f);
	p.setPen(QPen(black, 2));
	if (m_overClose) p.setBrush(QColor(200, 200, 200));
	else p.setBrush(bg);
	p.drawRect(closeRect);
	p.drawText(closeRect, Qt::AlignCenter, cl);
	
	bitBlt(this, 0, 0, &buf);
}

void highScoreDialog::calcSize()
{
	int mw, mh, lt, th;
	QRect r, r1, r2, r3, r4, r5;
	QPainter p(this);
	QFont f;
	
	f = p.font();
	f.setBold(true);
	p.setFont(f);
	r1 = p.boundingRect(QRect(), Qt::AlignAuto, hs);
	r1 = p.boundingRect(QRect(), Qt::AlignAuto, l1);
	r2 = p.boundingRect(QRect(), Qt::AlignAuto, l2);
	r3 = p.boundingRect(QRect(), Qt::AlignAuto, l3);
	r5 = p.boundingRect(QRect(), Qt::AlignAuto, cl);
	
	mw = blackFrameMargin + roundBoxMargin + roundBoxPen + minLevelSep + r1.width() + minLevelSep + r2.width() + minLevelSep + r3.width() + minLevelSep + roundBoxPen + roundBoxMargin + blackFrameMargin;
	
	f = QFont("Steve");
	f.setPointSize(fontUtils::fontSize(p, "A", 1000, namesFontSize));
	p.setFont(f);
	for (int i = 0; i < 3; i++)
	{
		lt = 0;
		QValueList< QPair<int, QString> >::const_iterator it;
		for (it = m_scores[i].begin(); it != m_scores[i].end(); ++it)
		{
			r4 = p.boundingRect(QRect(), Qt::AlignAuto, (*it).second);
			lt = QMAX(lt, r4.width());
		}
		mw = QMAX(mw, (blackFrameMargin + roundBoxMargin + roundBoxPen + 2 * counterMargin + textRight + lt + roundBoxPen + roundBoxMargin + blackFrameMargin));
	}
	
	th = QMAX(r1.height(), r2.height());
	th = QMAX(th, r3.height());
	
	mh = blackFrameMargin + hsMargin + r.height() + hsMargin + roundBoxMargin + roundBoxPen + th + counterMargin + moveDown * 5 + roundBoxPen + roundBoxMargin + hsMargin + r5.height() + hsMargin + blackFrameMargin;
	
	setMinimumSize(mw, mh);
	resize(mw, mh);
	setMouseTracking(true);
}
