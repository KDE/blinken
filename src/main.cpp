/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include "ksimon.h"

int main(int argc, char *argv[])
{
	KAboutData about("ksimon", I18N_NOOP("KSimon"), "0.1", I18N_NOOP("A memory enhancement game"), KAboutData::License_GPL);
	about.addAuthor("Albert Astals Cid", I18N_NOOP("Coding"), "tsdgeos@terra.es");
	about.addAuthor("Danny Allen", I18N_NOOP("Design, Graphics and Sounds"), "danny@dannyallen.co.uk");
	KCmdLineArgs::init(argc, argv, &about);
	KApplication app;
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	
	app.setTopWidget(new KSimon());
	return app.exec();
}
