/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qfont.h>
#include <qfontinfo.h>

#include <kio/netaccess.h>

#include "fontchecker.h"

bool fontChecker::checkInstalled(const QString &fontName, const QString &fontPath)
{
	QFont f(fontName);
	QFontInfo fi(f);
	// Works with Steve may need some tweaking to work with other fonts
	if (!fi.exactMatch())
	{
		KIO::NetAccess::copy(fontPath, "fonts:/Personal/", 0);
		return false;
	}
	return true;
}
