/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef KSIMON_H
#define KSIMON_H

#include <qwidget.h>

#include <arts/kartsdispatcher.h>
#include <arts/kartsserver.h>
#include <arts/kplayobject.h>
#include <arts/kplayobjectfactory.h>

class KSimon : public QWidget
{
	public:
		KSimon();
		~KSimon();
		
	protected:
		void paintEvent(QPaintEvent *);
		void mouseMoveEvent(QMouseEvent *e);
		void mousePressEvent(QMouseEvent *e);
	
	private:
		void drawHelp(QPainter &p);
		void drawQuit(QPainter &p);
		void drawStart(QPainter &p);
		void drawText(QPainter &p);
		void drawLevel(QPainter &p);
		void putFont(QPainter &p, const QString &s1, const QString &s2, int w, int h);
		
		QPixmap *m_back;
		bool m_overHelp, m_overQuit, m_overStart;
		QRect m_helpRect, m_quitRect, m_startRect;
		// 0 Press Start
		// 1 Choose Level
		int m_gamePhase;
		
		KArtsDispatcher *m_dispatcher;
		KArtsServer *m_server;
		KDE::PlayObjectFactory *m_factory;
		KDE::PlayObject *m_playobj;
};

#endif
