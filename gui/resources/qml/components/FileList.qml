import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property var model

    ColumnLayout {
        anchors.fill: parent
        spacing: 6

        // 工具栏
        RowLayout {
            Layout.fillWidth: true
            spacing: 4

            Button {
                text: qsTr("Add")
                icon.source: "qrc:/icons/add.svg"
                onClicked: app.selectFiles()
            }

            Button {
                text: qsTr("Remove")
                icon.source: "qrc:/icons/remove.svg"
                enabled: listView.currentIndex >= 0
                onClicked: {
                    model.removeFile(listView.currentIndex)
                }
            }

            Button {
                text: qsTr("Clear")
                enabled: model.count > 0
                onClicked: {
                    confirmClearDialog.open()
                }
            }

            Item { Layout.fillWidth: true }
        }

        // 文件列表视图
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: root.model

            ScrollBar.vertical: ScrollBar {
                active: true
            }

            // 空状态提示
            Rectangle {
                anchors.fill: parent
                color: "#F8F8F8"
                visible: listView.count === 0

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 10

                    Image {
                        source: "qrc:/icons/add.svg"
                        Layout.alignment: Qt.AlignHCenter
                        width: 48
                        height: 48
                    }

                    Label {
                        text: qsTr("No files added yet")
                        font.pixelSize: 16
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Button {
                        text: qsTr("Add Files")
                        Layout.alignment: Qt.AlignHCenter
                        onClicked: app.selectFiles()
                    }
                }
            }

            delegate: Rectangle {
                id: delegateItem
                width: listView.width
                height: 64
                color: model.isSelected ? "#E3F2FD" : (index % 2 === 0 ? "#FFFFFF" : "#F5F5F5")

                // 文件信息布局
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    // 文件图标（可以根据扩展名显示不同图标）
                    Rectangle {
                        width: 40
                        height: 40
                        radius: 4
                        color: {
                            if (model.fileExtension === "pdf") return "#F44336";
                            if (model.fileExtension === "jpg" || model.fileExtension === "jpeg") return "#4CAF50";
                            return "#2196F3";
                        }

                        Label {
                            anchors.centerIn: parent
                            text: model.fileExtension ? model.fileExtension.toUpperCase() : "?"
                            color: "white"
                            font.bold: true
                        }
                    }

                    // 文件信息
                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 2

                        Label {
                            text: model.fileName
                            font.bold: true
                            elide: Text.ElideMiddle
                            Layout.fillWidth: true
                        }

                        Label {
                            text: {
                                // 格式化文件大小
                                var size = model.fileSize;
                                if (size < 1024) return size + " B";
                                if (size < 1024 * 1024) return Math.round(size / 1024 * 10) / 10 + " KB";
                                return Math.round(size / (1024 * 1024) * 10) / 10 + " MB";
                            }
                            font.pixelSize: 12
                            color: "#757575"
                        }

                        Label {
                            text: model.lastModified
                            font.pixelSize: 12
                            color: "#757575"
                        }
                    }
                }

                // 鼠标悬停效果
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: {
                        if (!model.isSelected) {
                            delegateItem.color = "#EEEEEE"
                        }
                    }

                    onExited: {
                        if (!model.isSelected) {
                            delegateItem.color = index % 2 === 0 ? "#FFFFFF" : "#F5F5F5"
                        }
                    }

                    onClicked: {
                        listView.currentIndex = index
                        root.model.selectFile(index)
                    }
                }
            }
        }
    }

    // 确认清空文件列表对话框
    Dialog {
        id: confirmClearDialog
        title: qsTr("Clear Files")
        modal: true
        standardButtons: Dialog.Yes | Dialog.No
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 400

        Label {
            text: qsTr("Remove all files from the list?")
            wrapMode: Text.Wrap
            width: parent.width
        }

        onAccepted: {
            model.clearFiles()
        }
    }
}