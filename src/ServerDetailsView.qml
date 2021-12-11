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
import QtQuick.Layouts
import QtQuick.Controls
import "Views/Controls"

Item {
    id: root

    RowLayout {
        anchors.fill: parent
        spacing: 2

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            BorderedPanel {
                anchors.fill: parent
                anchors.margins: 2

                Column {
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    anchors.topMargin: 4
                    anchors.bottomMargin: 4
                    spacing: 2

                    Item {
                        width: parent.width
                        height: 46

                        Row {
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 4

                            TextButton {
                                disabled: httpProxyServer.serverStarted
                                title: "Start"
                                onPressed: {
                                    httpProxyServer.startServer();
                                }
                            }

                            TextButton {
                                disabled: !httpProxyServer.serverStarted
                                title: "Stop"
                                onPressed: {
                                    httpProxyServer.stopServer();
                                }
                            }

                            TextButton {
                                visible: httpProxyServer.serverStarted
                                title: "Restart"
                                onPressed: {
                                    httpProxyServer.restartServer();
                                }
                            }
                        }
                    }

                    Text {
                        text: httpProxyServer.serverStarted ? "Running on port " + configurationViewModel.port : "Not started"
                        color: httpProxyServer.serverStarted ? "green" : "red"
                        minimumPointSize: 11
                        font.pointSize: 11
                    }

                    LinkedText {
                        visible: configurationViewModel.isConfigReaded
                        text: "Configuration readed <a href='http://config'>from file</a>"
                        minimumPointSize: 9
                        font.pointSize: 9
                        linkTooltip: configurationViewModel.pathToYaml
                        onLinkActivated: {
                            configurationViewModel.openConfigFolder()
                        }
                    }

                    LinkedText {
                        visible: httpProxyServer.serverStarted
                        minimumPointSize: 9
                        font.pointSize: 9
                        text: "Open in browser <a href='http://localhost:" + configurationViewModel.port + "'>http://localhost:" + configurationViewModel.port + "/</a>"
                        onLinkActivated: Qt.openUrlExternally(link)
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true


        }
    }
}
