set(SOUND_FILES
    1.wav
    2.wav
    3.wav
    4.wav
    lose.wav
)

qt_add_resources(blinken "sounds"
    PREFIX "/"
    FILES
        ${SOUND_FILES}
)
