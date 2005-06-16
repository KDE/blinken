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

#include <kaction.h>
#include <kapplication.h>
#include <kconfig.h>
#include <khelpmenu.h>
#include <kinputdialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kstandarddirs.h>

#include "ksimon.h"
#include "button.h"
#include "counter.h"
#include "fontutils.h"
#include "number.h"
#include "highscoredialog.h"

KSimon::KSimon() : QWidget(0, 0, WStaticContents | WNoAutoErase), m_overHighscore(false), m_overQuit(false), m_overCentralText(false), m_overMenu(false), m_overAboutKDE(false), m_overAboutKSimon(false), m_overManual(false), m_showKeys(false), m_updateButtonHighlighting(false), m_highlighted(simonGame::none)
{
	m_back = new QPixmap(locate("appdata", "images/ksimon.png"));
	
	m_buttons[0] = new button(simonGame::blue);
	m_buttons[1] = new button(simonGame::yellow);
	m_buttons[2] = new button(simonGame::red);
	m_buttons[3] = new button(simonGame::green);
	
	m_highscore = new QPixmap(locate("appdata", "images/highscore.png"));
	m_highscoreHover = new QPixmap(locate("appdata", "images/highscore_hover.png"));
	m_quit = new QPixmap(locate("appdata", "images/quit.png"));
	m_quitHover = new QPixmap(locate("appdata", "images/quit_hover.png"));
	m_menu = new QPixmap(locate("appdata", "images/menu.png"));
	m_menuHover = new QPixmap(locate("appdata", "images/menu_hover.png"));
	m_mark = new QPixmap(locate("appdata", "images/mark.png"));
	m_highscoreRect = QRect(10, 10, 72, 72);
	m_quitRect = QRect(560, 10, 72, 73);
	m_menuRect = QRect(560, 443, 72, 72);
	m_aboutKDERect = QRect(452, 461, 54, 54);
	m_aboutKSimonRect = QRect(506, 461, 54, 54);
	m_manualRect = QRect(578, 389, 54, 54);
	
	m_fillColor = QColor(40, 40, 40);
	m_fontColor = QColor(126, 126, 126);
	m_fontHighlightColor  = QColor(230, 230, 230);
	m_countDownColor = QColor(55, 55, 55);
	
	setMouseTracking(true);
	setFixedSize(644, 525);
	show();
	
	m_unhighlighter = new QTimer(this);
	connect(m_unhighlighter, SIGNAL(timeout()), this, SLOT(unhighlight()));
	
	connect(&m_game, SIGNAL(gameEnded()), this, SLOT(checkHS()));
	connect(&m_game, SIGNAL(phaseChanged()), this, SLOT(update()));
	connect(&m_game, SIGNAL(highlight(simonGame::color, bool)), this, SLOT(highlight(simonGame::color, bool)));
	
	m_helpMenu = new KHelpMenu(this, kapp->aboutData());
	
	for (int i = 0; i < 3; i++) m_overLevels[i] = false;
}

KSimon::~KSimon()
{
	delete m_back;
	for (int i = 0; i < 4; i++) delete m_buttons[i];
	delete m_highscore;
	delete m_highscoreHover;
	delete m_quit;
	delete m_quitHover;
	delete m_menu;
	delete m_menuHover;
	delete m_mark;
	delete m_helpMenu;
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
			drawText(p, i18n("Start"), QPoint(318, 316), true, 10, 5, &m_centralTextRect, m_overCentralText, false);
		break;
		
		case simonGame::choosingLevel:
			drawLevel(p);
		break;
		
		case simonGame::waiting3:
		case simonGame::waiting2:
		case simonGame::waiting1:
		case simonGame::learningTheSequence:
		case simonGame::typingTheSequence:
			drawText(p, i18n("Restart"), QPoint(318, 316), true, 10, 5, &m_centralTextRect, m_overCentralText, false);
		break;
	}
	
	if (m_showKeys)
	{
		drawText(p, m_buttons[0]->shortcut(), QPoint(115, 285), true, 20, 5, 0, m_buttons[0]->selected(), false);
		drawText(p, m_buttons[1]->shortcut(), QPoint(115, 155), true, 20, 5, 0, m_buttons[1]->selected(), false);
		drawText(p, m_buttons[2]->shortcut(), QPoint(520, 155), true, 20, 5, 0, m_buttons[2]->selected(), false);
		drawText(p, m_buttons[3]->shortcut(), QPoint(520, 285), true, 20, 5, 0, m_buttons[3]->selected(), false);
	}
	
	drawScoreAndCounter(p);
	
	if (m_highlighted & simonGame::blue) p.drawPixmap(14, 225, *m_buttons[0] -> pixmap());
	if (m_highlighted & simonGame::yellow) p.drawPixmap(14, 16, *m_buttons[1] -> pixmap());
	if (m_highlighted & simonGame::red) p.drawPixmap(322, 16, *m_buttons[2] -> pixmap());
	if (m_highlighted & simonGame::green) p.drawPixmap(322, 225, *m_buttons[3] -> pixmap());
	
	drawStatusText(p);
	
	bitBlt(this, 0, 0, &buf);
	
	if (m_updateButtonHighlighting) updateButtonHighlighting(mapFromGlobal(QCursor::pos()));
}

void KSimon::keyPressEvent(QKeyEvent *e)
{
	if (e -> isAutoRepeat()) return;
	
	if (m_showKeys)
	{
		int i = 0;
		while (i < 4 && !m_buttons[i] -> selected()) i++;
	
		if (i < 4)
		{
			KShortcut ks(e -> key());
			if (!ks.toString().isEmpty())
			{
				bool different = true;
				int j = 0;
				while (different && j < 4)
				{
					different = (ks.toString() != m_buttons[j] -> shortcut());
					j++;
				}
			
				if (different)
				{
					m_buttons[i] -> setShortcut(ks);
					update();
				}
			}
		}
	}
	else
	{
		if (e -> key() == m_buttons[0] -> key()) pressedBlue();
		else if (e -> key() == m_buttons[1] -> key()) pressedYellow();
		else if (e -> key() == m_buttons[2] -> key()) pressedRed();
		else if (e -> key() == m_buttons[3] -> key()) pressedGreen();
	}
}

void KSimon::keyReleaseEvent(QKeyEvent *e)
{
	if (e -> isAutoRepeat()) return;
	
	if (e -> state() == Qt::ControlButton && e -> stateAfter() != Qt::ControlButton)
	{
		m_showKeys = !m_showKeys;
		for (int i = 0; i < 4; i++) m_buttons[i] -> setSelected(false);
		update();
	}
}

void KSimon::mouseMoveEvent(QMouseEvent *e)
{
	updateButtonHighlighting(e->pos());
}

void KSimon::mousePressEvent(QMouseEvent *e)
{
	if (m_overHighscore)
	{
		highScoreDialog *hsd = new highScoreDialog(this);
		hsd->showLevel(1);
		m_updateButtonHighlighting = true;
		update();
	}
	else if (m_overQuit) kapp->quit();
	else if (m_overAboutKSimon) m_helpMenu -> aboutApplication();
	else if (m_overAboutKDE) m_helpMenu -> aboutKDE();
	else if (m_overManual) m_helpMenu -> appHelpActivated();
	else if (m_game.phase() != simonGame::choosingLevel && m_overCentralText)
	{
		highlight(simonGame::none, true);
		m_overCentralText = false;
		for(int i = 0; i < 3; i++) m_overLevels[i] = false;
		m_game.setPhase(simonGame::choosingLevel);
		m_updateButtonHighlighting = true;
	}
	else if (m_game.phase() == simonGame::choosingLevel)
	{
		int level = 0;
		if (m_levelsRect[1].contains(e -> pos())) level = 1;
		else if (m_levelsRect[0].contains(e -> pos())) level = 2;
		else if (m_levelsRect[2].contains(e -> pos())) level = 3;
		if (level) 
		{
			for(int i = 0; i < 3; i++) m_overLevels[i] = false;
			m_game.start(level);
		}
	}
	
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
				if (m_showKeys) selectButton(3);
				else pressedGreen();
			}
			else if (x < -6 && y > 6)
			{
				if (m_showKeys) selectButton(0);
				else pressedBlue();
			}
			else if (x < -6 && y < -6)
			{
				if (m_showKeys) selectButton(1);
				else pressedYellow();
			}
			else if (x > 6 && y < -6)
			{
				if (m_showKeys) selectButton(2);
				else pressedRed();
			}
		}
	}
}

void KSimon::checkHS()
{
	highScoreDialog *hsd = new highScoreDialog(this);
	if (hsd->scoreGoodEnough(m_game.level(), m_game.score()))
	{
		bool ok;
		QString name = KInputDialog::getText(i18n("Enter your name"), i18n("Name:"), m_lastName, &ok);
		if (!name.isNull() && ok)
		{
			m_lastName = name;
			hsd->addScore(m_game.level(), m_game.score(), name);
		}
	}
	hsd->showLevel(m_game.level());
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

void KSimon::pressedYellow()
{
	if (m_game.canType())
	{
		highlight(simonGame::yellow, true);
		m_game.clicked(simonGame::yellow);
	}
}

void KSimon::pressedRed()
{
	if (m_game.canType())
	{
		highlight(simonGame::red, true);
		m_game.clicked(simonGame::red);
	}
}

void KSimon::pressedGreen()
{
	if (m_game.canType())
	{
		highlight(simonGame::green, true);
		m_game.clicked(simonGame::green);
	}
}

void KSimon::pressedBlue()
{
	if (m_game.canType())
	{
		highlight(simonGame::blue, true);
		m_game.clicked(simonGame::blue);
	}
}

void KSimon::selectButton(int button)
{
	int i = 0;
	bool selected = false;
	while (i < 4 && !selected)
	{
		selected = m_buttons[i] -> selected();
		if (!selected) i++;
	}
	
	if (selected)
	{
		if (i == button)
		{
			m_buttons[button] -> setSelected(false);
			update();
		}
	}
	else
	{
		m_buttons[button] -> setSelected(true);
		update();
	}
}

void KSimon::drawMenuQuit(QPainter &p)
{
	if (!m_overHighscore) p.drawPixmap(10, 10, *m_highscore);
	else p.drawPixmap(10, 10, *m_highscoreHover);
	
	if (!m_overQuit) p.drawPixmap(560, 10, *m_quit);
	else p.drawPixmap(560, 10, *m_quitHover);
	
	if (!m_overMenu) p.drawPixmap(560, 443, *m_menu);
	else p.drawPixmap(452, 389, *m_menuHover);
	
	if (m_overAboutKDE) p.drawPixmap(462, 433, *m_mark);
	else if (m_overAboutKSimon) p.drawPixmap(516, 433, *m_mark);
	else if (m_overManual)
	{
		p.translate(550, 430);
		p.rotate(-90.0);
		p.drawPixmap(0, 0, *m_mark);
		p.rotate(90.0);
		p.translate(-550, -430);
	}
}

void KSimon::drawScoreAndCounter(QPainter &p)
{
	QColor c1, c2, c3;
	p.translate(313, 125);
	
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
	
	counter::paint(p, m_game.phase() != simonGame::starting, m_game.score(), true, c1, c2, c3);
	
	p.translate(-313, -125);
}

void KSimon::drawStatusText(QPainter &p)
{
	QFont f = QFont("Steve");
	f.setPointSize(fontUtils::fontSize(p, "A", 1000, 25));
	p.setFont(f);
	
	p.translate(25, 505);
	p.rotate(-3.29);
	p.setPen(blue);

	QString restartText = i18n("Restart the game");
	if (m_overQuit) p.drawText(0, 0, i18n("Quit KSimon"));
	else if (m_overHighscore) p.drawText(0, 0, i18n("View HighScore Table"));
	else if (m_overAboutKSimon) p.drawText(0, 0, m_helpMenu -> menu() -> text(KHelpMenu::menuAboutApp).remove("&"));
	else if (m_overAboutKDE) p.drawText(0, 0, m_helpMenu -> menu() -> text(KHelpMenu::menuAboutKDE).remove("&"));
	else if (m_overManual) p.drawText(0, 0, m_helpMenu -> menu() -> text(KHelpMenu::menuHelpContents).remove("&"));
	else if (m_overLevels[0]) p.drawText(0, 0, i18n("2nd Level"));
	else if (m_overLevels[1]) p.drawText(0, 0, i18n("1st Level"));
	else if (m_overLevels[2]) p.drawText(0, 0, i18n("Random Level"));
	else if (m_buttons[0]->selected() || m_buttons[1]->selected() || m_buttons[2]->selected() || m_buttons[3]->selected()) p.drawText(0, 0, i18n("Press the key for this button"));
	else if (m_showKeys) p.drawText(0, 0, i18n("Click any button to change its key"));
	else
	{
		switch (m_game.phase())
		{
			case simonGame::starting:
				p.drawText(0, 0, i18n("Press Start to begin!"));
			break;
			
			case simonGame::choosingLevel:
				p.drawText(0, 0, i18n("Set the Difficulty Level..."));
			break;
			
			case simonGame::waiting3:
				if (m_overCentralText) p.drawText(0, 0, restartText);
				else p.drawText(0, 0, i18n("Next sequence in 3..."));
			break;
			
			case simonGame::waiting2:
				if (m_overCentralText) p.drawText(0, 0, restartText);
				else if (m_game.level() == 1) p.drawText(0, 0, i18n("Next sequence in 3, 2..."));
				else p.drawText(0, 0, i18n("Next sequence in 2..."));
			break;
		
			case simonGame::waiting1:
				if (m_overCentralText) p.drawText(0, 0, restartText);
				else if (m_game.level() == 1) p.drawText(0, 0, i18n("Next sequence in 3, 2, 1..."));
				else p.drawText(0, 0, i18n("Next sequence in 2, 1..."));
			break;
			
			case simonGame::learningTheSequence:
				if (m_overCentralText) p.drawText(0, 0, restartText);
				else p.drawText(0, 0, i18n("Remember this sequence..."));
			break;
			
			case simonGame::typingTheSequence:
				if (m_overCentralText) p.drawText(0, 0, restartText);
				else p.drawText(0, 0, i18n("Repeat the sequence!"));
			break;
		}
	}
}

void KSimon::drawLevel(QPainter &p)
{
	QString n[3];
	n[0] = i18n("2");
	n[1] = i18n("1");
	n[2] = i18n("?");
	
	drawText(p, i18n("Level"), QPoint(322, 281), false, 0, 0, 0, false, true);
	
	QPoint cp;
	for (int i = 0; i < 3; i++)
	{
		if (i == 0) cp = QPoint(319, 315);
		else if (i == 1) cp = QPoint(m_levelsRect[0].left() - m_levelsRect[0].width(), 315);
		else if (i == 2) cp = QPoint(m_levelsRect[0].right() + m_levelsRect[0].width(), 315);
		drawText(p, n[i], cp, true, 20, 5, &(m_levelsRect[i]), m_overLevels[i], true);
	}
}

void KSimon::drawText(QPainter &p, const QString &text, const QPoint &center, bool withMargin, int xMargin, int yMargin, QRect *rect, bool highlight, bool bold)
{
	QRect r;
	QFont oldFont, f = p.font();
	oldFont = f;
	f.setPointSize(fontUtils::fontSize(p, text, 190, 30));
	if (bold) f.setBold(true);
	p.setFont(f);
	
	r = p.boundingRect(QRect(), Qt::AlignAuto, text);
	r = QRect(0, 0, r.width() + xMargin, r.height() + yMargin);
	r.moveBy(center.x() - r.width() / 2, center.y() - r.height() / 2);
	
	if (withMargin)
	{
		p.fillRect(r, m_fillColor);
		p.setPen(QPen(black, 3));
		p.drawRoundRect(r.left(), r.top(), r.width(), r.height(), 15, 15);
	}
	
	if (!highlight) p.setPen(m_fontColor);
	else p.setPen(m_fontHighlightColor);
	p.drawText(r, Qt::AlignCenter, text);
	
	if (rect) *rect = r;
	p.setFont(oldFont);
}


void KSimon::updateButtonHighlighting(const QPoint &p)
{
	bool haveToUpdate;
	m_updateButtonHighlighting = false;
	haveToUpdate = false;
	
	if (m_highscoreRect.contains(p))
	{
		if (!m_overHighscore)
		{
			m_overHighscore = true;
			haveToUpdate = true;
		}
	}
	else if (m_overHighscore)
	{
		m_overHighscore = false;
		haveToUpdate = true;
	}
	
	if (m_menuRect.contains(p))
	{
		if (!m_overMenu)
		{
			m_overMenu = true;
			m_overAboutKDE = false;
			m_overAboutKSimon = false;
			m_overManual = false;
			haveToUpdate = true;
		}
		else if (m_overAboutKDE || m_overAboutKSimon || m_overManual)
		{
			m_overAboutKDE = false;
			m_overAboutKSimon = false;
			m_overManual = false;
			haveToUpdate = true;
		}
	}
	else if (m_overMenu)
	{
		if (m_aboutKDERect.contains(p))
		{
			if (!m_overAboutKDE)
			{
				m_overAboutKDE = true;
				m_overAboutKSimon = false;
				m_overManual = false;
				haveToUpdate = true;
			}
		}
		else if (m_aboutKSimonRect.contains(p))
		{
			if (!m_overAboutKSimon)
			{
				m_overAboutKDE = false;
				m_overAboutKSimon = true;
				m_overManual = false;
				haveToUpdate = true;
			}
		}
		else if (m_manualRect.contains(p))
		{
			if (!m_overManual)
			{
				m_overAboutKDE = false;
				m_overAboutKSimon = false;
				m_overManual = true;
				haveToUpdate = true;
			}
		}
		else
		{
			m_overMenu = false;
			m_overAboutKDE = false;
			m_overAboutKSimon = false;
			m_overManual = false;
			haveToUpdate = true;
		}
	}
	
	if (m_quitRect.contains(p))
	{
		if (!m_overQuit)
		{
			m_overQuit = true;
			haveToUpdate = true;
		}
	}
	else if (m_overQuit)
	{
		m_overQuit = false;
		haveToUpdate = true;
	}
	
	switch (m_game.phase())
	{
		case simonGame::starting:
		case simonGame::waiting3:
		case simonGame::waiting2:
		case simonGame::waiting1:
		case simonGame::learningTheSequence:
		case simonGame::typingTheSequence:
			if (m_centralTextRect.contains(p))
			{
				if (!m_overCentralText)
				{
					m_overCentralText = true;
					haveToUpdate = true;
				}
			}
			else if (m_overCentralText)
			{
				m_overCentralText = false;
				haveToUpdate = true;
			}
		break;
		
		case simonGame::choosingLevel:
			for (int i = 0; i < 3; i++)
			{
				if (m_levelsRect[i].contains(p))
				{
					if (!m_overLevels[i])
					{
						m_overLevels[i] = true;
						haveToUpdate = true;
					}
				}
				else if (m_overLevels[i])
				{
					m_overLevels[i] = false;
					haveToUpdate = true;
				}
			}
		break;
	}

	if (haveToUpdate) update();
}

#include "ksimon.moc"
