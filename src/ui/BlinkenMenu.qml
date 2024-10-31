/*
    SPDX-FileCopyrightText: 2024 Hanyang Zhang <hanyangzhang@qq.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick

Item {
    id: blinkenMenu
    width: 150 //default
    height: width
    signal openAboutBlinkenPage
    signal openAboutKDEPage
    signal openSettingsPage
    signal openHandBookPage

    Image {
        id: menuImage
        anchors.fill: parent
        source: "qrc:ui/menu.svg"
        scale: blinkenMenu.scale
        fillMode: Image.Stretch
    }

    MouseArea {
        id: questionMouseArea

        width: blinkenMenu.width * (60 / 150)
        height: questionMouseArea.width

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        scale: blinkenMenu.scale
        anchors.bottomMargin: 0
        anchors.rightMargin: 0

        onClicked: {
            if (menuState.state == "fold") {
                menuState.state = "unfold";
            } else {
                menuState.state = "fold";
            }
        }
    }

    MouseArea {
        id: aboutKDE
        width: blinkenMenu.width * 0.3
        height: blinkenMenu.height * 0.3
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        onClicked: {
            blinkenMenu.openAboutKDEPage();
        }
    }

    MouseArea {
        id: aboutBlinken

        width: blinkenMenu.width * 0.3
        height: blinkenMenu.height * 0.3

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: aboutKDE.width
        onClicked: {
            blinkenMenu.openAboutBlinkenPage();
        }
    }

    MouseArea {
        id: settings

        width: blinkenMenu.width * 0.3
        height: blinkenMenu.height * 0.3

        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: questionMouseArea.height
        onClicked: {
            blinkenMenu.openSettingsPage();
        }
    }

    MouseArea {
        id: handBook

        width: blinkenMenu.width * 0.3
        height: blinkenMenu.height * 0.3

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 0
        anchors.topMargin: 0
        onClicked: {
            blinkenMenu.openHandBookPage();
        }
    }

    StateGroup {
        id: menuState
        state: "fold"
        states: [
            State {
                name: "fold"
                PropertyChanges {
                    menuImage.source: "qrc:ui/menu.svg"
                    aboutKDE.enabled: false
                    aboutBlinken.enabled: false
                    settings.enabled: false
                    handBook.enabled: false
                }
            },
            State {
                name: "unfold"
                PropertyChanges {
                    menuImage.source: "qrc:ui/menu_list.svg"
                    aboutKDE.enabled: true
                    aboutBlinken.enabled: true
                    settings.enabled: true
                    handBook.enabled: true
                }
            }
        ]
    }
}
