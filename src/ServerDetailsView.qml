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
import QtQuick.Dialogs
import "Views/Controls"
import "Views"

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
                anchors.leftMargin: 2
                anchors.rightMargin: 2
                anchors.bottomMargin: 2

                Column {
                    id: serverInfoPanel
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.top: parent.top
                    anchors.topMargin: 4
                    height: parent.height / 2 - aliasesHeaderPanel.height
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

                    LinkedText {
                        visible: configurationViewModel.isHasChanges
                        text: "Configuration changed!<br>You can save changes to <a href='opened'>opened file</a> or <a href='new'>new file</a>"
                        onLinkActivated: function (link){
                            if (link === `new`) {
                                saveConfigurationDialog.open();
                                return;
                            }
                            configurationViewModel.saveConfiguration(true, "");
                        }
                    }
                    LinkedText {
                        visible: configurationViewModel.serverProtocol === `https`
                        width: parent.width
                        wrapMode: Text.WordWrap
                        maximumLineCount: 2
                        text: "For avoiding browser warnings you can <a href='install'>install a root certificate</a>."
                        onLinkActivated: function (link){
                            backend.installRootCA();
                        }
                    }
                }

                Item {
                    id: aliasesHeaderPanel
                    width: parent.width
                    height: 40
                    anchors.top: serverInfoPanel.bottom

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        text: "Aliases:"
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

                VerticalBorder {
                    anchors.bottom: aliasesHeaderPanel.bottom
                }

                TableView {
                    id: aliasesTable
                    visible: configurationViewModel.aliasesListModel.isHasAliases
                    clip: true
                    anchors.top: aliasesHeaderPanel.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    columnSpacing: 0
                    rowSpacing: 1
                    model: configurationViewModel.aliasesListModel
                    flickDeceleration: 1000
                    syncDirection: Qt.Vertical
                    ScrollBar.vertical: ScrollBar{
                        active: true
                    }
                    columnWidthProvider: function (column) {
                        return configurationViewModel.aliasesListModel.getColumnWidth(column, aliasesTable.width);
                    }
                    delegate: Rectangle {
                        implicitWidth: 20
                        implicitHeight: 30
                        color: "transparent"

                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: column === 0 ? 10 : 0
                            visible: column !== 2 && !isEditing
                            width: parent.width - 10
                            wrapMode: Text.WrapAnywhere
                            elide: Qt.ElideRight
                            maximumLineCount: 1
                            text: textValue

                            MouseArea {
                                anchors.fill: parent
                                onPressed: {
                                    configurationViewModel.aliasesListModel.enableEditing(aliasKey);
                                }
                            }
                        }
                        TextField {
                            visible: column !== 2 && isEditing
                            anchors.fill: parent
                            text: column === 0 ? aliasKey : aliasValue
                            onTextChanged: {
                                configurationViewModel.aliasesListModel.setEditingValue(aliasKey, column, text);
                            }
                        }

                        Item {
                            visible: column === 2 && !isEditing
                            width: parent.width - 10
                            height: parent.height

                            IconButton {
                                icon: storagePaths.icons + "delete.svg"
                                width: 30
                                height: 30
                                iconWidth: 22
                                iconHeight: 22
                                anchors.centerIn: parent
                                onPressed: {
                                    configurationViewModel.deleteAlias(aliasKey);
                                }
                            }
                        }

                        Row {
                            width: parent.width - 10
                            spacing: 2
                            height: 30
                            anchors.verticalCenter: parent.verticalCenter
                            visible: column === 2 && isEditing

                            IconButton {
                                icon: storagePaths.icons + "save.svg"
                                width: 26
                                height: 30
                                iconWidth: 22
                                iconHeight: 22
                                onPressed: {
                                    configurationViewModel.editAlias(aliasKey);
                                }
                            }

                            IconButton {
                                icon: storagePaths.icons + "cancel.svg"
                                width: 26
                                height: 30
                                iconWidth: 22
                                iconHeight: 22
                                onPressed: {
                                    configurationViewModel.aliasesListModel.disableEditing(aliasKey);
                                }
                            }
                        }

                        Rectangle {
                            anchors.bottom: parent.bottom
                            color: "#e7eaec"
                            width: parent.width
                            height: 1
                        }
                    }
                    onWidthChanged: aliasesTable.forceLayout()
                }

                Item {
                    visible: !configurationViewModel.aliasesListModel.isHasAliases
                    anchors.top: aliasesHeaderPanel.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

                    EmptyList {
                        anchors.centerIn: parent
                        title: "You did not have aliases yet.\nPlease click on Add button."
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
                    flickDeceleration: 1000
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
                            visible: column !== 2 && !isEditing
                            width: parent.width - 10
                            wrapMode: Text.WrapAnywhere
                            elide: Qt.ElideRight
                            maximumLineCount: 1
                            text: textValue

                            MouseArea {
                                anchors.fill: parent
                                onPressed: {
                                    configurationViewModel.mappingListModel.enableEditing(row);
                                }
                            }
                        }
                        TextField {
                            visible: column !== 2 && isEditing
                            anchors.fill: parent
                            text: column === 0 ? localRoute : externalRouteOrigin
                            onTextChanged: {
                                configurationViewModel.mappingListModel.setEditingValue(identifier, column, text);
                            }
                        }

                        Item {
                            visible: column === 2 && !isEditing
                            width: parent.width - 10
                            height: parent.height

                            IconButton {
                                icon: storagePaths.icons + "delete.svg"
                                width: 30
                                height: 30
                                iconWidth: 22
                                iconHeight: 22
                                anchors.centerIn: parent
                                onPressed: {
                                    configurationViewModel.deleteMapping(row);
                                }
                            }
                        }

                        Row {
                            width: parent.width - 10
                            spacing: 2
                            height: 30
                            anchors.verticalCenter: parent.verticalCenter
                            visible: column === 2 && isEditing

                            IconButton {
                                icon: storagePaths.icons + "save.svg"
                                width: 26
                                height: 30
                                iconWidth: 22
                                iconHeight: 22
                                onPressed: {
                                    configurationViewModel.editMapping(identifier);
                                }
                            }

                            IconButton {
                                icon: storagePaths.icons + "cancel.svg"
                                width: 26
                                height: 30
                                iconWidth: 22
                                iconHeight: 22
                                onPressed: {
                                    configurationViewModel.mappingListModel.disableEditing(row);
                                }
                            }
                        }

                        VerticalBorder {
                            anchors.bottom: parent.bottom
                        }
                    }
                    onWidthChanged: mappingsTable.forceLayout()
                }

                EmptyList {
                    visible: !configurationViewModel.mappingListModel.isHasMappings
                    title: "You did not have mappings yet.\nPlease click on Add button."
                }
            }
        }
    }

    FileDialog {
        id: saveConfigurationDialog
        modality: Qt.WindowModal
        title: `Open Yaml confguration file`
        fileMode: FileDialog.SaveFile
        nameFilters: [ `Yaml files (*.yml)` ]
        onAccepted: {
            configurationViewModel.saveConfiguration(false, selectedFile);
        }
        onRejected: { console.log("Rejected") }
    }
}
