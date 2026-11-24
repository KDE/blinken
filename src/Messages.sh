#! /bin/sh
$EXTRACTRC *.kcfg >> rc.cpp
$XGETTEXT `find . -name '*.cpp' -o -name '*.qml'` -o $podir/blinken.pot
rm -f rc.cpp
