bin_PROGRAMS = ksimon

INCLUDES = $(all_includes)

ksimon_LDFLAGS = $(all_libraries) $(KDE_RPATH)
ksimon_LDADD = $(LIB_KDEUI) -lartskde

ksimon_SOURCES = main.cpp ksimon.cpp

METASOURCES = AUTO

xdg_apps_DATA = ksimon.desktop

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/ksimon.pot
