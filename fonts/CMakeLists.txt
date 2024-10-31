if(QML_VERSION)
    qt_add_resources(blinken "fonts"
        PREFIX "/"
        FILES
            steve.ttf
    )
else()
    install( FILES steve.ttf  DESTINATION  ${KDE_INSTALL_DATADIR}/blinken/fonts )
endif()
