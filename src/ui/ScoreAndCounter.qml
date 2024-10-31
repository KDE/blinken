/*
    SPDX-FileCopyrightText: 2024 Hanyang Zhang <hanyangzhang@qq.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
import QtQuick

Item {
    id: scoreAndCounter
    property alias counter: counter

    Rectangle {
        id: plane
        color: "#282828"
        border.color: "black"

        radius: scoreAndCounter.height * 0.20
        border.width: scoreAndCounter.height * 0.075
        anchors.fill: parent
        scale: scoreAndCounter.scale

        Numbers {
            id: score
            width: scoreAndCounter.width * 0.7
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }

        Column {
            id: countDowns
            width: scoreAndCounter.width * 0.219
            height: scoreAndCounter.height * 0.8

            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: scoreAndCounter.width * 0.01
            anchors.topMargin: scoreAndCounter.height * 0.18

            spacing: scoreAndCounter.height * 0.02
            scale: scoreAndCounter.scale

            Image {
                id: countDown3
                width: scoreAndCounter.width * 0.1
                height: countDown3.width
                source: "qrc:numbers/blackBlock.svg"
                sourceSize.width: scoreAndCounter.width * 0.1369
                scale: scoreAndCounter.scale
                fillMode: Image.PreserveAspectFit
            }

            Image {
                id: countDown2
                width: countDown3.width
                height: countDown3.width
                source: "qrc:numbers/blackBlock.svg"
                scale: scoreAndCounter.scale
                fillMode: Image.PreserveAspectFit
            }

            Image {
                id: countDown1
                width: countDown3.width
                height: countDown3.width
                source: "qrc:numbers/blackBlock.svg"
                scale: scoreAndCounter.scale
                fillMode: Image.PreserveAspectFit
            }
        }
    }
    function setScore(scores: int) {
        score.setNumbers(scores);
    }

    StateGroup {
        id: counter
        state: "zero"
        states: [
            State {
                name: "zero"
                PropertyChanges {
                    countDown3.source: "qrc:numbers/blackBlock.svg"
                    countDown2.source: "qrc:numbers/blackBlock.svg"
                    countDown1.source: "qrc:numbers/blackBlock.svg"
                }
            },
            State {
                name: "three"
                PropertyChanges {
                    countDown3.source: "qrc:numbers/redBlock.svg"
                    countDown2.source: "qrc:numbers/redBlock.svg"
                    countDown1.source: "qrc:numbers/redBlock.svg"
                }
            },
            State {
                name: "two"
                PropertyChanges {
                    countDown3.source: "qrc:/numbers/blackBlock.svg"
                    countDown2.source: "qrc:/numbers/redBlock.svg"
                    countDown1.source: "qrc:/numbers/redBlock.svg"
                }
            },
            State {
                name: "one"
                PropertyChanges {
                    countDown3.source: "qrc:numbers/blackBlock.svg"
                    countDown1.source: "qrc:numbers/redBlock.svg"
                    countDown2.source: "qrc:numbers/blackBlock.svg"
                }
            }
        ]
    }
}
