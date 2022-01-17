import QtQuick

Item {
    id: root
    height: 30
    width: parent.width

    Row {
        anchors.fill: parent
        anchors.leftMargin: 8
        spacing: 2

        Repeater {
            model: backend.tabs
            delegate: Item {
                width: 120
                height: root.height

                Item {
                    clip: true
                    anchors.fill: parent

                    Rectangle {
                        color: "white"
                        width: parent.width
                        height: parent.height + 2
                        anchors.leftMargin: 2
                        anchors.rightMargin: 2
                        border.width: 1
                        border.color: isActived ? "#e7eaec" : "transparent"
                        radius: 4
                    }
                }

                Text {
                    anchors.centerIn: parent
                    font.pointSize: 10
                    text: title
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        if (isActived) return;

                        backend.tabs.activateTab(identifier);
                    }
                }
            }
        }
    }

}
