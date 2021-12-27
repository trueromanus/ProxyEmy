import QtQuick

Item {
    anchors.centerIn: parent
    width: parent.width
    height: 90

    property alias title: emptyMessage.text

    Image {
        id: emptyboxImage
        anchors.horizontalCenter: parent.horizontalCenter
        source: storagePaths.icons + "emptybox.svg"
        width: 50
        height: 50
        mipmap: true
    }
    Text {
        id: emptyMessage
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: emptyboxImage.bottom
        horizontalAlignment: Text.AlignHCenter
        text: "You did not have mappings yet.\nPlease click on Add button."
        font.pointSize: 9
    }
}
