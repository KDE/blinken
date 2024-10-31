/*
    SPDX-FileCopyrightText: 2024 Hanyang Zhang <hanyangzhang@qq.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick
import org.kde.blinken

Item {
    id: gameButton
    required property string normalImageSource
    required property string highLightImageSource
    required property int gameColor
    default property bool isHighLight: false

    Image {
        id: image
        anchors.fill: parent
        scale: gameButton.scale
        source: gameButton.isHighLight ? gameButton.highLightImageSource : gameButton.normalImageSource
        fillMode: Image.Stretch

        MaskedMouseArea {
            id: mouseArea
            anchors.fill: parent
            alphaThreshold: 0.5
            maskSource: image.source
            scale: image.scale
            onPressedChanged: {
                if (mouseArea.released) {
                    gameButton.isHighLight = false;
                }
                if (!BlinkenGame.canType())
                    return;
                if (mouseArea.pressed) {
                    gameButton.isHighLight = true;
                    BlinkenGame.clicked(gameColor);
                }
            }
        }
    }
}
