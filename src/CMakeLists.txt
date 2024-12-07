if(QML_VERSION)
    ecm_setup_version(${RELEASE_SERVICE_VERSION} VARIABLE_PREFIX BLINKEN VERSION_HEADER blinken_version.h)
    set(BLINKEN_SRCS
        main.cpp
        maskedmousearea.cpp maskedmousearea.h
        blinkengame.cpp
        soundsplayer.cpp
        highScoreManager.cpp
        settings.cpp
    )
    kconfig_add_kcfg_files(BLINKEN_SRCS settings.kcfgc 
        GENERATE_MOC
    )
    set(BLINKEN_QML_UI_SRCS 
        ui/Blinken.qml
        ui/BlinkenMenu.qml
        ui/ExitButton.qml
        ui/HighScoreButton.qml
        ui/ScoreAndCounter.qml
        ui/Numbers.qml
        ui/ScoreList.qml
        ui/HighScoreLists.qml
        ui/GameOptions.qml
        ui/GameButtons.qml
        ui/GameButton.qml
    )

    add_executable(blinken
        ${BLINKEN_SRCS}
    )
    target_compile_definitions(blinken PRIVATE QML_VERSION)

    ecm_add_qml_module(blinken
        URI "org.kde.blinken"
        VERSION 1.0
    )
    ecm_target_qml_sources(blinken SOURCES
        ${BLINKEN_QML_UI_SRCS}
        VERSION 1.0
    )
    if(ANDROID)
        include(ECMAddAndroidApk)
        ecm_add_android_apk(blinken ANDROID_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)
    endif()
    
    target_link_libraries(blinken PRIVATE
        Qt6::Quick
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Multimedia
        
        KF6::CoreAddons
        KF6::I18n
        KF6::GuiAddons
        KF6::Kirigami
        KF6::ConfigCore
        KF6::ConfigWidgets 
    )

else()
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
        highScoreManager.cpp
        highScoreManager.h
        main.cpp
        number.cpp
        number.h
        soundsplayer.cpp
        soundsplayer.h )

    kconfig_add_kcfg_files(blinken_SRCS settings.kcfgc GENERATE_MOC)

    file(GLOB ICON_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/*-apps-blinken.png")
    ecm_add_app_icon(blinken_SRCS ICONS ${ICON_SRCS})
    add_executable(blinken ${blinken_SRCS})

    target_link_libraries(blinken
        KF6::CoreAddons
        KF6::I18n
        KF6::XmlGui
        KF6::GuiAddons
        Qt::Svg
        Qt6::Qml
        KF6::DBusAddons
        KF6::Crash
        Phonon::phonon4qt6
    )

endif()

install(TARGETS blinken EXPORT blinken ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

########### install files ###############
install( PROGRAMS org.kde.blinken.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES blinken.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )
