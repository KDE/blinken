/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qbrush.h>
#include <qcolor.h>
#include <qpainter.h>

#include "number.h"

const int number::m_number0[20] = {1, 0, 0, 2,
                                   0, 5, 5, 0,
                                   0, 5, 5, 0,
                                   0, 5, 5, 0,
                                   4, 0, 0, 3};

const int number::m_number1[20] = {5, 1, 0, 5,
                                   5, 5, 0, 5,
                                   5, 5, 0, 5,
                                   5, 5, 0, 5,
                                   5, 5, 0, 5};

const int number::m_number2[20] = {0, 0, 0, 2,
                                   5, 5, 5, 0,
                                   1, 0, 0, 3,
                                   0, 5, 5, 5,
                                   0, 0, 0, 0};

const int number::m_number3[20] = {0, 0, 0, 2,
                                   5, 5, 5, 0,
                                   5, 0, 0, 3,
                                   5, 5, 5, 2,
                                   0, 0, 0, 3};


const int number::m_number4[20] = {0, 5, 5, 0,
                                   0, 5, 5, 0,
                                   0, 5, 5, 0,
                                   4, 0, 0, 0,
                                   5, 5, 5, 0};

const int number::m_number5[20] = {0, 0, 0, 0,
                                   0, 5, 5, 5,
                                   4, 0, 0, 2,
                                   5, 5, 5, 0,
                                   0, 0, 0, 3};

const int number::m_number6[20] = {1, 0, 0, 0,
                                   0, 5, 5, 5,
                                   0, 0, 0, 2,
                                   0, 5, 5, 0,
                                   4, 0, 0, 3};

const int number::m_number7[20] = {0, 0, 0, 2,
                                   5, 5, 5, 0,
                                   5, 5, 5, 0,
                                   5, 5, 5, 0,
                                   5, 5, 5, 0};

const int number::m_number8[20] = {1, 0, 0, 2,
                                   0, 5, 5, 0,
                                   4, 0, 0, 3,
                                   1, 5, 5, 2,
                                   4, 0, 0, 3};

const int number::m_number9[20] = {1, 0, 0, 2,
                                   0, 5, 5, 0,
                                   4, 0, 0, 3,
                                   5, 5, 5, 0,
                                   0, 0, 0, 3};

number::number(int n)
{
	m_number = n;
}

void number::paint(QPainter &p, int digits) const
{
	int nDigits, digit, number;
	nDigits = 0;
	number = m_number;
	
	while(nDigits < digits || number != 0)
	{
		digit = number % 10;
		number /= 10;
		paintDigit(p, digit);
		nDigits++;
		p.translate(-30, 0);
	}
	p.translate(30 * nDigits, 0);
}

void number::paintDigit(QPainter &p, int number) const
{
	// make gcc  happy
	const int *n = m_number0;
	int shape;
	
	switch (number)
	{
		case 1:
			n = m_number1;
		break;
		
		case 2:
			n = m_number2;
		break;
		
		case 3:
			n = m_number3;
		break;
		
		case 4:
			n = m_number4;
		break;
		
		case 5:
			n = m_number5;
		break;
		
		case 6:
			n = m_number6;
		break;
		
		case 7:
			n = m_number7;
		break;
		
		case 8:
			n = m_number8;
		break;
		
		case 9:
			n = m_number9;
		break;
	}
	
	p.setBrush(Qt::red);
	p.setPen(Qt::red);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			shape = n[j + i * 4];
			if (shape == 0)
			{
				p.fillRect(7 * (j-1), 7 * (i-1), 6, 6, Qt::red);
			}
			else if (shape != 5)
			{
				if (shape == 1) p.translate(7 * (j-1), 7 * (i-1));
				else if (shape == 2)
				{
					p.translate(7 * j - 2, 7 * (i-1));
					p.rotate(90);
				}
				else if (shape == 3)
				{
					p.translate(7 * j - 2, 7 * i - 2);
					p.rotate(180);
				}
				else if (shape == 4)
				{
					p.translate(7 * (j-1), 7 * i - 2);
					p.rotate(270);
				}
				
				p.drawPie(0, 0, 11, 11, 90 * 16, 16 * 90);
				
				if (shape == 1) p.translate(-7 * (j-1), -7 * (i-1));
				else if (shape == 2)
				{
					p.rotate(-90);
					p.translate(-(7 * j - 2), -7 * (i-1));

				}
				else if (shape == 3)
				{
					p.rotate(-180);
					p.translate(-(7 * j - 2), -(7 * i - 2));
				}
				else if (shape == 4)
				{
					p.rotate(-270);
					p.translate(- (7 * (j-1)), -(7 * i - 2));
				}
			}
		}
	}
}
