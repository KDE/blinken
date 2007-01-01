/***************************************************************************
 *   Copyright (C) 2005-2006 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef NUMBER_H
#define NUMBER_H

class QPainter;
class QSvgRenderer;

class number
{
	public:
		explicit number(int n);
		
		void paint(QPainter &p, int digits, QSvgRenderer *renderer) const;
	
	private:
		void paintDigit(QPainter &p, int n, QSvgRenderer *renderer) const;
		
		int m_number;
};

#endif
