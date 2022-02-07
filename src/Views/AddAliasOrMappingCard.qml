import QtQuick
import QtQuick.Controls
import "Controls"

ContentPopup {
    id: addCard
    visible: addOptionsCardViewModel.opened
    width: 340
    height: 180

    Item {
        anchors.fill: parent
        anchors.margins: 10

        Text {
            id: nameFieldLabel
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 2
            font.pointSize: 10
            text: addOptionsCardViewModel.nameFieldTitle
        }

        Text {
            anchors.left: nameFieldLabel.right
            anchors.leftMargin: 2
            anchors.top: nameFieldLabel.top
            text: addOptionsCardViewModel.nameFieldErrorMessage
            font.pointSize: 10
            color: "#ED5565"
        }

        TextField {
            id: nameTextField
            anchors.top: nameFieldLabel.bottom
            anchors.topMargin: 4
            width: parent.width
            text: addOptionsCardViewModel.nameField
            background: TextFieldValidateStyle {
                isNotValid: addOptionsCardViewModel.nameFieldErrorMessage
            }
            onTextChanged: {
                addOptionsCardViewModel.nameField = text;
            }
        }

        Text {
            id: valueFieldLabel
            anchors.top: nameTextField.bottom
            anchors.left: parent.left
            anchors.leftMargin: 2
            font.pointSize: 10
            text: addOptionsCardViewModel.valueFieldTitle
        }

        Text {
            anchors.left: valueFieldLabel.right
            anchors.leftMargin: 2
            anchors.top: valueFieldLabel.top
            text: addOptionsCardViewModel.valueFieldErrorMessage
            font.pointSize: 10
            color: "#ED5565"
        }

        TextField {
            id: valueTextField
            anchors.top: valueFieldLabel.bottom
            anchors.topMargin: 4
            width: parent.width
            text: addOptionsCardViewModel.valueField
            background: TextFieldValidateStyle {
                isNotValid: addOptionsCardViewModel.valueFieldErrorMessage
            }
            onTextChanged: {
                addOptionsCardViewModel.valueField = text;
            }
        }

        TextButton {
            disabled: !addOptionsCardViewModel.isValid
            width: 80
            anchors.bottom: parent.bottom
            anchors.right: cancelButton.left
            anchors.rightMargin: 8
            title: "Save"
            onPressed: {
                addOptionsCardViewModel.saveCard();
            }
        }

        TextButton {
            id: cancelButton
            width: 80
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            title: "Cancel"
            onPressed: {
                addOptionsCardViewModel.closeCard();
            }
        }
    }
}
