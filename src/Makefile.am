if include_ARTS
artslib = -lartskde
else
artslib =
endif

bin_PROGRAMS = blinken

INCLUDES = $(all_includes)

blinken_LDFLAGS = $(all_libraries) $(KDE_RPATH)
blinken_LDADD = $(LIB_KDEUI) $(LIB_KIO) $(artslib)

blinken_SOURCES = main.cpp blinken.cpp artsplayer.cpp blinkengame.cpp number.cpp highscoredialog.cpp counter.cpp fontutils.cpp fontchecker.cpp button.cpp settings.kcfgc

METASOURCES = AUTO

xdg_apps_DATA = blinken.desktop

kde_kcfg_DATA = blinken.kcfg

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/blinken.pot
