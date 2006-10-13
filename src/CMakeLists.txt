
########### next target ###############

remove_definitions(-DQT3_SUPPORT_WARNINGS)
remove_definitions(-DQT3_SUPPORT)

set(blinken_SRCS 
   main.cpp 
   blinken.cpp 
   artsplayer.cpp 
   blinkengame.cpp 
   number.cpp 
   highscoredialog.cpp 
   counter.cpp 
   fontutils.cpp 
   fontchecker.cpp 
   button.cpp )

kde4_automoc(blinken ${blinken_SRCS})

kde4_add_kcfg_files(blinken blinken_SRCS settings.kcfgc )

kde4_add_executable(blinken ${blinken_SRCS})

target_link_libraries(blinken  ${KDE4_KIO_LIBS} )

install(TARGETS blinken  DESTINATION ${BIN_INSTALL_DIR} )

########### install files ###############

install( FILES blinken.desktop  DESTINATION  ${XDG_APPS_DIR} )
install( FILES blinken.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )

