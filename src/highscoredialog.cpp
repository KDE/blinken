/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <tqpainter.h>
#include <tqpixmap.h>
#include <tqtabbar.h>
#include <tqtabwidget.h>

#include <kapplication.h>
#include <kconfig.h>
#include <klocale.h>

#include "highscoredialog.h"
#include "counter.h"
#include "fontutils.h"
#include "settings.h"

static const int margin = 15;
static const int smallMargin = 5;
static const int namesFontSize = 25;

/* scoresWidget */

class scoresWidget : public QWidget
{
	public:
		scoresWidget(TQWidget *parent, const TQValueList< QPair<int, TQString> > &scores);
		TQSize calcSize();

	protected:
		void paintEvent(TQPaintEvent *);
	
	private:
		const TQValueList< QPair<int, TQString> > &m_scores;
};

scoresWidget::scoresWidget(TQWidget *parent, const TQValueList< QPair<int, TQString> > &scores) : TQWidget(parent, 0, WStaticContents | WNoAutoErase), m_scores(scores)
{
}

void scoresWidget::paintEvent(TQPaintEvent *)
{
	int w = width();
	int h = height();
	TQFont f;
	TQPixmap buf(w, h);
	TQPainter p(&buf);
	TQRect r;
	TQColor bg = paletteBackgroundColor();
 
	// bg color
	p.fillRect(0, 0, w, h, bg);
	
	p.setPen(black);
	
	if (blinkenSettings::customFont()) f = TQFont("Steve");
	p.setFont(f);
	f.setPointSize(fontUtils::fontSize(p, "A", 1000, namesFontSize));
	p.setFont(f);
	
	p.translate(margin, margin);
	
	TQValueList< QPair<int, TQString> >::const_iterator it;
	for (it = m_scores.begin(); it != m_scores.end(); ++it)
	{
		counter::paint(p, !(*it).second.isEmpty(), (*it).first, false, TQColor(), TQColor(), TQColor());
		p.setPen(black);
		p.drawText(counter::width(false) + 2 * smallMargin, 30, (*it).second);
		p.translate(0, counter::height() + smallMargin);
	}
	
	bitBlt(this, 0, 0, &buf);
}

TQSize scoresWidget::calcSize()
{
	int mw, mh, lt;
	TQRect r;
	TQPainter p(this);
	TQFont f;
	
	if (blinkenSettings::customFont()) f = TQFont("Steve");
	p.setFont(f);
	f.setPointSize(fontUtils::fontSize(p, "A", 1000, namesFontSize));
	p.setFont(f);
	for (int i = 0; i < 3; i++)
	{
		lt = 0;
		TQValueList< QPair<int, TQString> >::const_iterator it;
		for (it = m_scores.begin(); it != m_scores.end(); ++it)
		{
			r = p.boundingRect(TQRect(), Qt::AlignAuto, (*it).second);
			lt = QMAX(lt, r.width());
		}
	}
	
	mw = margin + counter::width(false) + 2 * smallMargin + lt + margin;
	mh = margin * 2 + counter::height() * 5 + smallMargin * 4;
	
	TQSize size(mw, mh);
	setMinimumSize(size);
	resize(size);
	
	return size;
}

/* myTabWidget */

class myTabWidget : public QTabWidget
{
	public:
		myTabWidget(TQWidget *parent) : TQTabWidget(parent) {}
		
		TQSize tabBarSizeHint() const
		{
			return tabBar() -> tqsizeHint();
		}
};

/* highScoreDialog */

highScoreDialog::highScoreDialog(TQWidget *parent) : KDialogBase(parent, 0, true, i18n("Highscores"), KDialogBase::Close)
{
	m_tw = new myTabWidget(this);
	setMainWidget(m_tw);
	
	KConfig *cfg = kapp -> config();
	for (int i = 1; i <= 3; i++)
	{
		cfg -> setGroup(TQString("Level%1").arg(i));
		for (int j = 1; j <= 5; j++)
		{
			m_scores[i-1].append(qMakePair(cfg->readNumEntry(TQString("Score%1").arg(j)), cfg->readEntry(TQString("Name%1").arg(j))));
		}
	}
	
	m_tw -> addTab(new scoresWidget(m_tw, m_scores[0]), i18n("Level 1"));
	m_tw -> addTab(new scoresWidget(m_tw, m_scores[1]), i18n("Level 2"));
	m_tw -> addTab(new scoresWidget(m_tw, m_scores[2]), i18n("Level ?"));
}

bool highScoreDialog::scoreGoodEnough(int level, int score)
{
	level--;
	TQValueList< QPair<int, TQString> >::iterator it, itEnd;
	it = m_scores[level].begin();
	itEnd = m_scores[level].end();
	while (it != itEnd && (*it).first >= score) it++;
	
	return (it != itEnd);
}

void highScoreDialog::addScore(int level, int score, const TQString &name)
{
	level--;
	TQValueList< QPair<int, TQString> >::iterator it, itEnd;
	it = m_scores[level].begin();
	itEnd = m_scores[level].end();
	while (it != itEnd && (*it).first >= score) it++;
	
	if (it != itEnd)
	{
		m_scores[level].insert(it, qMakePair(score, name));
		m_scores[level].remove(--m_scores[level].end());
		
		KConfig *cfg = kapp -> config();
		cfg -> setGroup(TQString("Level%1").arg(level + 1));
		int j;
		for (it = m_scores[level].begin(), j = 1; it != m_scores[level].end(); ++it, j++)
		{
			cfg->writeEntry(TQString("Score%1").arg(j), (*it).first);
			cfg->writeEntry(TQString("Name%1").arg(j), (*it).second);
		}
		cfg -> sync();
	}
}

void highScoreDialog::showLevel(int level)
{
	TQSize max, aux;
	m_tw -> setCurrentPage(level -1);
	
	for (int i = 0; i < 3; i++)
	{
		aux = static_cast<scoresWidget*>(m_tw -> page(i)) -> calcSize();
		max = max.expandedTo(aux);
	}
	if (max.width() < m_tw -> tabBarSizeHint().width() + 5) m_tw -> setMinimumSize(m_tw -> tabBarSizeHint().width() + 5, max.height() + m_tw -> tabBarSizeHint().height() + 5);
	
	exec();
	delete this;
}
