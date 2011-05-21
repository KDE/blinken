project(blinken)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

set(CMAKE_MODULE_PATH ${blinken_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

if(${CMAKE_SOURCE_DIR} STREQUAL ${blinken_SOURCE_DIR})
    find_package(LibKdeEdu REQUIRED)
    include_directories(${LIBKDEEDU_INCLUDE_DIR})
else()
    set(LIBKDEEDU_KDEEDUUI_LIBRARIES kdeeduui)
endif()

add_subdirectory( doc )
add_subdirectory( src ) 
add_subdirectory( images ) 
add_subdirectory( icons ) 
add_subdirectory( sounds ) 
add_subdirectory( fonts ) 

########### install files ###############

install( FILES README.packagers  DESTINATION  ${DATA_INSTALL_DIR}/blinken/ )

