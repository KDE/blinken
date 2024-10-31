/*
    SPDX-FileCopyrightText: 2005-2006 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "highscoredialog.h"

#include <QDialogButtonBox>
#include <QPainter>
#include <QPixmap>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>

#include <KConfig>
#include <KFontUtils>
#include <KLocalizedString>
#include <KSharedConfig>

#include "counter.h"
#include "settings.h"
#include "highScoreManager.h"

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
		void paintEvent(QPaintEvent *) override;
	
	private:
		const QList< QPair<int, QString> > m_scores;
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
	
	if (BlinkenSettings::customFont()) f = QFont(QStringLiteral("Steve"));
	p.setFont(f);
	f.setPointSize(KFontUtils::adaptFontSize(p, QStringLiteral("A"), 1000, namesFontSize, 28, 1, KFontUtils::DoNotAllowWordWrap));
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
	
	if (BlinkenSettings::customFont()) f = QFont(QStringLiteral("Steve"));
	p.setFont(f);
	f.setPointSize(KFontUtils::adaptFontSize(p, QStringLiteral("A"), 1000, namesFontSize, 28, 1, KFontUtils::DoNotAllowWordWrap));
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

class myTabWidget : public QTabWidget
{
	public:
		myTabWidget(QWidget *parent) : QTabWidget(parent) {}
		
		QSize tabBarSizeHint() const
		{
			return tabBar() -> sizeHint();
		}
};

/* highScoreDialog */

highScoreDialog::highScoreDialog(QWidget *parent, QSvgRenderer *renderer) : QDialog(parent)
{
	setWindowTitle(i18nc("@title:window the highest scores for each level are shown", "Highscores"));

	setLayout(new QVBoxLayout(this));
	m_tw = new myTabWidget(this);
	layout()->addWidget(m_tw);
	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &highScoreDialog::close);
	layout()->addWidget(buttonBox);

	HighScoreManager hsm;
	
	m_tw -> addTab(new scoresWidget(nullptr, hsm.scores(0), renderer), i18nc("@title:group High scores Level 1 tab title", "Level 1"));
	m_tw -> addTab(new scoresWidget(nullptr, hsm.scores(1), renderer), i18nc("@title:group High scores Level 2 tab title", "Level 2"));
	m_tw -> addTab(new scoresWidget(nullptr, hsm.scores(2), renderer), i18nc("@title:group High scores Level ? tab tible", "Level ?"));
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
}

#include "moc_highscoredialog.cpp"
