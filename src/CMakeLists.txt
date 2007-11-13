
########### next target ###############

set(blinken_SRCS 
   main.cpp 
   blinken.cpp 
   soundsplayer.cpp 
   blinkengame.cpp 
   number.cpp 
   highscoredialog.cpp 
   counter.cpp 
   fontchecker.cpp 
   button.cpp )


kde4_add_kcfg_files(blinken_SRCS settings.kcfgc )
#uncomment this if oxygen icons for blinken are available
#set(blinken_icons
#    ${CMAKE_INSTALL_PREFIX}/share/icons/oxygen/16x16/apps/blinken.png
#    ${CMAKE_INSTALL_PREFIX}/share/icons/oxygen/32x32/apps/blinken.png
#    ${CMAKE_INSTALL_PREFIX}/share/icons/oxygen/48x48/apps/blinken.png
#    ${CMAKE_INSTALL_PREFIX}/share/icons/oxygen/64x64/apps/blinken.png
#    ${CMAKE_INSTALL_PREFIX}/share/icons/oxygen/128x128/apps/blinken.png
#)
set(blinken_icons
    ${CMAKE_CURRENT_SOURCE_DIR}/blinken/icons/hi16-app-blinken.png
    ${CMAKE_CURRENT_SOURCE_DIR}/blinken/icons/hi32-app-blinken.png
    ${CMAKE_CURRENT_SOURCE_DIR}/blinken/icons/hi48-app-blinken.png
    ${CMAKE_CURRENT_SOURCE_DIR}/blinken/icons/hi64-app-blinken.png
    ${CMAKE_CURRENT_SOURCE_DIR}/blinken/icons/hi128-app-blinken.png
)
kde4_add_win32_app_icon(blinken ${blinken_icons})

kde4_add_executable(blinken ${blinken_SRCS})

target_link_libraries(blinken  ${KDE4_KIO_LIBS} ${KDE4_PHONON_LIBS} kdeeduui )

install(TARGETS blinken  DESTINATION ${BIN_INSTALL_DIR} )

########### install files ###############

install( FILES blinken.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES blinken.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )

