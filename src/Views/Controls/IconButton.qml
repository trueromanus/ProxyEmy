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
import QtQuick.Controls

Item {
    id: root
    property alias icon: image.source
    property alias iconWidth: image.width
    property alias iconHeight: image.height
    property string tooltipMessage: ""
    property bool hoveredContainer: false

    signal pressed()

    Image {
        id: image
        anchors.centerIn: parent
        width: 22
        height: 22
        mipmap: true
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPressed: {
            root.pressed();
        }
        onEntered: {
            cursorShape = Qt.PointingHandCursor;
            hoveredContainer = true;
        }
        onExited: {
            cursorShape = Qt.ArrowCursor;
            hoveredContainer = false;
        }
    }

    ToolTip.delay: 1000
    ToolTip.visible: tooltipMessage && hoveredContainer
    ToolTip.text: tooltipMessage
}
