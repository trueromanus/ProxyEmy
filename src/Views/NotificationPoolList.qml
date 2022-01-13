import QtQuick

Item {
    id: root

    Column {
        anchors.fill: parent
        spacing: 2

        Repeater {
            clip: true
            model: notificationHub.pool
            delegate: Item {
                width: root.width
                height: 80
                Rectangle {
                    id: leftBorder
                    anchors.left: parent.left
                    width: 4
                    height: parent.height
                    color: "#1ab394"
                }

                Rectangle {
                    anchors.top: parent.top
                    anchors.left: leftBorder.right
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 4
                    color: "white"
                }

                Image {
                    id: messageIcon
                    anchors.left: leftBorder.right
                    anchors.leftMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    width: 40
                    height: 40
                    source: storagePaths.icons + (isError ? "error.svg" : "info.svg")
                }

                Column {
                    spacing: 2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: messageIcon.right
                    anchors.leftMargin: 4

                    Text {
                        font.pointSize: 12
                        text: title
                        maximumLineCount: 1
                    }

                    Text {
                        font.pointSize: 10
                        text: message
                        maximumLineCount: 2
                    }
                }

                NumberAnimation on opacity {
                    running: isActiveAnimation
                    from: 1
                    to: 0
                    duration: 300
                    onFinished: {
                        notificationHub.pool.itemAnimationEnded();
                    }
                }
            }
        }
    }
}
