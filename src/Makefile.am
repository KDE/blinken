bin_PROGRAMS = ksimon

INCLUDES = $(all_includes)

ksimon_LDFLAGS = $(all_libraries) $(KDE_RPATH)
ksimon_LDADD = $(LIB_KDEUI)
ksimon_SOURCES = main.cpp ksimon.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/ksimon.pot
