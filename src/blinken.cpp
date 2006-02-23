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

#include "blinken.h"
#include "button.h"
#include "counter.h"
#include "fontutils.h"
#include "number.h"
#include "highscoredialog.h"
#include "settings.h"

blinken::blinken() : QWidget(0, 0, WStaticContents | WNoAutoErase), m_overHighscore(false), m_overQuit(false), m_overCentralText(false), m_overMenu(false), m_overAboutKDE(false), m_overAboutBlinken(false), m_overManual(false), m_overCentralLetters(false), m_overCounter(false), m_overFont(false), m_overSound(false), m_showPreferences(false), m_updateButtonHighlighting(false), m_highlighted(blinkenGame::none)
{
	m_back = new QPixmap(locate("appdata", "images/blinken.png"));
	
	m_buttons[0] = new button(blinkenGame::blue);
	m_buttons[1] = new button(blinkenGame::yellow);
	m_buttons[2] = new button(blinkenGame::red);
	m_buttons[3] = new button(blinkenGame::green);
	
	m_highscore = new QPixmap(locate("appdata", "images/highscore.png"));
	m_highscoreHover = new QPixmap(locate("appdata", "images/highscore_hover.png"));
	m_quit = new QPixmap(locate("appdata", "images/quit.png"));
	m_quitHover = new QPixmap(locate("appdata", "images/quit_hover.png"));
	m_menu = new QPixmap(locate("appdata", "images/menu.png"));
	m_menuHover = new QPixmap(locate("appdata", "images/menu_hover.png"));
	m_mark = new QPixmap(locate("appdata", "images/mark.png"));
	m_highscoreRect = QRect(10, 10, 72, 72);
	m_quitRect = QRect(562, 10, 72, 73);
	m_menuRect = QRect(562, 443, 72, 72);
	m_aboutKDERect = QRect(452, 461, 54, 54);
	m_aboutBlinkenRect = QRect(506, 461, 56, 54);
	m_manualRect = QRect(578, 389, 54, 54);
	m_centralLettersRect = QRect(192, 194, 254, 54);
	m_counterRect = QRect(268, 110, 102, 52);
	
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
	connect(&m_game, SIGNAL(highlight(blinkenGame::color, bool)), this, SLOT(highlight(blinkenGame::color, bool)));
	
	m_helpMenu = new KHelpMenu(this, kapp->aboutData());
	
	for (int i = 0; i < 3; i++) m_overLevels[i] = false;
	
	QString aux = i18n("If the Steve font that is used by bliken by default to show status messages does not support any of the characters of your language, please translate that message to 1 and KDE standard font will be used to show the texts, if not translate it to 0", "0");
	
	m_alwaysUseNonCoolFont = aux == "1";
}

blinken::~blinken()
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

void blinken::paintEvent(QPaintEvent *)
{
	QPixmap buf(width(), height());
	QPainter p(&buf);
	
	p.drawPixmap(0, 0, *m_back);
	
	drawMenuQuit(p);
	QFont f1, f = p.font();
	f1 = f;
	f.setBold(true);
	p.setFont(f);
	switch (m_game.phase())
	{
		case blinkenGame::starting:
			drawText(p, i18n("Start"), QPoint(318, 316), true, 10, 5, &m_centralTextRect, m_overCentralText, false);
		break;
		
		case blinkenGame::choosingLevel:
			drawLevel(p);
		break;
		
		case blinkenGame::waiting3:
		case blinkenGame::waiting2:
		case blinkenGame::waiting1:
		case blinkenGame::learningTheSequence:
		case blinkenGame::typingTheSequence:
			drawText(p, i18n("Restart"), QPoint(318, 316), true, 10, 5, &m_centralTextRect, m_overCentralText, false);
		break;
	}
	p.setFont(f1);
	
	if (m_showPreferences)
	{
		// draw the current keys
		drawText(p, m_buttons[0]->shortcut(), QPoint(115, 285), true, 20, 5, 0, m_buttons[0]->selected(), false);
		drawText(p, m_buttons[1]->shortcut(), QPoint(115, 155), true, 20, 5, 0, m_buttons[1]->selected(), false);
		drawText(p, m_buttons[2]->shortcut(), QPoint(520, 155), true, 20, 5, 0, m_buttons[2]->selected(), false);
		drawText(p, m_buttons[3]->shortcut(), QPoint(520, 285), true, 20, 5, 0, m_buttons[3]->selected(), false);

		const QPen &oPen = p.pen();
		const QBrush &oBrush = p.brush();
		const QFont &oFont = p.font();
		
		// draw the internal square plus a bit of ellipse on the sides and the
		// two delimiter lines 
		p.setPen(NoPen);
		p.setBrush(m_fillColor);
		p.drawPie(169, 192, 4, 58, 1440, 2880);
		p.drawPie(465, 192, 4, 58, -1440, 2880);
		p.setPen(QPen(Qt::black, 3));
		p.fillRect(171, 192, 296, 58, m_fillColor);
		p.drawLine(169, 192, 469, 192);
		p.drawLine(169, 250, 469, 250);

		// draw the two squares of the options
		p.setPen(QPen(m_fontColor, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));

		m_soundRect = QRect(181, 209, 25, 25);
		m_fontRect = QRect(432, 209, 25, 25);
#ifndef WITHOUT_ARTS
		p.drawRect(m_soundRect);
		if (blinkenSettings::playSounds())
		{
			p.drawLine(186, 214, 199, 227); 
			p.drawLine(186, 227, 199, 214); 
		}
#endif
		if (!m_alwaysUseNonCoolFont)
		{
			p.drawRect(m_fontRect);
			if (blinkenSettings::customFont())
			{
				p.drawLine(437, 214, 450, 227); 
				p.drawLine(437, 227, 450, 214); 
			}
		}

		// draw the options' text
		QFont f1 = QFont();
		p.setFont(f1);
		int size, sizeAux;
		QRect area;
		QString sounds = i18n("Sounds");
		QString font = i18n("Font");
		size = fontUtils::fontSize(p, sounds, 95, 20);
		sizeAux = fontUtils::fontSize(p, font, 95, 20);
		if (sizeAux > size) size = sizeAux;
		f1.setPointSize(size);
#ifndef WITHOUT_ARTS
		area = p.boundingRect(QRect(), Qt::AlignAuto, sounds);
		area.moveBy(212, 221 - (area.height() / 2));
		p.drawText(area, Qt::AlignCenter, sounds);
		m_soundRect = m_soundRect.unite(area);
#endif
		if (!m_alwaysUseNonCoolFont)
		{
			area = p.boundingRect(QRect(), Qt::AlignAuto, font);
			area.moveBy(426 - area.width(), 221 - (area.height() / 2));
			p.drawText(area, Qt::AlignCenter, font);
			m_fontRect = m_fontRect.unite(area);
		}
		
		p.setFont(oFont);
		p.setPen(oPen);
		p.setBrush(oBrush);
	}
	
	drawScoreAndCounter(p);
	
	if (m_highlighted & blinkenGame::blue) p.drawPixmap(14, 225, *m_buttons[0] -> pixmap());
	if (m_highlighted & blinkenGame::yellow) p.drawPixmap(14, 16, *m_buttons[1] -> pixmap());
	if (m_highlighted & blinkenGame::red) p.drawPixmap(322, 16, *m_buttons[2] -> pixmap());
	if (m_highlighted & blinkenGame::green) p.drawPixmap(322, 225, *m_buttons[3] -> pixmap());
	
	drawStatusText(p);
	
	bitBlt(this, 0, 0, &buf);
	
	if (m_updateButtonHighlighting) updateButtonHighlighting(mapFromGlobal(QCursor::pos()));
}

void blinken::keyPressEvent(QKeyEvent *e)
{
	if (e -> isAutoRepeat()) return;
	
	if (m_showPreferences)
	{
		int i = 0;
		while (i < 4 && !m_buttons[i] -> selected()) i++;
	
		if (i < 4)
		{
			if (e -> key() == Qt::Key_Escape)
			{
				m_buttons[i] -> setSelected(false);
				update();
			}
			else
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
	}
	else
	{
		if (e -> key() == m_buttons[0] -> key()) pressedBlue();
		else if (e -> key() == m_buttons[1] -> key()) pressedYellow();
		else if (e -> key() == m_buttons[2] -> key()) pressedRed();
		else if (e -> key() == m_buttons[3] -> key()) pressedGreen();
	}
}

void blinken::keyReleaseEvent(QKeyEvent *e)
{
	if (e -> isAutoRepeat()) return;
	
	if (e -> state() == Qt::ControlButton && e -> stateAfter() != Qt::ControlButton && (m_game.phase() == blinkenGame::starting || m_game.phase() == blinkenGame::choosingLevel))
	{
		m_showPreferences = !m_showPreferences;
		for (int i = 0; i < 4; i++) m_buttons[i] -> setSelected(false);
		updateButtonHighlighting(mapFromGlobal(QCursor::pos()));
		update();
	}
}

void blinken::mouseMoveEvent(QMouseEvent *e)
{
	updateButtonHighlighting(e->pos());
}

void blinken::mousePressEvent(QMouseEvent *e)
{
	if (m_overHighscore || m_overCounter)
	{
		highScoreDialog *hsd = new highScoreDialog(this);
		hsd->showLevel(1);
		m_updateButtonHighlighting = true;
	}
	else if (m_showPreferences && m_fontRect.contains(e -> pos()) && !m_alwaysUseNonCoolFont)
	{
		blinkenSettings::setCustomFont(!blinkenSettings::customFont());
		blinkenSettings::writeConfig();
		update();
	}
	else if (m_showPreferences && m_soundRect.contains(e -> pos()))
	{
		blinkenSettings::setPlaySounds(!blinkenSettings::playSounds());
		blinkenSettings::writeConfig();
		update();
	}
	else if (m_overQuit) kapp->quit();
	else if (m_overAboutBlinken || m_overCentralLetters) m_helpMenu -> aboutApplication();
	else if (m_overAboutKDE) m_helpMenu -> aboutKDE();
	else if (m_overManual) m_helpMenu -> appHelpActivated();
	else if (m_game.phase() != blinkenGame::choosingLevel && m_overCentralText)
	{
		highlight(blinkenGame::none, true);
		m_overCentralText = false;
		for(int i = 0; i < 3; i++) m_overLevels[i] = false;
		m_game.setPhase(blinkenGame::choosingLevel);
		m_updateButtonHighlighting = true;
	}
	else if (m_game.phase() == blinkenGame::choosingLevel)
	{
		int level = 0;
		if (m_levelsRect[1].contains(e -> pos())) level = 1;
		else if (m_levelsRect[0].contains(e -> pos())) level = 2;
		else if (m_levelsRect[2].contains(e -> pos())) level = 3;
		if (level) 
		{
			for(int i = 0; i < 3; i++) m_overLevels[i] = false;
			m_game.start(level);
			if (m_showPreferences) m_showPreferences = false;
			m_updateButtonHighlighting = true;
		}
	}
	
	QPoint p = e->pos();
	p -= QPoint(319, 221);
	if (insideGreen(p))
	{
		if (m_showPreferences) selectButton(3);
		else pressedGreen();
	}
	else if (insideBlue(p))
	{
		if (m_showPreferences) selectButton(0);
		else pressedBlue();
	}
	else if (insideYellow(p))
	{
		if (m_showPreferences) selectButton(1);
		else pressedYellow();
	}
	else if (insideRed(p))
	{
		if (m_showPreferences) selectButton(2);
		else pressedRed();
	}
}

void blinken::checkHS()
{
	highScoreDialog *hsd = new highScoreDialog(this);
	if (hsd->scoreGoodEnough(m_game.level(), m_game.score()))
	{
		bool ok;
		QString name = KInputDialog::getText(i18n("Enter Your Name"), i18n("Name:"), m_lastName, &ok);
		if (!name.isNull() && ok)
		{
			m_lastName = name;
			hsd->addScore(m_game.level(), m_game.score(), name);
		}
		hsd->showLevel(m_game.level());
	}
}

void blinken::highlight(blinkenGame::color c, bool unhighlight)
{
	m_highlighted = c;
	update();
	if (unhighlight) m_unhighlighter -> start(250);
	else if (c == blinkenGame::none)
	{
		m_unhighlighter -> stop();
		updateCursor(mapFromGlobal(QCursor::pos()));
	}
}

void blinken::unhighlight()
{
	highlight(blinkenGame::none, false);
}

void blinken::pressedYellow()
{
	if (m_game.canType())
	{
		highlight(blinkenGame::yellow, true);
		m_game.clicked(blinkenGame::yellow);
	}
}

void blinken::pressedRed()
{
	if (m_game.canType())
	{
		highlight(blinkenGame::red, true);
		m_game.clicked(blinkenGame::red);
	}
}

void blinken::pressedGreen()
{
	if (m_game.canType())
	{
		highlight(blinkenGame::green, true);
		m_game.clicked(blinkenGame::green);
	}
}

void blinken::pressedBlue()
{
	if (m_game.canType())
	{
		highlight(blinkenGame::blue, true);
		m_game.clicked(blinkenGame::blue);
	}
}

void blinken::selectButton(int button)
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

bool blinken::insideGreen(const QPoint &p) const
{
	return insideButtonsArea(p) && p.x() > 6 && p.y() > 6;
}

bool blinken::insideBlue(const QPoint &p) const
{
	return insideButtonsArea(p) && p.x() < -6 && p.y() > 6;
}

bool blinken::insideYellow(const QPoint &p) const
{
	return insideButtonsArea(p) && p.x() < -6 && p.y() < -6;
}

bool blinken::insideRed(const QPoint &p) const
{
	return insideButtonsArea(p) && p.x() > 6 && p.y() < -6;
}

bool blinken::insideButtonsArea(const QPoint &p) const
{
	double x, y, x2, y2;
	x = (double)p.x();
	y = (double)p.y();
	x2 = x * x;
	y2 = y * y;
	if (x2 + y2 > 162.5 * 162.5)
	{
		// Outside the circle
		double x3, y3;
		x3 = x2 / (301 * 301);
		y3 = y2 / (201 * 201);
		if (x3 + y3 < 1) return true;
	}
	return false;
}

void blinken::drawMenuQuit(QPainter &p)
{
	if (!m_overHighscore) p.drawPixmap(10, 10, *m_highscore);
	else p.drawPixmap(10, 10, *m_highscoreHover);
	
	if (!m_overQuit) p.drawPixmap(562, 10, *m_quit);
	else p.drawPixmap(562, 10, *m_quitHover);
	
	if (!m_overMenu) p.drawPixmap(562, 443, *m_menu);
	else p.drawPixmap(454, 389, *m_menuHover);
	
	if (m_overAboutKDE) p.drawPixmap(462, 433, *m_mark);
	else if (m_overAboutBlinken) p.drawPixmap(516, 433, *m_mark);
	else if (m_overManual)
	{
		p.translate(550, 430);
		p.rotate(-90.0);
		p.drawPixmap(0, 0, *m_mark);
		p.rotate(90.0);
		p.translate(-550, -430);
	}
}

void blinken::drawScoreAndCounter(QPainter &p)
{
	QColor c1, c2, c3;
	p.translate(268, 110);
	
	switch (m_game.phase())
	{
		case blinkenGame::waiting3:
			c1 = red;
			c2 = red;
			c3 = red;
		break;
		
		case blinkenGame::waiting2:
			c1 = m_countDownColor;
			c2 = red;
			c3 = red;
		break;
		
		case blinkenGame::waiting1:
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
	
	counter::paint(p, m_game.phase() != blinkenGame::starting, m_game.score(), true, c1, c2, c3);
	
	p.translate(-268, -110);
}

void blinken::drawStatusText(QPainter &p)
{
	p.translate(25, 505);
	p.rotate(-3.29);
	p.setPen(blue);

	QString restartText = i18n("Restart the game");
	QString text;
	if (m_overQuit) text = i18n("Quit blinKen");
	else if (m_overHighscore || m_overCounter) text = i18n("View Highscore Table");
	else if (m_overAboutBlinken || m_overCentralLetters) text = m_helpMenu -> menu() -> text(KHelpMenu::menuAboutApp).remove("&");
	else if (m_overAboutKDE) text = m_helpMenu -> menu() -> text(KHelpMenu::menuAboutKDE).remove("&");
	else if (m_overManual) text = m_helpMenu -> menu() -> text(KHelpMenu::menuHelpContents).remove("&");
	else if (m_overLevels[0]) text = i18n("2nd Level");
	else if (m_overLevels[1]) text = i18n("1st Level");
	else if (m_overLevels[2]) text = i18n("Random Level");
	else if (m_buttons[0]->selected() || m_buttons[1]->selected() || m_buttons[2]->selected() || m_buttons[3]->selected()) text = i18n("Press the key for this button");
	else if (m_showPreferences) text = i18n("Click any button to change its key");
	else
	{
		switch (m_game.phase())
		{
			case blinkenGame::starting:
				text = i18n("Press Start to begin!");
			break;
			
			case blinkenGame::choosingLevel:
				text = i18n("Set the Difficulty Level...");
			break;
			
			case blinkenGame::waiting3:
				if (m_overCentralText) text = restartText;
				else text = i18n("Next sequence in 3...");
			break;
			
			case blinkenGame::waiting2:
				if (m_overCentralText) text = restartText;
				else if (m_game.level() == 1) text = i18n("Next sequence in 3, 2...");
				else text = i18n("Next sequence in 2...");
			break;
		
			case blinkenGame::waiting1:
				if (m_overCentralText) text = restartText;
				else if (m_game.level() == 1) text = i18n("Next sequence in 3, 2, 1...");
				else text = i18n("Next sequence in 2, 1...");
			break;
			
			case blinkenGame::learningTheSequence:
				if (m_overCentralText) text = restartText;
				else text = i18n("Remember this sequence...");
			break;
			
			case blinkenGame::typingTheSequence:
				if (m_overCentralText) text = restartText;
				else text = i18n("Repeat the sequence!");
			break;
		}
	}
	
	QFont f;
	if (blinkenSettings::customFont() && !m_alwaysUseNonCoolFont) f = QFont("Steve");
	p.setFont(f);
	f.setPointSize(fontUtils::fontSize(p, text, 380, 30));
	p.setFont(f);
	p.drawText(0, 0, text);
}

void blinken::drawLevel(QPainter &p)
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

void blinken::drawText(QPainter &p, const QString &text, const QPoint &center, bool withMargin, int xMargin, int yMargin, QRect *rect, bool highlight, bool bold)
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


void blinken::updateButtonHighlighting(const QPoint &p)
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
			m_overAboutBlinken = false;
			m_overManual = false;
			haveToUpdate = true;
		}
		else if (m_overAboutKDE || m_overAboutBlinken || m_overManual)
		{
			m_overAboutKDE = false;
			m_overAboutBlinken = false;
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
				m_overAboutBlinken = false;
				m_overManual = false;
				haveToUpdate = true;
			}
		}
		else if (m_aboutBlinkenRect.contains(p))
		{
			if (!m_overAboutBlinken)
			{
				m_overAboutKDE = false;
				m_overAboutBlinken = true;
				m_overManual = false;
				haveToUpdate = true;
			}
		}
		else if (m_manualRect.contains(p))
		{
			if (!m_overManual)
			{
				m_overAboutKDE = false;
				m_overAboutBlinken = false;
				m_overManual = true;
				haveToUpdate = true;
			}
		}
		else
		{
			m_overMenu = false;
			m_overAboutKDE = false;
			m_overAboutBlinken = false;
			m_overManual = false;
			haveToUpdate = true;
		}
	}
	
	if (!m_showPreferences && m_centralLettersRect.contains(p))
	{
		m_overCentralLetters = true;
		haveToUpdate = true;
	}
	else if (m_overCentralLetters)
	{
		m_overCentralLetters = false;
		haveToUpdate = true;
	}
	
	if (m_showPreferences && m_soundRect.contains(p))
	{
		m_overSound = true;
		haveToUpdate = true;
	}
	else if (m_overSound)
	{
		m_overSound = false;
		haveToUpdate = true;
	}
	
	if (m_showPreferences && m_fontRect.contains(p) && !m_alwaysUseNonCoolFont)
	{
		m_overFont = true;
		haveToUpdate = true;
	}
	else if (m_overFont)
	{
		m_overFont = false;
		haveToUpdate = true;
	}
	
	if (m_counterRect.contains(p))
	{
		m_overCounter = true;
		haveToUpdate = true;
	}
	else if (m_overCounter)
	{
		m_overCounter = false;
		haveToUpdate = true;
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
		case blinkenGame::starting:
		case blinkenGame::waiting3:
		case blinkenGame::waiting2:
		case blinkenGame::waiting1:
		case blinkenGame::learningTheSequence:
		case blinkenGame::typingTheSequence:
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
		
		case blinkenGame::choosingLevel:
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

	updateCursor(p);
	if (haveToUpdate) update();
}

void blinken::updateCursor(const QPoint &p)
{
	QPoint p2 = p - QPoint(319, 221);
	
	if (m_overHighscore || m_overQuit || m_overCentralText || m_overMenu || m_overAboutKDE || m_overAboutBlinken || m_overManual  || m_overLevels[0] || m_overLevels[1] || m_overLevels[2] || m_overCentralLetters || m_overCounter || (m_game.canType() && (insideGreen(p2) || insideRed(p2) || insideBlue(p2) || insideYellow(p2))) || m_overFont || m_overSound) setCursor(PointingHandCursor);
	else setCursor(ArrowCursor);
}

#include "blinken.moc"
