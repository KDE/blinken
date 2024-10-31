if(QML_VERSION)
    set(IMAGES 
        ui/background.svg 
        ui/exit_highlight.svg 
        ui/exit.svg 
        ui/highScore_highlight.svg
        ui/highScore.svg
        ui/menu_list.svg
        ui/menu.svg
        ui/rightArrow.svg
        ui/downArrow.svg

        numbers/0.svg
        numbers/1.svg
        numbers/2.svg
        numbers/3.svg
        numbers/4.svg
        numbers/5.svg
        numbers/6.svg
        numbers/7.svg
        numbers/8.svg
        numbers/9.svg
        numbers/blackBlock.svg
        numbers/redBlock.svg
        numbers/blank.svg
        numbers/full.svg

        color/blue_highlight.svg
        color/blue.svg
        color/green_highlight.svg
        color/green.svg
        color/red_highlight.svg
        color/red.svg
        color/yellow_highlight.svg
        color/yellow.svg
    )

    qt_add_resources(blinken "images"
        PREFIX "/"
        FILES
            ${IMAGES}
    )
else()
    install( FILES blinken.svg DESTINATION  ${KDE_INSTALL_DATADIR}/blinken/images )
endif()
