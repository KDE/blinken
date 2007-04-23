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
#include <kprocess.h>
#include <kstandarddirs.h>

#include "fontchecker.h"
#include "blinken.h"

int main(int argc, char *argv[])
{
	KAboutData about("blinken", I18N_NOOP("blinKen"), "0.1.1", I18N_NOOP("A memory enhancement game"), KAboutData::License_GPL, "© 2005 Albert Astals Cid\n© 2005 Danny Allen");
	about.addAuthor("Albert Astals Cid", I18N_NOOP("Coding"), "tsdgeos@terra.es");
	about.addAuthor("Danny Allen", I18N_NOOP("Design, Graphics and Sounds"), "danny@dannyallen.co.uk");
	about.addCredit("Steve Jordi", I18N_NOOP("GPL'ed his 'Steve' font so that we could use it"), "steve@sjordi.com");
	KCmdLineArgs::init(argc, argv, &about);
	KApplication app;
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	
	QFont f("Steve", 12, QFont::Normal, true);
	if (!fontChecker::checkInstalled(f, locate("appdata", "fonts/steve.ttf")))
	{
		KProcess *proc = new KProcess;
		for (int i = 0; i < argc; i++) *proc << argv[i];
		proc->start();
	}
	else
	{
		app.setTopWidget(new blinken());
		return app.exec();
	}
}
