import QtQuick
import QtQuick.Controls
import "Views/Controls"
import "Views"

Item {
    anchors.fill: parent

    BorderedPanel {
        anchors.fill: parent
        anchors.leftMargin: 2
        anchors.rightMargin: 2
        anchors.bottomMargin: 2

        Item {
            id: requestsLogHeader
            width: parent.width
            height: 40

            Switch {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "Enable record logs"
                font.pointSize: 10
                checked: configurationViewModel.isLogRequests
                onCheckedChanged: {
                    configurationViewModel.isLogRequests = checked;
                }
            }
        }

        Item {
            anchors.top: requestsLogHeader.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            visible: !requestsLogViewModel.isHasItems

            EmptyList {
                anchors.centerIn: parent
                title: "You did not have records yet"
            }
        }

        TableView {
            id: requestsLogTable
            anchors.top: requestsLogHeader.bottom
            visible: requestsLogViewModel.isHasItems
            clip: true
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            columnSpacing: 0
            rowSpacing: 1
            model: requestsLogViewModel.listModel
            syncDirection: Qt.Vertical
            ScrollBar.vertical: ScrollBar{
                active: true
            }
            columnWidthProvider: function (column) {
                return requestsLogViewModel.listModel.getColumnWidth(column, requestsLogTable.width);
            }
            delegate: Rectangle {
                id: itemContainer
                implicitWidth: 20
                implicitHeight: 30
                color: "transparent"

                property bool containerHovered: false

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 4
                    width: parent.width - 10
                    wrapMode: Text.WrapAnywhere
                    elide: Qt.ElideRight
                    maximumLineCount: 1
                    text: textValue

                    ToolTip.delay: 1000
                    ToolTip.visible: column !== 0 && itemContainer.containerHovered
                    ToolTip.text: textValue
                }

                Rectangle {
                    anchors.bottom: parent.bottom
                    color: "#e7eaec"
                    width: parent.width
                    height: 1
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        itemContainer.containerHovered = true;
                    }
                    onExited: {
                        itemContainer.containerHovered = false;
                    }
                }
            }
            onWidthChanged: requestsLogTable.forceLayout()
        }

    }
}
