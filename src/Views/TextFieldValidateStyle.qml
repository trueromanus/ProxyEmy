import QtQuick

Rectangle {
    border.width: 2
    radius: 2
    border.color: isNotValid ? "#ED5565" : (parent.focus ? "#1ab394" : "#e7eaec")

    property bool isNotValid: false
}
