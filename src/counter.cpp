/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qcolor.h>
#include <qpainter.h>

#include "counter.h"
#include "number.h"

void counter::paint(QPainter &p, bool drawScore, int score, bool drawTimer, const QColor &c1, const QColor &c2, const QColor &c3)
{
	p.save();
	p.translate(45, 15);
	p.setPen(QPen(Qt::black, 3));
	p.setBrush(QColor(40, 40, 40));
	if (drawTimer)
	{
		p.fillRect(-44, -13, 98, 48, p.brush());
		p.drawRoundRect(-45, -15, 100, 50, 15, 15);
	}
	else
	{
		p.fillRect(-44, -13, 70, 48, p.brush());
		p.drawRoundRect(-45, -15, 73, 50, 15, 15);
	}
	
	if (drawScore)
	{
		number n(score);
		n.paint(p, 2);
	}
	
	if (drawTimer)
	{
		p.fillRect(35, -6, 11, 9, c1);
		p.fillRect(35, 6, 11, 9, c2);
		p.fillRect(35, 18, 11, 9, c3);
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
