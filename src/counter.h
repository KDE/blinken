/*
    SPDX-FileCopyrightText: 2005-2006 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
