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

import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import ProxyEmy.Backend

ApplicationWindow {
    minimumWidth: 510
    width: 640
    height: 480
    visible: true
    title: backend.windowTitle
    background: Rectangle {
        anchors.fill: parent
        color: "white"
    }

    ServerDetailsView {
        id: serverDetailsView
        anchors.fill: parent
    }

    ProxyEmyBackend {
        id: backend
    }

    HttpProxyServer {
        id: httpProxyServer
        configuration: configurationViewModel
        Component.onDestruction: {
            httpProxyServer.stopServer();
        }
    }

    ConfigurationViewModel {
        id: configurationViewModel
    }
}
