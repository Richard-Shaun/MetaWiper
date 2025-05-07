import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: overwriteDialog
    title: qsTr("Overwrite Metadata")
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: 500
    height: 500

    property var initialMetadata: ({})
    property bool accepted: false

    signal metadataSubmitted(var metadata)

    // Metadata table model
    ListModel {
        id: metadataModel

        // Add empty row
        function addEmptyRow() {
            append({
                key: "",
                value: "",
                isNew: true
            })
        }

        // Initialize with existing metadata
        Component.onCompleted: {
            // Add common editable fields
            append({ key: "Title", value: "", isNew: true })
            append({ key: "Author", value: "", isNew: true })
            append({ key: "Subject", value: "", isNew: true })
            append({ key: "Keywords", value: "", isNew: true })
            append({ key: "Copyright", value: "", isNew: true })

            // Add one empty row for new entries
            addEmptyRow()
        }
    }

    contentItem: Item {
        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            Label {
                text: qsTr("Enter metadata values to overwrite:")
                font.bold: true
            }

            // Table header
            Rectangle {
                Layout.fillWidth: true
                height: 30
                color: "#e0e0e0"

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    Label {
                        text: qsTr("Key")
                        Layout.preferredWidth: parent.width * 0.45
                        horizontalAlignment: Text.AlignHCenter
                        font.bold: true
                    }

                    Rectangle {
                        width: 1
                        Layout.fillHeight: true
                        color: "#bdbdbd"
                    }

                    Label {
                        text: qsTr("Value")
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        font.bold: true
                    }
                }
            }

            // Metadata table
            ListView {
                id: metadataListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: metadataModel
                clip: true

                ScrollBar.vertical: ScrollBar {}

                delegate: Rectangle {
                    width: metadataListView.width
                    height: 40
                    color: index % 2 === 0 ? "#f5f5f5" : "#ffffff"

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 4
                        spacing: 8

                        // Key field
                        TextField {
                            id: keyField
                            Layout.preferredWidth: parent.width * 0.45
                            text: model.key
                            placeholderText: qsTr("Enter key")
                            onTextChanged: model.key = text
                        }

                        // Value field
                        TextField {
                            id: valueField
                            Layout.fillWidth: true
                            text: model.value
                            placeholderText: qsTr("Enter value")
                            onTextChanged: model.value = text
                        }

                        // Delete button
                        Button {
                            icon.source: "qrc:/icons/remove.svg"
                            onClicked: metadataModel.remove(index)
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Remove field")
                        }
                    }
                }
            }

            // Add button
            Button {
                text: qsTr("Add Field")
                icon.source: "qrc:/icons/add.svg"
                Layout.alignment: Qt.AlignRight
                onClicked: metadataModel.addEmptyRow()
            }
        }
    }

    // Handle OK button
    onAccepted: {
        accepted = true

        // Collect metadata
        var metadata = {}
        for (var i = 0; i < metadataModel.count; i++) {
            var item = metadataModel.get(i)
            if (item.key.trim() !== "" && item.value.trim() !== "") {
                metadata[item.key.trim()] = item.value.trim()
            }
        }

        metadataSubmitted(metadata)
    }
}