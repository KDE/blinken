/*
    SPDX-FileCopyrightText: 2024 Hanyang Zhang <hanyangzhang@qq.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
pragma ComponentBehavior: Bound
import QtQuick

Item {
    property alias levelListModel: levelListModel
    // List Data
    ListModel {
        id: levelListModel
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: levelListModel
        spacing: 10
        orientation: ListView.Vertical
        snapMode: ListView.SnapToItem
        clip: true
        header: Rectangle {
            // Making the first item away from tab buttons looks better
            height: listView.spacing
        }
        delegate: Rectangle {
            id: scoreLine
            required property string name
            required property string score
            color: "#00ffffff"
            width: listView.width
            height: listView.height * 0.18

            Rectangle {
                id: numersRect
                width: scoreLine.width * 0.25
                height: scoreLine.height
                color: "#00ffffff"
                border.color: "#000000"
                border.width: 2
                Numbers {
                    id: scoreNumbers
                    anchors.fill: numersRect
                    Component.onCompleted: {
                        setNumbers(scoreLine.score);
                    }
                }
            }
            Rectangle {
                id: nameRect
                color: "#00ffffff"
                height: scoreLine.height
                anchors.left: numersRect.right
                anchors.right: scoreLine.right
                anchors.leftMargin: 10

                FontLoader {
                    id: steveFontloader
                    source: "qrc:steve.ttf"
                }
                Text {
                    text: scoreLine.name
                    font.pixelSize: 18
                    font.family: BlinkenSettings.customFont ? steveFontloader.name : ""
                    anchors.verticalCenter: nameRect.verticalCenter
                }
            }
        }
    }
}
