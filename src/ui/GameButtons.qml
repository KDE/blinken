/*
    SPDX-FileCopyrightText: 2024 Hanyang Zhang <hanyangzhang@qq.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick
import org.kde.blinken

Item {
    id: gameButtons

    GameButton {
        id: redButton
        width: yellowButton.width
        height: yellowButton.height
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: yellowButton.anchors.topMargin
        anchors.rightMargin: gameButtons.width * 0.02929
        scale: gameButtons.scale

        normalImageSource: "qrc:color/red.svg"
        highLightImageSource: "qrc:color/red_highlight.svg"
        gameColor: BlinkenGame.Red
    }

    GameButton {
        id: yellowButton
        width: gameButtons.width * 0.4609
        height: gameButtons.height * 0.3593
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: gameButtons.width * 0.0332
        anchors.topMargin: gameButtons.height * 0.0338
        scale: gameButtons.scale

        normalImageSource: "qrc:color/yellow.svg"
        highLightImageSource: "qrc:color/yellow_highlight.svg"
        gameColor: BlinkenGame.Yellow
    }

    GameButton {
        id: blueButton
        width: yellowButton.width
        height: yellowButton.height
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: gameButtons.height * 0.2291
        anchors.leftMargin: yellowButton.anchors.leftMargin
        scale: gameButtons.scale

        normalImageSource: "qrc:color/blue.svg"
        highLightImageSource: "qrc:color/blue_highlight.svg"
        gameColor: BlinkenGame.Blue
    }

    GameButton {
        id: greenButton
        width: yellowButton.width
        height: yellowButton.height
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: redButton.anchors.rightMargin
        anchors.bottomMargin: blueButton.anchors.bottomMargin
        scale: gameButtons.scale

        normalImageSource: "qrc:color/green.svg"
        highLightImageSource: "qrc:color/green_highlight.svg"
        gameColor: BlinkenGame.Green
    }

    function highLight(aimColor) {
        switch (aimColor) {
        case (BlinkenGame.None):
            {
                redButton.isHighLight = false;
                greenButton.isHighLight = false;
                blueButton.isHighLight = false;
                yellowButton.isHighLight = false;
                break;
            }
        case (BlinkenGame.Red):
            {
                redButton.isHighLight = true;
                break;
            }
        case (BlinkenGame.Green):
            {
                greenButton.isHighLight = true;
                break;
            }
        case (BlinkenGame.Blue):
            {
                blueButton.isHighLight = true;
                break;
            }
        case (BlinkenGame.Yellow):
            {
                yellowButton.isHighLight = "true";
                break;
            }
        case (BlinkenGame.All):
            {
                redButton.isHighLight = true;
                greenButton.isHighLight = true;
                blueButton.isHighLight = true;
                yellowButton.isHighLight = true;
                break;
            }
        default:
            {
                // never happens
                break;
            }
        }
    }
}
