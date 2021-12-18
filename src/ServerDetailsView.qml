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
        spacing: 0

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
                            configurationViewModel.openConfigFolder();
                        }
                    }

                    LinkedText {
                        visible: httpProxyServer.serverStarted
                        minimumPointSize: 9
                        font.pointSize: 9
                        text: "Open in browser <a href='" + configurationViewModel.serverProtocol + "://localhost:" + configurationViewModel.port + "'>" + configurationViewModel.serverProtocol + "://localhost:" + configurationViewModel.port + "/</a>"
                        onLinkActivated: function(link) {
                            Qt.openUrlExternally(link);
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Item {
                width: parent.width
                height: 40

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    text: "Mappings:"
                    font.pointSize: 11
                }

                TextButton {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    title: "Add"
                    onPressed: {

                    }
                }
            }

            BorderedPanel {
                anchors.fill: parent
                anchors.topMargin: 42
                anchors.bottomMargin: 2
                anchors.rightMargin: 2

                TableView {
                    id: mappingsTable
                    visible: configurationViewModel.mappingListModel.isHasMappings
                    clip: true
                    anchors.fill: parent
                    columnSpacing: 0
                    rowSpacing: 1
                    model: configurationViewModel.mappingListModel
                    syncDirection: Qt.Vertical
                    ScrollBar.vertical: ScrollBar{
                        active: true
                    }
                    columnWidthProvider: function (column) {
                        return configurationViewModel.mappingListModel.getColumnWidth(column, mappingsTable.width);
                    }
                    delegate: Rectangle {
                        implicitWidth: 20
                        implicitHeight: 30
                        color: "transparent"

                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: column === 0 ? 10 : 0
                            visible: column !== 2
                            wrapMode: Text.NoWrap
                            text: display
                        }

                        IconButton {
                            icon: storagePaths.icons + "delete.svg"
                            width: 30
                            height: 30
                            iconWidth: 22
                            iconHeight: 22
                            anchors.centerIn: parent
                            visible: column === 2
                            onPressed: {
                                configurationViewModel.mappingListModel.deleteMapping(row);
                            }
                        }

                        Rectangle {
                            anchors.bottom: parent.bottom
                            color: "#e7eaec"
                            width: parent.width
                            height: 1
                        }
                    }
                    onWidthChanged: mappingsTable.forceLayout()
                }

                Item {
                    visible: !configurationViewModel.mappingListModel.isHasMappings
                    anchors.centerIn: parent
                    width: parent.width
                    height: 90

                    Image {
                        id: emptyboxImage
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: storagePaths.icons + "emptybox.svg"
                        width: 50
                        height: 50
                        mipmap: true
                    }
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: emptyboxImage.bottom
                        horizontalAlignment: Text.AlignHCenter
                        text: "You did not have mappings yet.\nPlease click on Add button."
                        font.pointSize: 9
                    }
                }
            }
        }
    }
}
