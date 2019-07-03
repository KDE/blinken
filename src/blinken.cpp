/***************************************************************************
 *   Copyright (C) 2005-2007 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "blinken.h"

#include <QCursor>
#include <QEvent>
#include <QPainter>
#include <QSvgRenderer>
#include <QTimer>
#include <QAction>
#include <QApplication>
#include <QInputDialog>
#include <QKeySequence>
#include <QMouseEvent>

#include <kconfig.h>
#include <khelpmenu.h>
#include <kfontutils.h>
#include <klocalizedstring.h>
#include <KAboutData>

#include "button.h"
#include "counter.h"
#include "number.h"
#include "highscoredialog.h"
#include "settings.h"

static const double centerX = 2.0;
static const double centerY = 2.5;
static const double ellipseSmallAxisX = 3.98;
static const double ellipseSmallAxisY = 3.41;
static const double ellipseBigAxisX = 2.16;
static const double ellipseBigAxisY = 2.74;
static const double nonButtonRibbonX = 150.0;
static const double nonButtonRibbonY = 125.0;

blinken::blinken() : m_overHighscore(false), m_overQuit(false), m_overCentralText(false), m_overMenu(false), m_overAboutKDE(false), m_overAboutBlinken(false), m_overSettings(false), m_overManual(false), m_overCentralLetters(false), m_overCounter(false), m_overFont(false), m_overSound(false), m_showPreferences(false), m_updateButtonHighlighting(false), m_highlighted(blinkenGame::none)
{
	m_renderer = new QSvgRenderer(QStandardPaths::locate(QStandardPaths::DataLocation, QStringLiteral("images/blinken.svg")));
	
	m_buttons[0] = new button(blinkenGame::blue);
	m_buttons[1] = new button(blinkenGame::yellow);
	m_buttons[2] = new button(blinkenGame::red);
	m_buttons[3] = new button(blinkenGame::green);
	
	m_fillColor = QColor(40, 40, 40);
	m_fontColor = QColor(126, 126, 126);
	m_fontHighlightColor  = QColor(230, 230, 230);
	m_countDownColor = QColor(55, 55, 55);
	
	setMouseTracking(true);
	show();
	
	m_unhighlighter = new QTimer(this);
	connect(m_unhighlighter, &QTimer::timeout, this, &blinken::unhighlight);
	
	connect(&m_game, &blinkenGame::gameEnded, this, &blinken::checkHS);
	connect(&m_game, SIGNAL(phaseChanged()), this, SLOT(update()));
	connect(&m_game, &blinkenGame::highlight, this, &blinken::highlight);
	
	m_helpMenu = new KHelpMenu(this, KAboutData::applicationData());
	m_helpMenu->menu(); // ensures the actions are created
	
	for (int i = 0; i < 3; i++) m_overLevels[i] = false;
	
	QString aux = i18nc("If the Steve font that is used by Blinken by default to show status messages does not support any of the characters of your language, please translate that message to 1 and KDE standard font will be used to show the texts, if not translate it to 0", "0");
	
	m_alwaysUseNonCoolFont = aux == QStringLiteral("1");
	setAutoSaveSettings();
}

blinken::~blinken()
{
	delete m_renderer;
	for (int i = 0; i < 4; i++) delete m_buttons[i];
	delete m_helpMenu;
}

QSize blinken::sizeHint() const
{
	return QSize(600, 490);
}

void blinken::paintEvent(QPaintEvent *)
{
	m_centralLettersRect = QRectF((double)width() / 3.35,
	                              (double)height() / 2.68,
	                              (double)width() / 2.54,
	                              (double)height() / 10.0);
	m_counterRect = QRectF((double)width() / 2.42,
	                              (double)height() / 4.85,
	                              (double)width() / 6.22,
	                              (double)height() / 9.8);

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing, true);
	
	if (m_lastSize != size())
	{
		m_pixmapCache.clear();
		m_lastSize = size();
	}
	
	// Base
	// This -1 +1 suck but without them i got blank lines on borders
	p.drawPixmap(-1, 0, getPixmap(QStringLiteral("blinkenBase"), size()+QSize(1,1)));
	
	double xScaleButtons = 374.625 / 814.062;
	double yScaleButtons = 250.344 / 696.5;

	const auto sz = QSize((int)(width() * xScaleButtons), (int)(height() * yScaleButtons));

	auto getPixmapFor = [this, sz](blinkenGame::color color, const QString& pixmapName) -> QPixmap {
		return getPixmap( m_highlighted & color ?
			QStringLiteral("%1_highlight").arg(pixmapName) :
			QStringLiteral("%1_normal").arg(pixmapName), sz);
	};

	p.drawPixmap(QPointF(width() / 1.975, height() / 28.0), getPixmapFor(blinkenGame::red, QStringLiteral("red")));
	p.drawPixmap(QPointF(width() / 1.975, height() / 2.45), getPixmapFor(blinkenGame::green, QStringLiteral("green")));
	p.drawPixmap(QPointF(width() / 30.0, height() / 28.0), getPixmapFor(blinkenGame::yellow, QStringLiteral("yellow")));
	p.drawPixmap(QPointF(width() / 30.0, height() / 2.45), getPixmapFor(blinkenGame::blue, QStringLiteral("blue")));

	drawMenuQuit(p);
	p.resetTransform();
	
	// 644 525 are fixed size of preSVG blinken
	p.scale((double)width()/644.0, (double)height()/525.0);
	if (m_showPreferences)
	{
		// draw the current keys
		drawText(p, m_buttons[0]->shortcut(), QPoint(115, 285), true, 20, 5, nullptr, m_buttons[0]->selected(), false);
		drawText(p, m_buttons[1]->shortcut(), QPointF(115, 155), true, 20, 5, nullptr, m_buttons[1]->selected(), false);
		drawText(p, m_buttons[2]->shortcut(), QPointF(520, 155), true, 20, 5, nullptr, m_buttons[2]->selected(), false);
		drawText(p, m_buttons[3]->shortcut(), QPointF(520, 285), true, 20, 5, nullptr, m_buttons[3]->selected(), false);

		const QPen &oPen = p.pen();
		const QBrush &oBrush = p.brush();
		const QFont &oFont = p.font();
		
		// draw the internal square plus a bit of ellipse on the sides and the
		// two delimiter lines
		p.setPen(Qt::NoPen);
		p.setBrush(m_fillColor);
		p.drawPie(173, 180, 6, 58, 1440, 2880);
		p.drawPie(470, 180, 6, 58, -1440, 2880);
		p.setPen(QPen(Qt::black, 3));
		p.fillRect(175, 180, 299, 58, m_fillColor);
		p.drawLine(175, 180, 472, 180);
		p.drawLine(175, 238, 472, 238);

		// draw the two squares of the options
		p.setPen(QPen(m_fontColor, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));

		m_soundRect = QRect(181, 197, 25, 25);
		m_fontRect = QRect(432, 197, 25, 25);
		p.drawRect(m_soundRect);
		if (blinkenSettings::playSounds())
		{
			p.drawLine(186, 202, 201, 217);
			p.drawLine(186, 217, 201, 202);
		}
		if (!m_alwaysUseNonCoolFont)
		{
			p.drawRect(m_fontRect);
			if (blinkenSettings::customFont())
			{
				p.drawLine(437, 202, 452, 217);
				p.drawLine(437, 217, 452, 202);
			}
		}

		// draw the options' text
		QFont f1 = QFont();
		p.setFont(f1);
		int size, sizeAux;
		QRect area;
		QString sounds = i18n("Sounds");
		QString font = i18n("Font");
		size = KFontUtils::adaptFontSize(p, sounds, 95, 20, 28, 1, KFontUtils::DoNotAllowWordWrap);
		sizeAux = KFontUtils::adaptFontSize(p, font, 95, 20, 28, 1, KFontUtils::DoNotAllowWordWrap);
		if (sizeAux > size) size = sizeAux;
		f1.setPointSize(size);
		area = p.boundingRect(QRect(), Qt::AlignLeft, sounds);
		area.translate(212, 209 - (area.height() / 2));
		p.drawText(area, Qt::AlignCenter, sounds);
		m_soundRect = m_soundRect.united(area);
		if (!m_alwaysUseNonCoolFont)
		{
			area = p.boundingRect(QRect(), Qt::AlignLeft, font);
			area.translate(426 - area.width(), 209 - (area.height() / 2));
			p.drawText(area, Qt::AlignCenter, font);
			m_fontRect = m_fontRect.united(area);
		}
		
		p.setFont(oFont);
		p.setPen(oPen);
		p.setBrush(oBrush);
		
		// store the "real" rect
		m_fontRect = p.worldTransform().mapRect(m_fontRect);
		m_soundRect = p.worldTransform().mapRect(m_soundRect);
	}
	
	drawScoreAndCounter(p);
	
	drawStatusText(p);
	
	p.resetTransform();
	p.setFont(QFont()); //Go back to normal font
	// Start / Restart button
	// We don't use the scale matrix because this way we get a better font
	double aux1, aux2, aux3, aux4;
	aux1 = (double)width() / 2.02515723270440251572;
	aux2 = (double)height() / 1.66139240506329113924;
	aux3 = (double)width() / 64.4;
	aux4 = (double)height() / 105.0;
	switch (m_game.phase())
	{
		case blinkenGame::starting:
			drawText(p, i18nc("@action:button Start a new game", "Start"), QPointF(aux1, aux2), true, aux3, aux4, &m_centralTextRect, m_overCentralText, true);
		break;
		
		case blinkenGame::choosingLevel:
			drawLevel(p);
		break;
		
		case blinkenGame::waiting3:
		case blinkenGame::waiting2:
		case blinkenGame::waiting1:
		case blinkenGame::learningTheSequence:
		case blinkenGame::typingTheSequence:
			drawText(p, i18n("Restart"), QPointF(aux1, aux2), true, aux3, aux4, &m_centralTextRect, m_overCentralText, true);
		break;
	}
	
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
				QKeySequence ks(e -> key());
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
						m_buttons[i] -> setShortcut(e->key());
						update();
					}
				}
			}
		}
	}
	else
	{
		if (m_game.phase() == blinkenGame::starting)
		{
			if (e -> key() == Qt::Key_Return || e -> key() == Qt::Key_Enter)
			{
				startGamePressed();
				return;
			}
		}
		else if (m_game.phase() == blinkenGame::choosingLevel)
		{
			if (e -> key() == Qt::Key_1)
			{
				startGameAtLevel(1);
				return;
			}
			else if (e -> key() == Qt::Key_2)
			{
				startGameAtLevel(2);
				return;
			}
			else if (e -> key() == Qt::Key_3)
			{
				startGameAtLevel(3);
				return;
			}
		}
		
		if (e -> key() == m_buttons[0] -> key()) pressedColor(blinkenGame::color::blue);
		else if (e -> key() == m_buttons[1] -> key()) pressedColor(blinkenGame::color::yellow);
		else if (e -> key() == m_buttons[2] -> key()) pressedColor(blinkenGame::color::red);
		else if (e -> key() == m_buttons[3] -> key()) pressedColor(blinkenGame::color::green);
	}
}

void blinken::keyReleaseEvent(QKeyEvent *e)
{
	if (e -> isAutoRepeat()) return;
	
	if (e -> key() == Qt::Key_Control)
	{
		togglePreferences();
	}
}

void blinken::togglePreferences()
{
	if (m_game.phase() == blinkenGame::starting || m_game.phase() == blinkenGame::choosingLevel)
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
		highScoreDialog *hsd = new highScoreDialog(this, m_renderer);
		hsd->showLevel(1);
		m_updateButtonHighlighting = true;
	}
	else if (m_showPreferences && m_fontRect.contains(e -> pos()) && !m_alwaysUseNonCoolFont)
	{
		blinkenSettings::setCustomFont(!blinkenSettings::customFont());
		blinkenSettings::self()->save();
		update();
	}
	else if (m_showPreferences && m_soundRect.contains(e -> pos()))
	{
		blinkenSettings::setPlaySounds(!blinkenSettings::playSounds());
		blinkenSettings::self()->save();
		update();
	}
	else if (m_overQuit) qApp->quit();
	else if (m_overAboutBlinken || m_overCentralLetters) m_helpMenu -> aboutApplication();
	else if (m_overAboutKDE) m_helpMenu -> aboutKDE();
	else if (m_overSettings) togglePreferences();
	else if (m_overManual) m_helpMenu -> appHelpActivated();
	else if (m_game.phase() != blinkenGame::choosingLevel && m_overCentralText)
	{
		startGamePressed();
	}
	else if (m_game.phase() == blinkenGame::choosingLevel)
	{
		int level = 0;
		if (m_levelsRect[1].contains(e -> pos())) level = 1;
		else if (m_levelsRect[0].contains(e -> pos())) level = 2;
		else if (m_levelsRect[2].contains(e -> pos())) level = 3;
		if (level) 
		{
			startGameAtLevel(level);
		}
	}
	
	QPointF p = e->pos();
	p -= QPointF((double)width() / centerX, (double)height() / centerY);
	if (insideGreen(p))
	{
		if (m_showPreferences) selectButton(3);
		else pressedColor(blinkenGame::color::green);
	}
	else if (insideBlue(p))
	{
		if (m_showPreferences) selectButton(0);
		else pressedColor(blinkenGame::color::blue);
	}
	else if (insideYellow(p))
	{
		if (m_showPreferences) selectButton(1);
		else pressedColor(blinkenGame::color::yellow);
	}
	else if (insideRed(p))
	{
		if (m_showPreferences) selectButton(2);
		else pressedColor(blinkenGame::color::red);
	}
}

void blinken::checkHS()
{
	highScoreManager hsm;
	if (hsm.scoreGoodEnough(m_game.level(), m_game.score()))
	{
		bool ok;
		const QString name = QInputDialog::getText(this, i18n("Enter Your Name"), i18nc("@label:textbox refers to the user's name", "Name:"), QLineEdit::Normal, m_lastName, &ok);
		if (!name.isNull() && ok)
		{
			m_lastName = name;
			hsm.addScore(m_game.level(), m_game.score(), name);
		}
		highScoreDialog *hsd = new highScoreDialog(this, m_renderer);
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

void blinken::pressedColor(blinkenGame::color color)
{
	if (m_game.canType())
	{
		highlight(color, true);
		m_game.clicked(color);
	}
}

void blinken::startGamePressed()
{
	highlight(blinkenGame::none, true);
	m_overCentralText = false;
	for(int i = 0; i < 3; i++) m_overLevels[i] = false;
	m_game.setPhase(blinkenGame::choosingLevel);
	m_updateButtonHighlighting = true;
}

void blinken::startGameAtLevel(int level)
{
	for(int i = 0; i < 3; i++) m_overLevels[i] = false;
	m_game.start(level);
	if (m_showPreferences) m_showPreferences = false;
	m_updateButtonHighlighting = true;
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

bool blinken::insideGreen(const QPointF &p) const
{
	// nonButtonRibbon is used so that the buttons are not clickable in the gray space that is in between the buttons
	return insideButtonsArea(p) &&
	       p.x() > (double)width()/nonButtonRibbonX &&
	       p.y() > (double)height()/nonButtonRibbonY;
}

bool blinken::insideBlue(const QPointF &p) const
{
	return insideButtonsArea(p) &&
	       p.x() < -(double)width()/nonButtonRibbonX &&
	       p.y() > (double)height()/nonButtonRibbonY;
}

bool blinken::insideYellow(const QPointF &p) const
{
	return insideButtonsArea(p) &&
	       p.x() < -(double)width()/nonButtonRibbonX &&
	       p.y() < -(double)height()/nonButtonRibbonY;
}

bool blinken::insideRed(const QPointF &p) const
{
	return insideButtonsArea(p) &&
	       p.x() > (double)width()/nonButtonRibbonX &&
	       p.y() < -(double)height()/nonButtonRibbonY;
}

bool blinken::insideButtonsArea(const QPointF &p) const
{
	double x, y, x2, y2;
	x = p.x();
	y = p.y();
	x2 = x * x;
	y2 = y * y;
	double x3, y3;
	double smallaxis1 = (double)width() / ellipseSmallAxisX;
	double smallaxis2 = (double)height() / ellipseSmallAxisY;
	x3 = x2 / (smallaxis1 * smallaxis1);
	y3 = y2 / (smallaxis2 * smallaxis2);
	if (x3 + y3 > 1)
	{
		// Outside the inner ellipse
		double x4, y4;
		double bigaxis1 = (double)width() / ellipseBigAxisX;
		double bigaxis2 = (double)height() / ellipseBigAxisY;
		x4 = x2 / (bigaxis1 * bigaxis1);
		y4 = y2 / (bigaxis2 * bigaxis2);
		if (x4 + y4 < 1) return true;
	}
	return false;
}

void blinken::drawMenuQuit(QPainter &p)
{
	double xScaleSquareButtons = 90.0 / 814.062;
	double yScaleSquareButtons = 90.0 / 696.5;
	
	// Highscore button
	p.resetTransform();
	p.translate( (double)width() / 50.875, (double)height() / 30);
	p.scale(xScaleSquareButtons, yScaleSquareButtons);
	if (m_overHighscore)
	{
		m_renderer->render(&p, QStringLiteral("highscore_hover"));
	}
	else
	{
		m_renderer->render(&p, QStringLiteral("highscore_normal"));
	}
	m_highscoreRect = QRect(qRound((double)width() / 50.875),
	                        qRound((double)height() / 30.0),
	                        qRound((double)width() * xScaleSquareButtons),
	                        qRound((double)height() * yScaleSquareButtons));
	
	// Quit button
	p.resetTransform();
	p.translate( (double)width() / 1.15, (double)height() / 30.0);
	p.scale(xScaleSquareButtons, yScaleSquareButtons);
	if (m_overQuit)
	{
		m_renderer->render(&p, QStringLiteral("quit_hover"));
	}
	else
	{
		m_renderer->render(&p, QStringLiteral("quit_normal"));
	}
	m_quitRect = QRect(qRound((double)width() / 1.15),
	                   qRound((double)height() / 30.0),
	                   qRound((double)width() * xScaleSquareButtons),
	                   qRound((double)height() * yScaleSquareButtons));
	
	// Help button
	p.resetTransform();
	if (m_overMenu)
	{
		double xScaleHoverHelpButton = 225.0 / 814.062;
		double yScaleHoverHelpButton = 225.0 / 696.5;
		p.translate( (double)width() / 1.42, (double)height() / 1.56 );
		p.scale(xScaleHoverHelpButton, yScaleHoverHelpButton);
		m_renderer->render(&p, QStringLiteral("help_hover"));
		
		p.resetTransform();
		double xScaleArrowButton1 = 40.0 / 814.062;
		double yScaleArrowButton1 = 27.5 / 696.5;
		
		double xScaleArrowButton2 = 27.5 / 814.062;
		double yScaleArrowButton2 = 40.0 / 696.5;
		if (m_overAboutKDE)
		{
			p.translate((double)width() / 1.3877, (double)height() / 1.23);
			p.scale(xScaleArrowButton1, yScaleArrowButton1);
			m_renderer->render(&p, QStringLiteral("arrow_down"));
		}
		else if (m_overAboutBlinken)
		{
			p.translate((double)width() / 1.2445, (double)height() / 1.23);
			p.scale(xScaleArrowButton1, yScaleArrowButton1);
			m_renderer->render(&p, QStringLiteral("arrow_down"));
		}
		else if (m_overSettings)
		{
			p.translate((double)width() / 1.174, (double)height() / 1.345);
			p.scale(xScaleArrowButton2, yScaleArrowButton2);
			m_renderer->render(&p, QStringLiteral("arrow_right"));
		}
		else if (m_overManual)
		{
			p.translate((double)width() / 1.174, (double)height() / 1.5);
			p.scale(xScaleArrowButton2, yScaleArrowButton2);
			m_renderer->render(&p, QStringLiteral("arrow_right"));
		}
	}
	else
	{
		p.translate( (double)width() / 1.15, (double)height() / 1.2 );
		p.scale(xScaleSquareButtons, yScaleSquareButtons);
		m_renderer->render(&p, QStringLiteral("help_normal"));
	}
	m_menuRect = QRect(qRound((double)width() / 1.15),
	                   qRound((double)height() / 1.2),
	                   qRound((double)width() * xScaleSquareButtons),
	                   qRound((double)height() * yScaleSquareButtons));
	m_aboutBlinkenRect = QRect(QPoint(qRound((double)width() / 1.271), qRound((double)height() / 1.153)),
	                           QPoint(m_menuRect.left(), m_menuRect.bottom()));
	m_settingsRect = QRect(QPoint(qRound((double)width() / 1.1146), qRound((double)height() / 1.3667)),
	                     QPoint(m_menuRect.right(), m_menuRect.top()));
	m_manualRect = QRect(QPoint(qRound((double)width() / 1.1146), qRound((double)height() / 1.55)),
	                     QPoint(m_menuRect.right(), m_settingsRect.top()));
	m_aboutKDERect = QRect(QPoint(qRound((double)width() / 1.421), qRound((double)height() / 1.153)),
	                       QPoint(m_aboutBlinkenRect.left(), m_aboutBlinkenRect.bottom()));
}

void blinken::drawScoreAndCounter(QPainter &p)
{
	QColor c1, c2, c3;
	p.translate(268, 110);
	
	switch (m_game.phase())
	{
		case blinkenGame::waiting3:
			c1 = Qt::red;
			c2 = Qt::red;
			c3 = Qt::red;
		break;
		
		case blinkenGame::waiting2:
			c1 = m_countDownColor;
			c2 = Qt::red;
			c3 = Qt::red;
		break;
		
		case blinkenGame::waiting1:
			c1 = m_countDownColor;
			c2 = c1;
			c3 = Qt::red;
		break;
		
		default:
			c1 = m_countDownColor;
			c2 = c1;
			c3 = c1;
		break;
	}
	
	counter::paint(p, m_game.phase() != blinkenGame::starting, m_game.score(), true, c1, c2, c3, m_renderer);
	
	p.translate(-268, -110);
}

void blinken::drawStatusText(QPainter &p)
{
	p.translate(25, 505);
	p.rotate(-3.29);
	p.setPen(Qt::blue);

	QString restartText = i18n("Restart the game");
	QString text;
	if (m_overQuit) text = i18n("Quit Blinken");
	else if (m_overHighscore || m_overCounter) text = i18n("View Highscore Table");
	else if (m_overAboutBlinken || m_overCentralLetters) text = m_helpMenu->action( KHelpMenu::menuAboutApp )->text().remove(QLatin1Char('&'));
	else if (m_overAboutKDE) text = m_helpMenu->action( KHelpMenu::menuAboutKDE )->text().remove(QLatin1Char('&'));
	else if (m_overSettings)
	{
		if (!m_showPreferences) text = i18n("Show Settings");
		else text = i18n("Hide Settings");
	}
	else if (m_overManual) text = m_helpMenu->action( KHelpMenu::menuHelpContents )->text().remove(QLatin1Char('&'));
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
				text = i18n("Press Start to begin");
			break;
			
			case blinkenGame::choosingLevel:
				text = i18n("Set the Difficulty Level...");
			break;
			
			case blinkenGame::waiting3:
				if (m_overCentralText)
				{
					text = restartText;
				}
				else
				{
					text = i18n("Next sequence in 3...");
				}
			break;
			
			case blinkenGame::waiting2:
				if (m_overCentralText)
				{
					text = restartText;
				}
				else
				{
					if (m_game.level() == 1)
					{
						text = i18n("Next sequence in 3, 2...");
					}
					else
					{
						text = i18n("Next sequence in 2...");
					}
				}
			break;
		
			case blinkenGame::waiting1:
				if (m_overCentralText)
				{
					text = restartText;
				}
				else
				{
					if (m_game.level() == 1)
					{
						text = i18n("Next sequence in 3, 2, 1...");
					}
					else
					{
						text = i18n("Next sequence in 2, 1...");
					}
				}
			break;
			
			case blinkenGame::learningTheSequence:
				if (m_overCentralText)
				{
					text = restartText;
				}
				else
				{
					text = i18n("Remember this sequence...");
				}
			break;
			
			case blinkenGame::typingTheSequence:
				if (m_overCentralText)
				{
					text = restartText;
				}
				else
				{
					text = i18n("Repeat the sequence");
				}
			break;
		}
	}
	
	QFont f;
	if (blinkenSettings::customFont() && !m_alwaysUseNonCoolFont) f = QFont(QStringLiteral("Steve"));
	p.setFont(f);
	f.setPointSize(KFontUtils::adaptFontSize(p, text, 380, 30, 28, 1, KFontUtils::DoNotAllowWordWrap));
	p.setFont(f);
	p.drawText(0, 0, text);
}

void blinken::drawLevel(QPainter &p)
{
	QString n[3];
	n[0] = i18n("2");
	n[1] = i18n("1");
	n[2] = i18n("?");
	
	double posX = (double)width() / 2.0;
	double posY = (double)height() / 1.868;
	drawText(p, i18nc("@label:chooser which level is currently being played", "Level"), QPointF(posX, posY), false, 0, 0, nullptr, false, true);
	
	QPointF cp;
	for (int i = 0; i < 3; i++)
	{
		double posX2 = (double)width() / 2.019;
		double posY2 = (double)height() / 1.677;
		double xMargin = (double)width() / 32.2;
		double yMargin = (double)height() / 105.0;
		if (i == 0) cp = QPointF(posX2, posY2);
		else if (i == 1) cp = QPointF(m_levelsRect[0].left() - m_levelsRect[0].width(), posY2);
		else if (i == 2) cp = QPointF(m_levelsRect[0].right() + m_levelsRect[0].width(), posY2);
		drawText(p, n[i], cp, true, xMargin, yMargin, &(m_levelsRect[i]), m_overLevels[i], true);
	}
}

void blinken::drawText(QPainter &p, const QString &text, const QPointF &center, bool withMargin, double xMargin, double yMargin, QRectF *rect, bool highlight, bool bold)
{
	QRectF r;
	QFont oldFont, f = p.font();
	oldFont = f;
	double aux1 = (double)width() / 3.389;
	double aux2 = (double)height() / 17.5;
	f.setPointSize(KFontUtils::adaptFontSize(p, text, qRound(aux1), qRound(aux2), 28, 1, KFontUtils::DoNotAllowWordWrap));
	if (bold) f.setBold(true);
	p.setFont(f);
	
	r = p.boundingRect(QRectF(), Qt::AlignLeft, text);
	r = QRectF(0, 0, r.width() + xMargin, r.height() + yMargin);
	r.translate(center.x() - r.width() / 2, center.y() - r.height() / 2);
	
	if (withMargin)
	{
		p.fillRect(r, m_fillColor);
		p.setPen(QPen(Qt::black, 3));
		p.drawRoundedRect(r, 15, 15, Qt::RelativeSize);
	}
	
	if (highlight)
	{
		p.setPen(m_fontHighlightColor);
	}
	else
	{
		p.setPen(m_fontColor);
	}
	p.drawText(r, Qt::AlignCenter, text);
	
	if (rect) *rect = p.worldTransform().mapRect(r);
	
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
			m_overSettings = false;
			m_overManual = false;
			haveToUpdate = true;
		}
		else if (m_overAboutKDE || m_overAboutBlinken || m_overManual || m_overSettings)
		{
			m_overAboutKDE = false;
			m_overAboutBlinken = false;
			m_overSettings = false;
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
				m_overSettings = false;
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
				m_overSettings = false;
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
				m_overSettings = false;
				m_overManual = true;
				haveToUpdate = true;
			}
		}
		else if (m_settingsRect.contains(p))
		{
			if (!m_overSettings)
			{
				m_overAboutKDE = false;
				m_overAboutBlinken = false;
				m_overSettings = true;
				m_overManual = false;
				haveToUpdate = true;
			}
		}
		else
		{
			m_overMenu = false;
			m_overAboutKDE = false;
			m_overAboutBlinken = false;
			m_overSettings = false;
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
	QPointF p2 = p - QPointF((double)width() / centerX, (double)height() / centerY);
	
	if (m_overHighscore || m_overQuit || m_overCentralText || m_overMenu || m_overAboutKDE || m_overAboutBlinken || m_overManual || m_overSettings || m_overLevels[0] || m_overLevels[1] || m_overLevels[2] || m_overCentralLetters || m_overCounter || (m_game.canType() && (insideGreen(p2) || insideRed(p2) || insideBlue(p2) || insideYellow(p2))) || m_overFont || m_overSound) setCursor(Qt::PointingHandCursor);
	else setCursor(Qt::ArrowCursor);
}

QPixmap blinken::getPixmap(const QString &element, const QSize &imageSize)
{
	QMap<QString, QPixmap>::Iterator it = m_pixmapCache.find(element);
	if (it == m_pixmapCache.end())
	{
		QPixmap pix(imageSize);
		pix.fill(Qt::transparent);
		QPainter p(&pix);
		m_renderer->render(&p, element);
		it = m_pixmapCache.insert(element, pix);
	}
	return it.value();
}

