/***************************************************************************
 *   Copyright (C) 2005-2006 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "number.h"

#include <QPainter>
#include <QSvgRenderer>

number::number(int n)
{
	m_number = n;
}

void number::paint(QPainter &p, int digits, QSvgRenderer *renderer) const
{
	int nDigits, digit, number;
	nDigits = 0;
	number = m_number;
	
	while(nDigits < digits || number != 0)
	{
		digit = number % 10;
		number /= 10;
		paintDigit(p, digit, renderer);
		nDigits++;
		p.translate(-30, 0);
	}
	p.translate(30 * nDigits, 0);
}

void number::paintDigit(QPainter &p, int number, QSvgRenderer *renderer) const
{
	// make gcc  happy
	QString n = QStringLiteral("zero");
	
	switch (number)
	{
		case 1:
			n = QStringLiteral("one");
		break;
		
		case 2:
			n = QStringLiteral("two");
		break;
		
		case 3:
			n = QStringLiteral("three");
		break;
		
		case 4:
			n = QStringLiteral("four");
		break;
		
		case 5:
			n = QStringLiteral("five");
		break;
		
		case 6:
			n = QStringLiteral("six");
		break;
		
		case 7:
			n = QStringLiteral("seven");
		break;
		
		case 8:
			n = QStringLiteral("eight");
		break;
		
		case 9:
			n = QStringLiteral("nine");
		break;
	}
	
	renderer->render(&p, n, QRect(0, 0, 24, 30));
}
