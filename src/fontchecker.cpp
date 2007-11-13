/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "fontchecker.h"

#include <qfont.h>
#include <qfontinfo.h>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include <kio/netaccess.h>

bool fontChecker::checkInstalled(const QFont &font, const QString &fontPath)
{
	QFontInfo fi(font);
	// Works with Steve may need some tweaking to work with other fonts
	if (!fi.exactMatch())
	{
#ifdef Q_OS_WIN
    int iAdded = AddFontResourceW((LPCWSTR)fontPath.utf16());
    return iAdded != 0;
#else
		bool success = KIO::NetAccess::file_copy(KUrl::fromPath(fontPath), KUrl("fonts:/Personal/"), (QWidget*)0);
		return !success;
#endif
	}
	return true;
}
