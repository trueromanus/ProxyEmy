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
import "Views/Controls"

ApplicationWindow {
    id: root
    width: 500
    height: 190
    minimumWidth: 500
    minimumHeight: 190
    maximumWidth: 500
    maximumHeight: 190
    modality: Qt.WindowModal
    flags: Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    title: "About"

    Image {
        id: logoImage
        source: storagePaths.icons + "logo.svg"
        width: 50
        height: 50
        mipmap: true
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    Text {
        id: applicationName
        anchors.left: logoImage.right
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 4
        text: "ProxyEmy"
        font.pointSize: 12
        font.bold: true
    }

    Text {
        id: versionInfo
        anchors.left: logoImage.right
        anchors.top: applicationName.bottom
        anchors.leftMargin: 20
        anchors.topMargin: 4
        text: "version 0.0.4"
        font.pointSize: 11
    }

    LinkedText {
        id: opensourceInfo
        anchors.left: logoImage.right
        anchors.top: versionInfo.bottom
        anchors.leftMargin: 20
        anchors.topMargin: 8
        text: "This application is open source.<br>You can check out sources <a href='https://github.com/trueromanus/ProxyEmy'>at this address</a>."
        font.pointSize: 11
        onLinkActivated: function (link) {
            Qt.openUrlExternally(link);
        }
    }

    LinkedText {
        id: qtInfo
        anchors.left: logoImage.right
        anchors.top: opensourceInfo.bottom
        anchors.leftMargin: 20
        anchors.topMargin: 8
        text: "Used Qt version 6.2.1<br>If you want to get sources of Qt please <a href='mailto:rorhww3glmwj@list.ru'>email this address</a>."
        font.pointSize: 11
        onLinkActivated: function (link) {
            Qt.openUrlExternally(link);
        }
    }

    Text {
        id: copyrightInfo
        anchors.left: logoImage.right
        anchors.top: qtInfo.bottom
        anchors.leftMargin: 20
        anchors.topMargin: 8
        text: "Copyright (c) 2022 Roman Vladimirov. All right reserved."
        font.pointSize: 11
    }
}
