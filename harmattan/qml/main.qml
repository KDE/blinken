/***************************************************************************
 *   Copyright (C) 2011 by Albert Astals Cid <aacid@kde.org>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

import QtQuick 1.1
import com.nokia.meego 1.0
import BlinkenLibrary 1.0

PageStackWindow
{
    id: appWindow

    initialPage: mainPage

    HighScoreManager
    {
        id: hsm
    }

    QueryDialog
    {
        id: aboutDialog
        message: qsTr("Blinken for Meego Harmattan 1.0.2<br><br>tsdgeos@yahoo.es<br><br>This software is GPLv2+ licensed")
    }

    Dialog
    {
        id: highScoreDialog

        function scoreText(score)
        {
            if (score == 0) return ""
            else return score
        }

        function showLevel(level)
        {
            if (level == 0) levelChooser.checkedButton = level1Button
            else if (level == 1) levelChooser.checkedButton = level2Button
            else if (level == 2) levelChooser.checkedButton = level3Button
            name1.text = hsm.name(level, 0)
            score1.text = scoreText(hsm.score(level, 0))
            name2.text = hsm.name(level, 1)
            score2.text = scoreText(hsm.score(level, 1))
            name3.text = hsm.name(level, 2)
            score3.text = scoreText(hsm.score(level, 2))
            name4.text = hsm.name(level, 3)
            score4.text = scoreText(hsm.score(level, 3))
            name5.text = hsm.name(level, 4)
            score5.text = scoreText(hsm.score(level, 4))
        }

        content: Column
        {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            Label
            {
                font.family: style.titleFontFamily
                font.pixelSize: style.titleFontPixelSize + 10
                font.bold:  style.titleFontBold
                font.capitalization: style.titleFontCapitalization
                color: style.titleTextColor
                horizontalAlignment: Text.AlignHCenter
                width: parent.width

                text: qsTr("High Scores")
            }

            Row
            {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 20

                Column
                {
                    spacing: 10
                    Label
                    {
                        font.family: style.titleFontFamily
                        font.pixelSize: style.titleFontPixelSize
                        font.bold:  style.titleFontBold
                        font.capitalization: style.titleFontCapitalization
                        color: style.titleTextColor

                        id: name1
                    }
                    Label
                    {
                        font.family: style.titleFontFamily
                        font.pixelSize: style.titleFontPixelSize
                        font.bold:  style.titleFontBold
                        font.capitalization: style.titleFontCapitalization
                        color: style.titleTextColor

                        id: name2
                    }
                    Label
                    {
                        font.family: style.titleFontFamily
                        font.pixelSize: style.titleFontPixelSize
                        font.bold:  style.titleFontBold
                        font.capitalization: style.titleFontCapitalization
                        color: style.titleTextColor

                        id: name3
                    }
                    Label
                    {
                        font.family: style.titleFontFamily
                        font.pixelSize: style.titleFontPixelSize
                        font.bold:  style.titleFontBold
                        font.capitalization: style.titleFontCapitalization
                        color: style.titleTextColor

                        id: name4
                    }
                    Label
                    {
                        font.family: style.titleFontFamily
                        font.pixelSize: style.titleFontPixelSize
                        font.bold:  style.titleFontBold
                        font.capitalization: style.titleFontCapitalization
                        color: style.titleTextColor

                        id: name5
                    }
                }
                Column
                {
                    spacing: 10
                    Label
                    {
                        font.family: style.titleFontFamily
                        font.pixelSize: style.titleFontPixelSize
                        font.bold:  style.titleFontBold
                        font.capitalization: style.titleFontCapitalization
                        color: style.titleTextColor
                        horizontalAlignment: Text.AlignRight

                        id: score1
                    }
                    Label
                    {
                        font.family: style.titleFontFamily
                        font.pixelSize: style.titleFontPixelSize
                        font.bold:  style.titleFontBold
                        font.capitalization: style.titleFontCapitalization
                        color: style.titleTextColor
                        horizontalAlignment: Text.AlignRight
                        width: parent.width

                        id: score2
                    }
                    Label
                    {
                        font.family: style.titleFontFamily
                        font.pixelSize: style.titleFontPixelSize
                        font.bold:  style.titleFontBold
                        font.capitalization: style.titleFontCapitalization
                        color: style.titleTextColor
                        horizontalAlignment: Text.AlignRight
                        width: parent.width

                        id: score3
                    }
                    Label
                    {
                        font.family: style.titleFontFamily
                        font.pixelSize: style.titleFontPixelSize
                        font.bold:  style.titleFontBold
                        font.capitalization: style.titleFontCapitalization
                        color: style.titleTextColor
                        horizontalAlignment: Text.AlignRight
                        width: parent.width

                        id: score4
                    }
                    Label
                    {
                        font.family: style.titleFontFamily
                        font.pixelSize: style.titleFontPixelSize
                        font.bold:  style.titleFontBold
                        font.capitalization: style.titleFontCapitalization
                        color: style.titleTextColor
                        horizontalAlignment: Text.AlignRight
                        width: parent.width

                        id: score5
                    }
                }
            }

            ButtonRow
            {
                id: levelChooser
                Button
                {
                    id: level1Button
                    text: qsTr("Level 1")
                    onClicked: highScoreDialog.showLevel(0)
                }
                Button
                {
                    id: level2Button
                    text: qsTr("Level 2")
                    onClicked: highScoreDialog.showLevel(1)
                }
                Button
                {
                    id: level3Button
                    text: qsTr("Level ?")
                    onClicked: highScoreDialog.showLevel(2)
                }
            }
        }

        buttons: Row
        {
        }
    }

    Dialog
    {
        property alias name: nameText.text
        property int score: -1
        property int level: -1

        QueryDialogStyle
        {
            id: style
        }

        id: askForNameDialog

        content: Column
        {
            id: nameColumn
            anchors.horizontalCenter: parent.horizontalCenter
            height: 110

            Label
            {
                font.family: style.titleFontFamily
                font.pixelSize: style.titleFontPixelSize
                font.bold:  style.titleFontBold
                font.capitalization: style.titleFontCapitalization
                color: style.titleTextColor

                id: text
                text: qsTr("Enter Your Name:")
            }

            TextField
            {
                id: nameText
                inputMethodHints: Qt.ImhNoPredictiveText
                width: 400
                height: 50
            }
        }

        buttons: Row
        {
            spacing: style.buttonsColumnSpacing
            anchors.horizontalCenter: parent.horizontalCenter
            Button
            {
                __dialogButton: true
                platformStyle: ButtonStyle {inverted: true}
                text: qsTr("OK");
                onClicked:
                {
                    askForNameDialog.accept()
                    mainPage.setHighScore(nameText.text, askForNameDialog.level, askForNameDialog.score);
                }
            }
            Button
            {
                __dialogButton: true
                platformStyle: ButtonStyle {inverted: true}
                text: qsTr("Cancel");
                onClicked:
                {
                    askForNameDialog.reject()
                }
            }
        }
    }

    Page
    {
        orientationLock: PageOrientation.LockLandscape
        id: mainPage

        Rectangle
        {
            anchors.fill: parent
            color: "red"
        }

        BlinkenWidget
        {
            id: blinken
            anchors.fill: parent
            onAskForName:
            {
                askForNameDialog.open()
                askForNameDialog.name = lastName
                askForNameDialog.level = level
                askForNameDialog.score = score
            }

            onShowHighScoreDialog:
            {
                highScoreDialog.showLevel(level)
                highScoreDialog.open()
            }

            onShowAbout:
            {
                aboutDialog.open()
            }
        }

        function setHighScore(name, level, score)
        {
            blinken.setHighScore(name, level, score)
        }
    }

}
