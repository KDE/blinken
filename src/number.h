/*
    SPDX-FileCopyrightText: 2005-2006 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
