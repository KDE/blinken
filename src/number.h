/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef NUMBER_H
#define NUMBER_H

class QPainter;

class number
{
	public:
		number(int n);
		
		void paint(QPainter &p, int digits) const;
	
	private:
		void paintDigit(QPainter &p, int n) const;
		
		int m_number;
		
		static const int m_number0[20];
		static const int m_number1[20];
		static const int m_number2[20];
		static const int m_number3[20];
		static const int m_number4[20];
		static const int m_number5[20];
		static const int m_number6[20];
		static const int m_number7[20];
		static const int m_number8[20];
		static const int m_number9[20];
};

#endif
