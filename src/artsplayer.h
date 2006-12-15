/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ARTSPLAYER_H
#define ARTSPLAYER_H

#include <phonon/mediaobject.h>

#include <QList>
#include <QTimer>

#include "blinkengame.h"

class artsPlayer : public QObject
{
Q_OBJECT
	public:
		artsPlayer();
		~artsPlayer();
		
		void play(blinkenGame::color c);
		
	signals:
		void ended();
		
	private slots:
		void playEnded();
		
	private:
		Phonon::MediaObject m_greenSound, m_redSound, m_blueSound, m_yellowSound, m_allSound;
		Phonon::MediaObject *m_currentSound;
		
		QTimer m_warnTimer;
};

#endif
