
########### next target ###############

ecm_setup_version(${KDE_APPLICATIONS_VERSION} VARIABLE_PREFIX BLINKEN VERSION_HEADER blinken_version.h)

set(blinken_SRCS 
   main.cpp 
   blinken.cpp 
   soundsplayer.cpp 
   blinkengame.cpp 
   number.cpp 
   highscoredialog.cpp 
   counter.cpp 
   button.cpp )

include_directories(${PHONON_INCLUDES})

kconfig_add_kcfg_files(blinken_SRCS settings.kcfgc )

add_executable(blinken ${blinken_SRCS})

target_link_libraries(blinken
    KF5::I18n
    KF5::XmlGui
    KF5::GuiAddons
    Qt5::Svg
    KF5::DBusAddons
    KF5::Crash
    ${PHONON_LIBRARIES}
    )

install(TARGETS blinken EXPORT blinken ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS org.kde.blinken.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES blinken.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )

