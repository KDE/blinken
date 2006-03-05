kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/blinken/images FILES blinken.png redh.png blueh.png greenh.png yellowh.png quit.png quit_hover.png menu.png menu_hover.png highscore.png highscore_hover.png mark.png )

kde4_footer()



#original Makefile.am contents follow:

#imagesdir = $(kde_datadir)/blinken/images
#images_DATA = blinken.png redh.png blueh.png greenh.png yellowh.png quit.png quit_hover.png menu.png menu_hover.png highscore.png highscore_hover.png mark.png
