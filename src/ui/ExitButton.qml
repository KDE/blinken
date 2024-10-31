/*
    SPDX-FileCopyrightText: 2024 Hanyang Zhang <hanyangzhang@qq.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick

Item {
    height: 768 //default size
    width: height

    Rectangle {
        id: rectangle
        color: "#00ffffff"
        border.color: "#00000000"
        anchors.fill: parent

        Image {
            id: image
            anchors.fill: parent
            source: "qrc:ui/exit.svg"
            fillMode: Image.Stretch
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onPressed: {
                image.source = "qrc:ui/exit_highlight.svg";
            }
            onReleased: {
                Qt.quit();
            }
        }
    }
}
