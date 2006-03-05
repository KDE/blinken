kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/blinken/fonts FILES steve.ttf )

kde4_footer()



#original Makefile.am contents follow:

#fontsdir = $(kde_datadir)/blinken/fonts
#fonts_DATA = steve.ttf
