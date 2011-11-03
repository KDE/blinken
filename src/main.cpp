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

#ifdef HARMATTAN_BUILD
#include "blinkenproxywidget.h"
#include "highscoredialog.h"
#include <QGraphicsProxyWidget>
#include <qdeclarative.h>
#include <QtDeclarative/QDeclarativeView>
#endif

int main(int argc, char *argv[])
{
	KAboutData about("blinken", 0, ki18n("Blinken"), "0.2", ki18n("A memory enhancement game"), KAboutData::License_GPL, ki18n("© 2005-2007 Albert Astals Cid\n© 2005-2007 Danny Allen"));
	about.addAuthor(ki18n("Albert Astals Cid"), ki18n("Coding"), "tsdgeos@terra.es");
	about.addAuthor(ki18n("Danny Allen"), ki18n("Design, Graphics and Sounds"), "danny@dannyallen.co.uk");
	about.addCredit(ki18n("Steve Jordi"), ki18n("GPL'ed his 'Steve' font so that we could use it"), "steve@sjordi.com");
	KCmdLineArgs::init(argc, argv, &about);
	KApplication app;
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

#ifdef HARMATTAN_BUILD
	KGlobal::dirs()->addResourceDir("appdata", "/opt/blinken/");
#endif

	QFont f("Steve", 12, QFont::Normal, true);
	// Works with Steve may need some tweaking to work with other fonts
	if (!QFontInfo(f).exactMatch())
	{
		QFontDatabase::addApplicationFont(KStandardDirs::locate("appdata", "fonts/steve.ttf"));
	}

#ifdef HARMATTAN_BUILD
	qmlRegisterType<BlinkenProxyWidget>("BlinkenLibrary", 1, 0, "BlinkenWidget");
	qmlRegisterType<highScoreManager>("BlinkenLibrary", 1, 0, "HighScoreManager");

	QScopedPointer<QDeclarativeView> view(new QDeclarativeView());
	view->setSource(QUrl::fromLocalFile("/opt/blinken/qml/main.qml"));
	view->showFullScreen();
#else
	app.setTopWidget(new blinken());
#endif

	return app.exec();
}
