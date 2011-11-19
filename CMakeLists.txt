project(blinken)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

set(CMAKE_MODULE_PATH ${blinken_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

if (HARMATTAN)
  add_definitions(-DHARMATTAN_BUILD)
  include_directories (harmattan)
  add_subdirectory( harmattan )
  install( FILES COPYING  DESTINATION  ${DATA_INSTALL_DIR}/blinken/ ) 
else (HARMATTAN)
  add_subdirectory( doc )
  add_subdirectory( icons )
  install( FILES README.packagers  DESTINATION  ${DATA_INSTALL_DIR}/blinken/ )
endif (HARMATTAN)

add_subdirectory( src ) 
add_subdirectory( images )  
add_subdirectory( sounds ) 
add_subdirectory( fonts ) 
