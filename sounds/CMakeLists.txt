set(SOUND_FILES
    1.wav
    2.wav
    3.wav
    4.wav
    lose.wav
)

if(QML_VERSION)
    qt_add_resources(blinken "sounds"
        PREFIX "/"
        FILES
            ${SOUND_FILES}
    )
else()
    install( FILES  ${SOUND_FILES} DESTINATION  ${KDE_INSTALL_DATADIR}/blinken/sounds )
endif()
