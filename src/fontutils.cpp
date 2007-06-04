/***************************************************************************
 *   Copyright (C) 2005-2006 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "fontutils.h"

#include <qpainter.h>

int fontUtils::fontSize(QPainter &p, const QString &s1, int w, int h)
{
	int size;
	QRect aux1;
	bool done = false;
	
	size = 28;
	
	while (!done)
	{
		QFont f = p.font();
		f.setPointSize(size);
		p.setFont(f);
		aux1 = p.boundingRect(QRect(), Qt::AlignLeft, s1);
		if (aux1.width() == 0 || aux1.height() == 0) return -1;
		else if (aux1.width() > w || aux1.height() > h) size = qMin(w * size / aux1.width(), h * size / aux1.height());
		else done = true;
	}
	
	return size;
}
