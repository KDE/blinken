/***************************************************************************
 *   Copyright (C) 2005-2006 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "highscoredialog.h"

#include <qpainter.h>
#include <qpixmap.h>
#include <qtabbar.h>

#include <kconfig.h>
#include <klocale.h>
#include <kglobal.h>
#include <ktabwidget.h>

#include "counter.h"
#include "libkdeedu/kedufontutils.h"
#include "settings.h"

static const int margin = 15;
static const int smallMargin = 5;
static const int namesFontSize = 25;

/* scoresWidget */

class scoresWidget : public QWidget
{
	public:
		scoresWidget(QWidget *parent, const QList< QPair<int, QString> > &scores, QSvgRenderer *renderer);
		QSize calcSize();

	protected:
		void paintEvent(QPaintEvent *);
	
	private:
		const QList< QPair<int, QString> > &m_scores;
		QSvgRenderer *m_renderer;
};

scoresWidget::scoresWidget(QWidget *parent, const QList< QPair<int, QString> > &scores, QSvgRenderer *renderer)
 : QWidget(parent), m_scores(scores), m_renderer(renderer)
{
	setAttribute(Qt::WA_StaticContents);
}

void scoresWidget::paintEvent(QPaintEvent *)
{
	int w = width();
	int h = height();
	QFont f;
	QPixmap buf(w, h);
	QPainter p(&buf);
	QRect r;
	QColor bg = palette().window().color();
 
	// bg color
	p.fillRect(0, 0, w, h, bg);
	
	p.setPen(Qt::black);
	
	if (blinkenSettings::customFont()) f = QFont("Steve");
	p.setFont(f);
	f.setPointSize(fontUtils::fontSize(p, "A", 1000, namesFontSize, fontUtils::DoNotAllowWordWrap));
	p.setFont(f);
	
	p.translate(margin, margin);
	
	QList< QPair<int, QString> >::const_iterator it;
	for (it = m_scores.begin(); it != m_scores.end(); ++it)
	{
		counter::paint(p, !(*it).second.isEmpty(), (*it).first, false, QColor(), QColor(), QColor(), m_renderer);
		p.setPen(Qt::black);
		p.drawText(counter::width(false) + 2 * smallMargin, 30, (*it).second);
		p.translate(0, counter::height() + smallMargin);
	}
	
	QPainter p2(this);
	p2.drawPixmap(0, 0, buf);
}


QSize scoresWidget::calcSize()
{
	int mw, mh, lt;
	QRect r;
	QPixmap dummyPixmap(2000, 2000);
	QPainter p(&dummyPixmap);
	QFont f;
	
	if (blinkenSettings::customFont()) f = QFont("Steve");
	p.setFont(f);
	f.setPointSize(fontUtils::fontSize(p, "A", 1000, namesFontSize, fontUtils::DoNotAllowWordWrap));
	p.setFont(f);
	for (int i = 0; i < 3; i++)
	{
		lt = 0;
		QList< QPair<int, QString> >::const_iterator it;
		for (it = m_scores.begin(); it != m_scores.end(); ++it)
		{
			r = p.boundingRect(QRect(), Qt::AlignLeft, (*it).second);
			lt = qMax(lt, r.width());
		}
	}
	
	mw = margin + counter::width(false) + 2 * smallMargin + lt + margin;
	mh = margin * 2 + counter::height() * 5 + smallMargin * 4;
	
	QSize size(mw, mh);
	setMinimumSize(size);
	resize(size);
	
	return size;
}

/* myTabWidget */

class myTabWidget : public KTabWidget
{
	public:
		myTabWidget(QWidget *parent) : KTabWidget(parent) {}
		
		QSize tabBarSizeHint() const
		{
			return tabBar() -> sizeHint();
		}
};

/* highScoreDialog */

highScoreDialog::highScoreDialog(QWidget *parent, QSvgRenderer *renderer) : KDialog(parent)
{
	setCaption(i18nc("@title:window the highest scores for each level are shown", "Highscores"));
	setButtons(Close);

	m_tw = new myTabWidget(this);
	setMainWidget(m_tw);
	
	for (int i = 1; i <= 3; i++)
	{
		KConfigGroup cfg(KGlobal::config(), QString("Level%1").arg(i));  
		for (int j = 1; j <= 5; j++)
		{
			m_scores[i-1].append(qMakePair(cfg.readEntry(QString("Score%1").arg(j),QVariant(0)).toInt(),cfg.readEntry(QString("Name%1").arg(j),QString())));
		}
	}
	
	m_tw -> addTab(new scoresWidget(0, m_scores[0], renderer), i18ncp("@title:group level high scores", "Level %1", "Level %1", 1));
	m_tw -> addTab(new scoresWidget(0, m_scores[1], renderer), i18ncp("@title:group level high scores", "Level %1", "Level %1", 2));
	m_tw -> addTab(new scoresWidget(0, m_scores[2], renderer), i18nc("@title:group all other level high scores", "Level ?"));
}

bool highScoreDialog::scoreGoodEnough(int level, int score)
{
	level--;
	QList< QPair<int, QString> >::iterator it, itEnd;
	it = m_scores[level].begin();
	itEnd = m_scores[level].end();
	while (it != itEnd && (*it).first >= score) ++it;
	
	return (it != itEnd);
}

void highScoreDialog::addScore(int level, int score, const QString &name)
{
	level--;
	QList< QPair<int, QString> >::iterator it, itEnd;
	it = m_scores[level].begin();
	itEnd = m_scores[level].end();
	while (it != itEnd && (*it).first >= score) ++it;
	
	if (it != itEnd)
	{
		m_scores[level].insert(it, qMakePair(score, name));
		m_scores[level].erase(--m_scores[level].end());
		
		KConfigGroup cfg(KGlobal::config(), QString("Level%1").arg(level + 1)); 
		int j;
		for (it = m_scores[level].begin(), j = 1; it != m_scores[level].end(); ++it, j++)
		{
			cfg.writeEntry(QString("Score%1").arg(j), (*it).first);
			cfg.writeEntry(QString("Name%1").arg(j), (*it).second);
		}
		cfg.sync();
	}
}

void highScoreDialog::showLevel(int level)
{
	QSize max, aux;
	m_tw -> setCurrentIndex(level -1);
	
	for (int i = 0; i < 3; i++)
	{
		aux = static_cast<scoresWidget*>(m_tw -> widget(i)) -> calcSize();
		max = max.expandedTo(aux);
	}
	if (max.width() < m_tw -> tabBarSizeHint().width() + 5) m_tw -> setMinimumSize(m_tw -> tabBarSizeHint().width() + 5, max.height() + m_tw -> tabBarSizeHint().height() + 5);
	
	exec();
	delete this;
}
