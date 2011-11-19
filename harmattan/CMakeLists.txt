install( FILES qml/main.qml  DESTINATION  ${DATA_INSTALL_DIR}/blinken/qml )
install( FILES blinken.sh  DESTINATION  ${DATA_INSTALL_DIR}/blinken/bin )
install( FILES blinken_harmattan.desktop  DESTINATION  ${CMAKE_INSTALL_PREFIX}/usr/share/applications )
install( FILES blinken80.png  DESTINATION  ${CMAKE_INSTALL_PREFIX}/usr/share/icons/hicolor/80x80/apps )
