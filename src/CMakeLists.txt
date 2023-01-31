
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

include_directories(${PHONON_INCLUDES})

kconfig_add_kcfg_files(blinken_SRCS settings.kcfgc )

file(GLOB ICON_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/*-apps-blinken.png")
ecm_add_app_icon(blinken_SRCS ICONS ${ICON_SRCS})
add_executable(blinken ${blinken_SRCS})

target_link_libraries(blinken
    KF${KF_MAJOR_VERSION}::CoreAddons
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::XmlGui
    KF${KF_MAJOR_VERSION}::GuiAddons
    Qt::Svg
    KF${KF_MAJOR_VERSION}::DBusAddons
    KF${KF_MAJOR_VERSION}::Crash
    ${PHONON_LIBRARIES}
    )

install(TARGETS blinken EXPORT blinken ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS org.kde.blinken.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES blinken.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )

