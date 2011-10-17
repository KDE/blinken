/***************************************************************************
 *   Copyright (C) 2005-2007 by Albert Astals Cid <aacid@kde.org>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "blinken.h"

#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kapplication.h>

#include <QFontDatabase>
#include <QFontInfo>

int main(int argc, char *argv[])
{
	KAboutData about("blinken", 0, ki18n("Blinken"), "0.2", ki18n("A memory enhancement game"), KAboutData::License_GPL, ki18n("© 2005-2007 Albert Astals Cid\n© 2005-2007 Danny Allen"));
	about.addAuthor(ki18n("Albert Astals Cid"), ki18n("Coding"), "tsdgeos@terra.es");
	about.addAuthor(ki18n("Danny Allen"), ki18n("Design, Graphics and Sounds"), "danny@dannyallen.co.uk");
	about.addCredit(ki18n("Steve Jordi"), ki18n("GPL'ed his 'Steve' font so that we could use it"), "steve@sjordi.com");
	KCmdLineArgs::init(argc, argv, &about);
	KApplication app;
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

	QFont f("Steve", 12, QFont::Normal, true);
	// Works with Steve may need some tweaking to work with other fonts
	if (!QFontInfo(f).exactMatch())
	{
		QFontDatabase::addApplicationFont(KStandardDirs::locate("appdata", "fonts/steve.ttf"));
	}

	app.setTopWidget(new blinken());
	return app.exec();
}
