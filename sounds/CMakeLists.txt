kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/blinken/sounds FILES 1.wav 2.wav 3.wav 4.wav lose.wav )

kde4_footer()



#original Makefile.am contents follow:

#soundsdir = $(kde_datadir)/blinken/sounds
#sounds_DATA = 1.wav 2.wav 3.wav 4.wav lose.wav
