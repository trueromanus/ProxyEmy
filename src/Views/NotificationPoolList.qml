/*
    ProxyEmy local proxy server
    Copyright (C) 2022 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

import QtQuick

Item {
    id: root

    Column {
        anchors.fill: parent
        spacing: 2

        Repeater {
            clip: true
            model: notificationHub.pool
            delegate: Item {
                width: root.width
                height: 80
                Rectangle {
                    id: leftBorder
                    anchors.left: parent.left
                    width: 4
                    height: parent.height - 4
                    color: isError ? "#ED5565" : "#1c84c6"
                }

                Rectangle {
                    anchors.top: parent.top
                    anchors.left: leftBorder.right
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 4
                    color: isError ? "#fde8ea" : "#e9f4fc"
                }

                Image {
                    id: messageIcon
                    anchors.left: leftBorder.right
                    anchors.leftMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    width: 30
                    height: 30
                    mipmap: true
                    source: storagePaths.icons + (isError ? "error.svg" : "info.svg")
                }

                Column {
                    id: labels
                    spacing: 2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: messageIcon.right
                    anchors.leftMargin: 8
                    anchors.right: parent.right

                    Text {
                        width: labels.width - 8
                        height: 20
                        font.pointSize: 12
                        text: title
                        elide: Qt.ElideRight
                        maximumLineCount: 1
                    }

                    Text {
                        width: labels.width - 8
                        height: 40
                        font.pointSize: 10
                        text: message
                        elide: Qt.ElideRight
                        wrapMode: Text.WordWrap
                        maximumLineCount: 2
                    }
                }

                NumberAnimation on opacity {
                    running: isActiveAnimation
                    from: 1
                    to: 0
                    duration: 300
                    onFinished: {
                        notificationHub.pool.itemAnimationEnded();
                    }
                }
            }
        }
    }
}
