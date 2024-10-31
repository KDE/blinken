/*
    SPDX-FileCopyrightText: 2024 Hanyang Zhang <hanyangzhang@qq.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick

Item {
    id: highScoreButton
    property alias mouseArea: mouseArea
    property alias highScoreButtonImage: image

    Rectangle {
        id: rectangle
        color: "#00ffffff"
        border.color: "#00000000"
        anchors.fill: parent
        scale: highScoreButton.scale

        Image {
            id: image
            anchors.fill: parent
            source: "qrc:ui/highScore.svg"
            scale: highScoreButton.scale
            fillMode: Image.PreserveAspectFit
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            scale: highScoreButton.scale
        }
    }
}
