kde4_header()




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/blinken/sounds FILES 1.wav 2.wav 3.wav 4.wav lose.wav )

kde4_footer()



#original Makefile.am contents follow:

#soundsdir = $(kde_datadir)/blinken/sounds
#sounds_DATA = 1.wav 2.wav 3.wav 4.wav lose.wav
