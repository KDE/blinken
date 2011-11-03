/***************************************************************************
 *   Copyright (C) 2011 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef BLINKENPROXYWIDGET_H
#define BLINKENPROXYWIDGET_H

#include "blinken.h"

#include <QGraphicsProxyWidget>

#ifdef HARMATTAN_BUILD
class BlinkenProxyWidget : public QGraphicsProxyWidget
{
Q_OBJECT
	public:
		BlinkenProxyWidget(QGraphicsItem* parent = 0);

		Q_INVOKABLE void setHighScore(const QString &name, int level, int score);

	signals:
		 void askForName(const QString &lastName, int level, int score);
		 void showHighScoreDialog(int level);
		 void showAbout();

private:
	blinken *widget;
};
#endif

#endif // BLINKENPROXYWIDGET_H
