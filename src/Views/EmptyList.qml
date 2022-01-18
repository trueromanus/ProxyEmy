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
    anchors.centerIn: parent
    width: parent.width
    height: 90

    property alias title: emptyMessage.text

    Image {
        id: emptyboxImage
        anchors.horizontalCenter: parent.horizontalCenter
        source: storagePaths.icons + "emptybox.svg"
        width: 50
        height: 50
        mipmap: true
    }
    Text {
        id: emptyMessage
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: emptyboxImage.bottom
        horizontalAlignment: Text.AlignHCenter
        text: "You did not have mappings yet.\nPlease click on Add button."
        font.pointSize: 9
    }
}
