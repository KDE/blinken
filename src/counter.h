/***************************************************************************
 *   Copyright (C) 2005-2006 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef COUNTER_H
#define COUNTER_H

class QPainter;
class QSvgRenderer;
class QColor;
class counter
{
	public:
		static void paint(QPainter &p, bool drawScore, int score, bool drawTimer, const QColor &c1, const QColor &c2, const QColor &c3, QSvgRenderer *renderer);
		static int width(bool drawTimer);
		static int height();
};

#endif
