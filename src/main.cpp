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
	KAboutData about("ksimon", I18N_NOOP("KSimon"), "0.1", 0, KAboutData::License_GPL);
	about.addAuthor( "Albert Astals Cid", 0, "tsdgeos@terra.es" );
	KCmdLineArgs::init(argc, argv, &about);
	KApplication app;
	
	app.setTopWidget(new KSimon());
	return app.exec();
}
