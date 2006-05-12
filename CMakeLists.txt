project(blinken)

add_subdirectory( src ) 
add_subdirectory( images ) 
add_subdirectory( icons ) 
add_subdirectory( sounds ) 
add_subdirectory( fonts ) 




########### install files ###############

install_files( ${DATA_INSTALL_DIR}/blinken/ FILES README.packagers )




#original Makefile.am contents follow:

#SUBDIRS = src images icons sounds fonts
#
#docsdir = $(kde_datadir)/blinken/
#docs_DATA = README.packagers
