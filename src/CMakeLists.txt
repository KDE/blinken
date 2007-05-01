
########### next target ###############

set(blinken_SRCS 
   main.cpp 
   blinken.cpp 
   soundsplayer.cpp 
   blinkengame.cpp 
   number.cpp 
   highscoredialog.cpp 
   counter.cpp 
   fontutils.cpp 
   fontchecker.cpp 
   button.cpp )

kde4_automoc(${blinken_SRCS})

kde4_add_kcfg_files(blinken_SRCS settings.kcfgc )

kde4_add_executable(blinken ${blinken_SRCS})

target_link_libraries(blinken  ${KDE4_KIO_LIBS} ${KDE4_PHONON_LIBS} )

install(TARGETS blinken  DESTINATION ${BIN_INSTALL_DIR} )

########### install files ###############

install( FILES blinken.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES blinken.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )

