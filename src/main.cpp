/*
    SPDX-FileCopyrightText: 2005-2007 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "blinken_version.h"

#include <KAboutData>
#include <KLocalizedString>

#include "blinkengame.h"
#include "highScoreManager.h"
#include "settings.h"

#ifdef QML_VERSION

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "maskedmousearea.h"

#else

#include <QApplication>
#include <QCommandLineParser>
#include <QFontDatabase>
#include <QFontInfo>
#include <QStandardPaths>

#include <KCrash>
#include <KDBusService>

#include "blinken.h"

#endif

Q_DECL_EXPORT
int main(int argc, char *argv[])
{

#ifdef QML_VERSION
	QGuiApplication app{argc, argv};
#else
	QApplication app{argc, argv};
#endif

	KLocalizedString::setApplicationDomain(QByteArrayLiteral("blinken"));

	KAboutData about(QStringLiteral("blinken"), i18n("Blinken"), QStringLiteral(BLINKEN_VERSION_STRING), i18n("A memory enhancement game"), KAboutLicense::GPL, i18n("© 2005-2007 Albert Astals Cid\nSPDX-FileCopyrightText: 2005-2007 Danny Allen "));
	about.addAuthor(i18n("Albert Astals Cid"), i18n("Coding"), QStringLiteral("aacid@kde.org"));
	about.addAuthor(i18n("Danny Allen"), i18n("Design, Graphics and Sounds"), QStringLiteral("danny@dannyallen.co.uk"));
	about.addCredit(i18n("Steve Jordi"), i18n("GPL'ed his 'Steve' font so that we could use it"), QStringLiteral("steve@sjordi.com"));
	KAboutData::setApplicationData(about);

#ifdef QML_VERSION
	QQmlApplicationEngine engine;

	QQmlContext *context = engine.rootContext();
	context->setContextObject(new KLocalizedContext{&engine});

	engine.loadFromModule("org.kde.blinken", "Blinken");

#else
	KCrash::initialize();

	QCommandLineParser parser;
	about.setupCommandLine(&parser);
	parser.process(app);
	about.processCommandLine(&parser);

	app.setWindowIcon(QIcon::fromTheme(QStringLiteral("blinken")));

	QFont f(QStringLiteral("Steve"), 12, QFont::Normal, true);
	// Works with Steve may need some tweaking to work with other fonts
	if (!QFontInfo(f).exactMatch())
	{
		QFontDatabase::addApplicationFont(QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, QStringLiteral("fonts/steve.ttf")));
	}
	KDBusService service;
	new blinken();
#endif
	
	return app.exec();
}
