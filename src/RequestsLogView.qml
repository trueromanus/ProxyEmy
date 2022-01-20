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
            height: 30
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
                return requestsLogViewModel.listModel.getColumnWidth(column, aliasesTable.width);
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

                    ToolTip.delay: 1000
                    ToolTip.visible: linkTooltip && hoveredLink
                    ToolTip.text: linkTooltip
                }

                Rectangle {
                    anchors.bottom: parent.bottom
                    color: "#e7eaec"
                    width: parent.width
                    height: 1
                }
            }
            onWidthChanged: requestsLogTable.forceLayout()
        }

    }
}
