/***************************************************************************
 *   Copyright (C) 2011 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "blinkenproxywidget.h"

#ifdef HARMATTAN_BUILD

BlinkenProxyWidget::BlinkenProxyWidget(QGraphicsItem* parent)
	 : QGraphicsProxyWidget(parent)
 {
	widget = new blinken();
	widget->setAttribute(Qt::WA_NoSystemBackground);
	setWidget(widget);
	connect(widget, SIGNAL(askForName(QString, int, int)), this, SIGNAL(askForName(QString, int, int)));
	connect(widget, SIGNAL(showHighScoreDialog(int)), this, SIGNAL(showHighScoreDialog(int)));
	connect(widget, SIGNAL(showAbout()), this, SIGNAL(showAbout()));
}

void BlinkenProxyWidget::setHighScore(const QString &name, int level, int score)
{
	widget->setHighScore(name, level, score);
}

#endif
