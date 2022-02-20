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
import "Controls"

Item {
    id: root
    height: 30
    width: parent.width

    Rectangle {
        opacity: .2
        color: "#f2f2f2"
        anchors.fill: parent
    }

    Row {
        anchors.fill: parent
        anchors.leftMargin: 8
        spacing: 2

        Repeater {
            model: backend.tabs
            delegate: Item {
                id: itemRoot
                width: 120
                height: root.height
                property bool itemHovered: false

                Item {
                    clip: true
                    anchors.fill: parent

                    Rectangle {
                        color: isActived ? "white" : (itemRoot.itemHovered ? Qt.rgba(1,1,1,.2) : "transparent")
                        width: parent.width
                        height: parent.height + 4
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                        border.width: 1
                        border.color: isActived ? "#e7eaec" : "transparent"
                        radius: 4
                    }
                }

                Text {
                    anchors.centerIn: parent
                    font.pointSize: 11
                    text: title
                    color: isActived || itemRoot.itemHovered ? "black" : "gray"
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onPressed: {
                        if (isActived) return;

                        backend.tabs.activateTab(identifier);
                    }
                    onEntered: {
                        itemRoot.itemHovered = true;
                    }
                    onExited: {
                        itemRoot.itemHovered = false;
                    }
                }
            }
        }
    }

    IconButton {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        icon: storagePaths.icons + "info.svg"
        width: 30
        height: parent.height
        iconWidth: 20
        iconHeight: 20
        tooltipMessage: "About"
        onPressed: {
            aboutWindow.show();
        }
    }

}
