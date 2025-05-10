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
    width: parent.width/2.3
    height: parent.height/1.5

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

        // Initialize with common metadata fields
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

    // Fill in current metadata values when dialog opens
    onOpened: {
        // Try to pre-populate values from current metadata if available
        if (metadataModel && window.metadataModel) {
            var currentFileMetadata = window.metadataModel.metadata

            for (var i = 0; i < metadataModel.count; i++) {
                var item = metadataModel.get(i)
                if (item.key && currentFileMetadata[item.key]) {
                    metadataModel.setProperty(i, "value", currentFileMetadata[item.key])
                }
            }
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
                        Layout.preferredWidth: parent.width * 0.31
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

                // 增加每项之间的间距
                spacing: 4

                // 确保列表视图有适当的内边距
                topMargin: 2
                bottomMargin: 2

                ScrollBar.vertical: ScrollBar {}

                delegate: Rectangle {
                    width: metadataListView.width
                    // 增加每行的高度
                    height: 60
                    color: index % 2 === 0 ? "#f5f5f5" : "#ffffff"

                    RowLayout {
                        anchors.fill: parent
                        // 减小内边距以避免缩小输入框的可见区域
                        anchors.margins: 4
                        spacing: 8

                        // Key field
                        TextField {
                            id: keyField
                            Layout.preferredWidth: parent.width * 0.3
                            text: model.key
                            placeholderText: qsTr("Enter key")
                            onTextChanged: model.key = text
                            // 确保文本垂直居中显示
                            verticalAlignment: TextInput.AlignVCenter
                            // 增加文本左边距
                            leftPadding: 8
                        }

                        // Value field
                        TextField {
                            id: valueField
                            Layout.preferredWidth: parent.width * 0.55
                            text: model.value
                            placeholderText: qsTr("Enter value")
                            onTextChanged: model.value = text
                            // 确保文本垂直居中显示
                            verticalAlignment: TextInput.AlignVCenter
                            // 增加文本左边距
                            leftPadding: 8
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
                // 增加顶部边距，与列表分开
                Layout.topMargin: 4
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