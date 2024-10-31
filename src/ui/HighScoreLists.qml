/*
    SPDX-FileCopyrightText: 2024 Hanyang Zhang <hanyangzhang@qq.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: highScoreLists
    property alias highScoreListsPopup: highScoreListsPopup

    function openLists() {
        highScoreListsPopup.open();
        level1List.levelListModel.clear();
        level2List.levelListModel.clear();
        level3List.levelListModel.clear();
        for (let i = 0; i < 5; i++) {
            let scoreItem = HighScoreManager.score(0, i);
            let nameItem = HighScoreManager.name(0, i);
            if (scoreItem == 0)
                break;
            level1List.levelListModel.set(i, {
                score: scoreItem,
                name: nameItem
            });
        }
        for (let i = 0; i < 5; i++) {
            let scoreItem = HighScoreManager.score(1, i);
            let nameItem = HighScoreManager.name(1, i);
            if (scoreItem == 0)
                break;
            level2List.levelListModel.set(i, {
                score: scoreItem,
                name: nameItem
            });
        }
        for (let i = 0; i < 5; i++) {
            let scoreItem = HighScoreManager.score(2, i);
            let nameItem = HighScoreManager.name(2, i);
            if (scoreItem == 0)
                break;
            level3List.levelListModel.set(i, {
                score: scoreItem,
                name: nameItem
            });
        }
    }
    Popup {
        id: highScoreListsPopup

        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        padding: 10
        modal: true

        contentItem: Page {
            id: scorePage
            header: TabBar {
                id: bar
                width: highScoreListsPopup.contentItem.width

                TabButton {
                    text: i18nc("@title:group High scores Level 1 tab title", "Level 1")
                    width: (highScoreListsPopup.contentItem.width) / 3
                }
                TabButton {
                    text: i18nc("@title:group High scores Level 2 tab title", "Level 2")
                    width: (highScoreListsPopup.contentItem.width) / 3
                }
                TabButton {
                    text: i18nc("@title:group High scores Level ? tab tible", "Level ?")
                    width: (highScoreListsPopup.contentItem.width) / 3
                }
            }

            StackLayout {
                id: stackLayout
                width: parent.width
                height: parent.height
                currentIndex: bar.currentIndex
                ScoreList {
                    id: level1List
                }
                ScoreList {
                    id: level2List
                }
                ScoreList {
                    id: level3List
                }
            }
        }
    }
}
