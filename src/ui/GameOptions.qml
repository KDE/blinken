/*
    SPDX-FileCopyrightText: 2024 Hanyang Zhang <hanyangzhang@qq.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick
import org.kde.blinken

Item {
    id: gameOptions
    property alias startButton: startButton
    property alias optionsList: optionsList
    property alias blinkenState: blinkenState

    Rectangle {
        id: startButton
        color: "#282828"
        border.color: "black"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        border.width: 5
        radius: 15

        Text {
            id: text1
            color: "#ffffff"
            text: i18nc("@action:button Start a new game", "Start")
            anchors.fill: parent
            font.pixelSize: 22
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            id: startMouseArea
            anchors.fill: parent
            onClicked: {
                if (blinkenState.state === "default") {
                    blinkenState.state = "selectLevelState";
                }
                BlinkenGame.setPhase(BlinkenGame.ChoosingLevel);
            }
        }
    }

    Row {
        id: optionsList
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        visible: false

        Rectangle {
            id: plane
            color: "#282828"
            border.color: "black"

            width: optionsList.width * (43 / 220)
            height: optionsList.height * (48 / 70)
            radius: 8
            border.width: 4

            Text {
                id: text2
                color: "#ffffff"
                text: i18n("1")
                anchors.fill: parent
                font.pixelSize: 32
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                id: level1MouseArea
                anchors.fill: parent
                onClicked: {
                    if (blinkenState.state === "selectLevelState") {
                        BlinkenGame.start(1);
                        blinkenState.state = "gameStart";
                    }
                }
            }
        }

        Rectangle {
            id: plane1
            color: "#282828"
            border.color: "black"

            width: plane.width
            height: plane.height
            radius: plane.radius
            border.width: plane.border.width

            Text {
                id: text3
                color: "#ffffff"
                text: i18n("2")
                anchors.fill: parent
                font.pixelSize: 32
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                id: level2MouseArea
                anchors.fill: parent
                onClicked: {
                    if (blinkenState.state === "selectLevelState") {
                        BlinkenGame.start(2);
                        blinkenState.state = "gameStart";
                    }
                }
            }
        }

        Rectangle {
            id: plane2
            color: "#282828"
            border.color: "black"

            width: plane.width
            height: plane.height
            radius: plane.radius
            border.width: plane.border.width

            Text {
                id: text4
                color: "#ffffff"
                text: i18n("?")
                anchors.fill: parent
                font.pixelSize: 32
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                id: level3MouseArea
                anchors.fill: parent
                onClicked: {
                    if (blinkenState.state === "selectLevelState") {
                        BlinkenGame.start(3);
                        blinkenState.state = "gameStart";
                    }
                }
            }
        }
    }

    Rectangle {
        id: restartButton
        color: "#282828"
        border.color: "black"

        x: startButton.x
        y: startButton.y
        width: startButton.width
        height: startButton.height
        visible: false
        radius: startButton.radius
        border.width: startButton.border.width

        Text {
            id: text5
            anchors.fill: parent
            color: "#ffffff"
            text: i18n("Restart")
            font.pixelSize: 15
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            onClicked: {
                if (blinkenState.state === "gameStart") {
                    BlinkenGame.setPhase(BlinkenGame.Starting);
                    blinkenState.state = "selectLevelState";
                }
            }
        }
    }

    StateGroup {
        id: blinkenState
        state: "default"
        states: [
            State {
                name: "default"
                PropertyChanges {
                    startButton.visible: true
                    optionsList.visible: false
                    restartButton.visible: false
                }
            },
            State {
                name: "selectLevelState"
                PropertyChanges {
                    startButton.visible: false
                    optionsList.visible: true
                    restartButton.visible: false
                }
            },
            State {
                name: "gameStart"
                PropertyChanges {
                    startButton.visible: false
                    restartButton.visible: true
                    optionsList.visible: false
                }
            }
        ]
    }
}
