/*
    SPDX-FileCopyrightText: 2024 Hanyang Zhang <hanyangzhang@qq.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
import QtQuick

Rectangle {
    id: numbers
    color: "transparent"
    Row {
        anchors.fill: parent
        leftPadding: parent.width * 0.09589
        topPadding: parent.height * 0.14
        spacing: parent.width * 0.05479
        scale: parent.scale

        Image {
            id: number2
            width: parent.width * 0.4
            height: parent.height * 0.7
            source: "qrc:numbers/0.svg"
            scale: parent.scale
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: number1
            width: number2.width
            height: number2.height
            source: "qrc:numbers/0.svg"
            scale: parent.scale
            fillMode: Image.PreserveAspectFit
        }
    }
    function setNumbers(number: int) {
        var num1 = number % 10;
        var num2 = Math.floor(number / 10);
        number1.source = "qrc:numbers/" + num1 + ".svg";
        number2.source = "qrc:numbers/" + num2 + ".svg";
    }
}
