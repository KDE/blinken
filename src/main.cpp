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
#include <klocalizedstring.h>
#include <qapplication.h>
#include <qcommandlineparser.h>

#include <QFontDatabase>
#include <QFontInfo>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
	KLocalizedString::setApplicationDomain("blinken");

	QApplication app(argc, argv);
	KAboutData about("blinken", i18n("Blinken"), "0.3", i18n("A memory enhancement game"), KAboutLicense::GPL, i18n("© 2005-2007 Albert Astals Cid\n© 2005-2007 Danny Allen"));
	about.addAuthor(i18n("Albert Astals Cid"), i18n("Coding"), "aacid@kde.org");
	about.addAuthor(i18n("Danny Allen"), i18n("Design, Graphics and Sounds"), "danny@dannyallen.co.uk");
	about.addCredit(i18n("Steve Jordi"), i18n("GPL'ed his 'Steve' font so that we could use it"), "steve@sjordi.com");

	KAboutData::setApplicationData(about);

	QCommandLineParser parser;
	parser.addVersionOption();
	about.setupCommandLine(&parser);
	parser.process(app);
	about.processCommandLine(&parser);

	QFont f("Steve", 12, QFont::Normal, true);
	// Works with Steve may need some tweaking to work with other fonts
	if (!QFontInfo(f).exactMatch())
	{
		QFontDatabase::addApplicationFont(QStandardPaths::locate(QStandardPaths::DataLocation, "fonts/steve.ttf"));
	}

	new blinken();
	return app.exec();
}
