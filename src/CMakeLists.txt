
########### next target ###############

ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX BLINKEN VERSION_HEADER blinken_version.h)

set(blinken_SRCS 
    blinken.cpp
    blinkengame.cpp
    blinkengame.h
    blinken.h
    button.cpp
    button.h
    counter.cpp
    counter.h
    highscoredialog.cpp
    highscoredialog.h
    main.cpp
    number.cpp
    number.h
    soundsplayer.cpp
    soundsplayer.h )

kconfig_add_kcfg_files(blinken_SRCS settings.kcfgc )

file(GLOB ICON_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/*-apps-blinken.png")
ecm_add_app_icon(blinken_SRCS ICONS ${ICON_SRCS})
add_executable(blinken ${blinken_SRCS})

target_link_libraries(blinken
    KF6::CoreAddons
    KF6::I18n
    KF6::XmlGui
    KF6::GuiAddons
    Qt::Svg
    KF6::DBusAddons
    KF6::Crash
    Phonon::phonon4qt6
    )

install(TARGETS blinken EXPORT blinken ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS org.kde.blinken.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES blinken.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )

