kde4_header()

add_subdirectory( src ) 
add_subdirectory( images ) 
add_subdirectory( icons ) 
add_subdirectory( sounds ) 
add_subdirectory( fonts ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/blinken/ FILES README.packagers )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = src images icons sounds fonts
#
#docsdir = $(kde_datadir)/blinken/
#docs_DATA = README.packagers
