
########### next target ###############

set(blinken_SRCS 
   main.cpp 
   blinken.cpp 
   soundsplayer.cpp 
   blinkengame.cpp 
   number.cpp 
   highscoredialog.cpp 
   counter.cpp 
   button.cpp )


kde4_add_kcfg_files(blinken_SRCS settings.kcfgc )

#uncomment this if oxygen icons for blinken are available
#kde4_add_app_icon(blinken_SRCS "${CMAKE_INSTALL_PREFIX}/share/icons/oxygen/*/apps/blinken.png")
kde4_add_app_icon(blinken_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-blinken.png")

kde4_add_executable(blinken ${blinken_SRCS})

target_link_libraries(blinken  ${KDE4_KIO_LIBS} ${KDE4_PHONON_LIBS})

install(TARGETS blinken  ${INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############

install( PROGRAMS blinken.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES blinken.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )

