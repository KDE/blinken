########### install files ###############
#
if(NOT ANDROID)
kdoctools_create_handbook(index.docbook INSTALL_DESTINATION ${KDE_INSTALL_DOCBUNDLEDIR}/en SUBDIR blinken)
endif()
