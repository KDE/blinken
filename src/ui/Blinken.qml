/*
    SPDX-FileCopyrightText: 2024 Hanyang Zhang <hanyangzhang@qq.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.blinken

Kirigami.AbstractApplicationWindow {
    id: blinken
    title: i18n("Blinken")
    color: "black"
    readonly property int blinkenBackgroundWidth: 814
    readonly property int blinkenBackgroundHeight: 696
    Rectangle {
        id: rootRectangle
        height: parent.height<parent.width*(3/4)?parent.height:parent.width*(3/4)
        width: parent.height<parent.width*(3/4)?height * (4 / 3):parent.width
        anchors.centerIn: parent

        Image {
            id: background
            anchors.fill: parent
            source: "qrc:ui/background.svg"
            fillMode: Image.Stretch

            GameButtons {
                id: gameButtons
                anchors.fill: parent
            }
        }

        ExitButton {
            id: exitButton
            width: rootRectangle.width * (80 / blinken.blinkenBackgroundWidth)
            height: width
            anchors.right: parent.right
            anchors.top: parent.top
            scale: rootRectangle.scale
            anchors.rightMargin: rootRectangle.width * (8 / blinken.blinkenBackgroundWidth)
            anchors.topMargin: rootRectangle.height * (8 / blinken.blinkenBackgroundHeight)
        }

        BlinkenMenu {
            id: blinkenMenu
            width: rootRectangle.width * (230 / blinken.blinkenBackgroundWidth)
            height: width
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: rootRectangle.height * (8 / blinken.blinkenBackgroundHeight)
            anchors.rightMargin: rootRectangle.width * (8 / blinken.blinkenBackgroundWidth)
        }

        HighScoreButton {
            id: highScoreButton
            width: rootRectangle.width * (80 / blinken.blinkenBackgroundWidth)
            height: width
            anchors.left: parent.left
            anchors.top: parent.top
            scale: rootRectangle.scale
            anchors.leftMargin: rootRectangle.width * (8 / blinken.blinkenBackgroundWidth)
            anchors.topMargin: rootRectangle.height * (8 / blinken.blinkenBackgroundHeight)
            mouseArea.onPressed: highScoreButtonImage.source = "qrc:ui/highScore_highlight.svg"
            mouseArea.onReleased: {
                highScoreButtonImage.source = "qrc:ui/highScore.svg";
                highScoreLists.openLists();
            }
        }
        HighScoreLists {
            id: highScoreLists
            highScoreListsPopup.anchors.centerIn: Overlay.overlay
            highScoreListsPopup.width: rootRectangle.width * 0.5
            highScoreListsPopup.height: rootRectangle.height * 0.8
        }
        FontLoader {
            id: steveFontloader
            source: "qrc:steve.ttf"
        }
        Text {
            id: statusText
            width: rootRectangle.width * (500/ blinken.blinkenBackgroundWidth)
            height: rootRectangle.height * (160  / blinken.blinkenBackgroundHeight)
            color: "#0062f7" //To look more like the original
            font.family: BlinkenSettings.customFont ? steveFontloader.name : ""
            text: i18n("Press Start to begin")
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: rootRectangle.height * (0 / blinken.blinkenBackgroundHeight)
            anchors.leftMargin: rootRectangle.width * (30 / blinken.blinkenBackgroundWidth)

            font.pixelSize: 40
            fontSizeMode: Text.HorizontalFit
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignBottom
            wrapMode:Text.WrapAnywhere
            transform: Rotation { origin.x: -3; origin.y: -3; angle: -3.3}
        }
        ScoreAndCounter {
            id: scoreAndCounter
            width: rootRectangle.width * (150 / blinken.blinkenBackgroundWidth)
            height: rootRectangle.height * (85 / blinken.blinkenBackgroundHeight)
            anchors.top: parent.top
            anchors.topMargin: parent.height * 0.17
            anchors.horizontalCenter: parent.horizontalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: highScoreLists.openLists()
            }
        }

        GameOptions {
            id: gameOptions
            anchors.horizontalCenter: rootRectangle.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height * 0.43
            startButton.width: rootRectangle.width * (95 / blinken.blinkenBackgroundWidth)
            startButton.height: rootRectangle.height * (65 / blinken.blinkenBackgroundHeight)

            optionsList.width: rootRectangle.width * (250 / blinken.blinkenBackgroundWidth)
            optionsList.height: rootRectangle.height * (90 / blinken.blinkenBackgroundHeight)
            optionsList.spacing: rootRectangle.width * (45 / blinken.blinkenBackgroundWidth)
        }
    }

    property string playerName: ""
    property int lastGameScore: -1
    Dialog {
        id: nameDialog
        modal: true
        closePolicy: Popup.NoAutoClose
        anchors.centerIn: parent
        title: i18n("Enter Your Name")
        standardButtons: Dialog.Ok
        RowLayout {
            Label {
                text: i18nc("@label:textbox refers to the user's name", "Name:")
            }
            TextInput {
                id: nameInput
                focus: true
                font.bold: true
                text: blinken.playerName
                //old Blinken doesn't have the limit, but whenever the name is too long, it can't be displayed very well
                maximumLength: 20
            }
        }
        onAccepted: {
            let name = nameInput.text;
            blinken.playerName = nameInput.text;
            if (name != "" && lastGameScore > 0)
                HighScoreManager.addScore(BlinkenGame.level(), lastGameScore, name);
            lastGameScore = -1;
        }
    }

    //about Blinken Page
    Popup {
        id: aboutBlinkenPage
        anchors.centerIn: parent
        height: rootRectangle.height * 0.9
        width: rootRectangle.width * 0.7
        FormCard.AboutPage {
            anchors.fill: parent
        }
    }
    //about KDE Page
    Popup {
        id: aboutKDEPage
        anchors.centerIn: parent
        height: rootRectangle.height * 0.9
        width: rootRectangle.width * 0.7
        FormCard.AboutKDEPage {
            anchors.fill: parent
        }
    }
    // Settting
    Popup {
        id: settingsPage
        anchors.centerIn: parent
        height: rootRectangle.height * (170 / blinken.blinkenBackgroundHeight)
        width: rootRectangle.width * (450 / blinken.blinkenBackgroundWidth)
        Column {
            Switch {
                text: i18n("Sounds")
                checked: BlinkenSettings.playSounds
                onToggled: {
                    BlinkenSettings.playSounds = !BlinkenSettings.playSounds;
                    BlinkenSettings.save();
                }
            }
            Switch {
                text: i18n("Use custom font for status text")
                checked: BlinkenSettings.customFont
                onToggled: {
                    var aux = i18nc("If the Steve font that is used by Blinken by default to show status messages does not support any of the characters of your language, please translate that message to 1 and KDE standard font will be used to show the texts, if not translate it to 0", "0");
                    if (BlinkenSettings.customFont) {
                        statusText.font.family = "";
                    } else {
                        if (aux == "0")
                            statusText.font.family = steveFontloader.name;
                    }
                    BlinkenSettings.customFont = !BlinkenSettings.customFont;
                    BlinkenSettings.save();
                }
            }
        }
    }
    Connections {
        target: blinkenMenu
        function onOpenAboutBlinkenPage() {
            aboutBlinkenPage.open();
        }
        function onOpenAboutKDEPage() {
            aboutKDEPage.open();
        }
        function onOpenSettingsPage() {
            settingsPage.open();
        }
        function onOpenHandBookPage() {
            Qt.openUrlExternally("help:/");
        }
    }

    Connections {
        target: BlinkenGame
        function onGameEnded() {
            let score = BlinkenGame.score();
            let level = BlinkenGame.level();
            if (HighScoreManager.scoreGoodEnough(level, score)) {
                lastGameScore = score;
                nameDialog.open();
            }
        }
        function onPhaseChanged() {
            switch (BlinkenGame.phase) {
            case (BlinkenGame.Starting):
                {
                    statusText.text = i18n("Press Start to begin");
                    scoreAndCounter.setScore(0);
                    scoreAndCounter.counter.state = "zero";
                    break;
                }
            case (BlinkenGame.ChoosingLevel):
                {
                    statusText.text = i18n("Set the Difficulty Level...");
                    scoreAndCounter.setScore(0);
                    scoreAndCounter.counter.state = "zero";
                    gameOptions.blinkenState.state = "selectLevelState";
                    break;
                }
            case (BlinkenGame.Waiting3):
                {
                    statusText.text = i18n("Next sequence in 3...");
                    scoreAndCounter.setScore(BlinkenGame.score());
                    scoreAndCounter.counter.state = "three";
                    break;
                }
            case (BlinkenGame.Waiting2):
                {
                    if (BlinkenGame.level() === 1) {
                        statusText.text = i18n("Next sequence in 3, 2...");
                    } else {
                        statusText.text = i18n("Next sequence in 2...");
                    }
                    scoreAndCounter.setScore(BlinkenGame.score());
                    scoreAndCounter.counter.state = "two";
                    break;
                }
            case (BlinkenGame.Waiting1):
                {
                    if (BlinkenGame.level() === 1) {
                        statusText.text = i18n("Next sequence in 3, 2, 1...");
                    } else {
                        statusText.text = i18n("Next sequence in 2, 1...");
                    }
                    scoreAndCounter.counter.state = "one";
                    break;
                }
            case (BlinkenGame.LearningTheSequence):
                {
                    statusText.text = i18n("Remember this sequence...");
                    scoreAndCounter.counter.state = "zero";
                    break;
                }
            case (BlinkenGame.TypingTheSequence):
                {
                    statusText.text = i18n("Repeat the sequence");
                    break;
                }
            default:
                {
                    // never happens
                    break;
                }
            }
        }
        function onHighlight(aimColor, unhighlight) {
            gameButtons.highLight(aimColor);
            if (aimColor == 15)
                timer.setTimeout(function () {
                    gameButtons.highLight(0);
                }, 250);
        }
    }
    Timer {
        id: timer
        function setTimeout(cb, delayTime) {
            timer.interval = delayTime;
            timer.repeat = false;
            timer.triggered.connect(cb);
            timer.triggered.connect(function release() {
                timer.triggered.disconnect(cb);
                timer.triggered.disconnect(release);
            });
            timer.start();
        }
    }
}
