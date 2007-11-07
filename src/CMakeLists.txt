
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
if(MSVC)
  set(blinken_SRCS ${blinken_SRCS} blinken.rc)
endif(MSVC)

if(MINGW)
   KDE_ADD_RESOURCES2(blinken_SRCS blinken.rc)
endif(MINGW)

kde4_add_executable(blinken ${blinken_SRCS})

target_link_libraries(blinken  ${KDE4_KIO_LIBS} ${KDE4_PHONON_LIBS} kdeeduui )

install(TARGETS blinken  DESTINATION ${BIN_INSTALL_DIR} )

########### install files ###############

install( FILES blinken.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES blinken.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )

