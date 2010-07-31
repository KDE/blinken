/***************************************************************************
 *   Copyright (C) 2005 by Albert Astals Cid <tsdgeos@terra.es>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <tqfont.h>
#include <tqfontinfo.h>

#include <kio/netaccess.h>

#include "fontchecker.h"

bool fontChecker::checkInstalled(const TQFont &font, const TQString &fontPath)
{
	TQFontInfo fi(font);
	// Works with Steve may need some tweaking to work with other fonts
	if (!fi.exactMatch())
	{
		bool success = KIO::NetAccess::copy(fontPath, "fonts:/Personal/", 0);
		return !success;
	}
	return true;
}
