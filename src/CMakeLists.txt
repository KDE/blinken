
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


kconfig_add_kcfg_files(blinken_SRCS settings.kcfgc )

add_executable(blinken ${blinken_SRCS})

MESSAGE(${PHONON_LIBRARIES})
target_link_libraries(blinken
    KF5::I18n
    KF5::XmlGui
    Qt5::Svg
    ${KDE4_KDEUI_LIBS}
    ${PHONON_LIBRARIES}
    )

install(TARGETS blinken EXPORT blinken ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS blinken.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
install( FILES blinken.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )

