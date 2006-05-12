



########### next target ###############

set(blinken_SRCS 
   main.cpp 
   blinken.cpp 
   artsplayer.cpp 
   blinkengame.cpp 
   number.cpp 
   highscoredialog.cpp 
   counter.cpp 
   fontutils.cpp 
   fontchecker.cpp 
   button.cpp )

kde4_automoc(${blinken_SRCS})

kde4_add_kcfg_files(blinken_SRCS settings.kcfgc )

kde4_add_executable(blinken ${blinken_SRCS})

target_link_libraries(blinken  ${KDE4_KDECORE_LIBS} kdeui kio )

install_targets(/bin blinken )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES blinken.desktop )
install_files( ${KCFG_INSTALL_DIR} FILES blinken.kcfg )




#original Makefile.am contents follow:

#if include_ARTS
#artslib = -lartskde
#else
#artslib =
#endif
#
#bin_PROGRAMS = blinken
#
#INCLUDES = $(all_includes)
#
#blinken_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#blinken_LDADD = $(LIB_KDEUI) $(LIB_KIO) $(artslib)
#
#blinken_SOURCES = main.cpp blinken.cpp artsplayer.cpp blinkengame.cpp number.cpp highscoredialog.cpp counter.cpp fontutils.cpp fontchecker.cpp button.cpp settings.kcfgc
#
#METASOURCES = AUTO
#
#xdg_apps_DATA = blinken.desktop
#
#kde_kcfg_DATA = blinken.kcfg
#
#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/blinken.pot
