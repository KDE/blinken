/*
    SPDX-FileCopyrightText: 2005-2007 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "blinken.h"

#include "blinken_version.h"

#include <KAboutData>
#include <KLocalizedString>
#include <KCrash>
#include <KDBusService>
#include <Kdelibs4ConfigMigrator>

#include <QApplication>
#include <QCommandLineParser>

#include <QFontDatabase>
#include <QFontInfo>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
	KLocalizedString::setApplicationDomain("blinken");

	QApplication app(argc, argv);
	KAboutData about(QStringLiteral("blinken"), i18n("Blinken"), QStringLiteral(BLINKEN_VERSION_STRING), i18n("A memory enhancement game"), KAboutLicense::GPL, i18n("© 2005-2007 Albert Astals Cid\nSPDX-FileCopyrightText: 2005-2007 Danny Allen "));
	about.addAuthor(i18n("Albert Astals Cid"), i18n("Coding"), QStringLiteral("aacid@kde.org"));
	about.addAuthor(i18n("Danny Allen"), i18n("Design, Graphics and Sounds"), QStringLiteral("danny@dannyallen.co.uk"));
	about.addCredit(i18n("Steve Jordi"), i18n("GPL'ed his 'Steve' font so that we could use it"), QStringLiteral("steve@sjordi.com"));

	KAboutData::setApplicationData(about);

	KCrash::initialize();

	Kdelibs4ConfigMigrator migrate(QStringLiteral("blinken"));
	migrate.setConfigFiles(QStringList() << QStringLiteral("blinkenrc"));
	migrate.migrate();


	QCommandLineParser parser;
	about.setupCommandLine(&parser);
	parser.process(app);
	about.processCommandLine(&parser);

	app.setWindowIcon(QIcon::fromTheme(QStringLiteral("blinken")));

 	QFont f(QStringLiteral("Steve"), 12, QFont::Normal, true);
	// Works with Steve may need some tweaking to work with other fonts
	if (!QFontInfo(f).exactMatch())
	{
		QFontDatabase::addApplicationFont(QStandardPaths::locate(QStandardPaths::DataLocation, QStringLiteral("fonts/steve.ttf")));
	}
	KDBusService service;
	new blinken();
	return app.exec();
}
