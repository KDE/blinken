/***************************************************************************
 *   Copyright (C) 2005-2006 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "counter.h"

#include <QColor>
#include <QPainter>

#include "number.h"

void counter::paint(QPainter &p, bool drawScore, int score, bool drawTimer, const QColor &c1, const QColor &c2, const QColor &c3, QSvgRenderer *renderer)
{
	p.save();
	p.translate(45, 15);
	p.setPen(QPen(Qt::black, 3));
	p.setBrush(QColor(40, 40, 40));
	if (drawTimer)
	{
		p.fillRect(-44, -13, 98, 48, p.brush());
		p.drawRoundedRect(-45, -15, 100, 50, 15, 15, Qt::RelativeSize);
	}
	else
	{
		p.fillRect(-44, -13, 70, 48, p.brush());
		p.drawRoundedRect(-45, -15, 73, 50, 15, 15, Qt::RelativeSize);
	}
	
	if (drawTimer)
	{
		p.fillRect(35, -6, 11, 9, c1);
		p.fillRect(35, 6, 11, 9, c2);
		p.fillRect(35, 18, 11, 9, c3);
	}
	
	if (drawScore)
	{
		p.translate(-5, -5);
		number n(score);
		n.paint(p, 2, renderer);
	}
	
	p.restore();
}

int counter::width(bool drawTimer)
{
	if (drawTimer) return 100;
	else return 73;
}

int counter::height()
{
	return 50;
}
