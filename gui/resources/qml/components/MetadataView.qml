import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property var model

    // 新增信号，用于通知需要刷新元数据
    signal refreshRequested()

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        // table header
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Item { Layout.fillWidth: true }
        }

        // Metadata table view
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            color: "#F8F8F8"

            // Table header
            Rectangle {
                id: tableHeader
                width: parent.width
                height: 35
                z: 2
                color: "#E0E0E0"

                Row {
                    width: parent.width
                    height: parent.height

                    Rectangle {
                        width: parent.width * 0.4
                        height: parent.height
                        color: "#E0E0E0"
                        border.color: "#BDBDBD"

                        Label {
                            anchors.centerIn: parent
                            text: qsTr("Key")
                            font.bold: true
                        }
                    }

                    Rectangle {
                        width: parent.width * 0.6
                        height: parent.height
                        color: "#E0E0E0"
                        border.color: "#BDBDBD"

                        Label {
                            anchors.centerIn: parent
                            text: qsTr("Value")
                            font.bold: true
                        }
                    }
                }
            }

            // Empty state display
            ColumnLayout {
                anchors.centerIn: parent
                spacing: 10
                visible: !(model && model.count > 0)

                Image {
                    source: "qrc:/icons/info.svg"
                    Layout.alignment: Qt.AlignHCenter
                    width: 48
                    height: 48
                }

                Label {
                    text: qsTr("No metadata available")
                    font.pixelSize: 16
                    Layout.alignment: Qt.AlignHCenter
                }

                Label {
                    text: qsTr("Select a file to view its metadata")
                    color: "#757575"
                    visible: !(model && model.count > 0)
                    Layout.alignment: Qt.AlignHCenter
                }
            }

            // List view instead of table view
            ListView {
                id: metadataListView
                anchors.fill: parent
                anchors.topMargin: tableHeader.height
                model: root.model
                clip: true

                ScrollBar.vertical: ScrollBar {
                    active: true
                }

                delegate: Rectangle {
                    width: metadataListView.width
                    height: Math.max(keyText.implicitHeight, valueEdit.implicitHeight) + 16
                    color: index % 2 === 0 ? "#FFFFFF" : "#F5F5F5"

                    Row {
                        anchors.fill: parent

                        // Key column
                        Rectangle {
                            width: parent.width * 0.4
                            height: parent.height
                            color: "transparent"
                            border.color: "#E0E0E0"

                            Label {
                                id: keyText
                                anchors.fill: parent
                                anchors.margins: 8
                                text: model.key || ""
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                                wrapMode: Text.Wrap
                            }
                        }

                        // Value column
                        Rectangle {
                            width: parent.width * 0.6
                            height: parent.height
                            color: "transparent"
                            border.color: "#E0E0E0"

                            TextEdit {
                                id: valueEdit
                                anchors.fill: parent
                                anchors.margins: 8
                                text: model.value || ""
                                wrapMode: Text.Wrap
                                readOnly: !(model.isEditable || false)
                                selectByMouse: true

                                // Copy button appears on hover
                                Item {
                                    id: buttonContainer
                                    anchors.right: parent.right
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 100
                                    height: 35

                                    // 悬停检测区域
                                    HoverHandler {
                                        id: hoverHandler
                                    }

                                    Button {
                                        id: copyButton
                                        anchors.fill: parent
                                        text: qsTr("Copy")
                                        visible: hoverHandler.hovered && !copyTooltip.visible

                                        onClicked: {
                                            valueEdit.selectAll()
                                            valueEdit.copy()
                                            copyTooltip.open()
                                            valueEdit.deselect() // 复制后取消选择
                                        }
                                    }
                                }

                                ToolTip {
                                    id: copyTooltip
                                    text: qsTr("Copied!")
                                    timeout: 1000
                                    visible: false // 确保初始状态为隐藏
                                    y: valueEdit.height / 2
                                    x: buttonContainer.x - 10
                                }

                            }
                        }
                    }
                }
            }
        }

        // Action buttons row
        RowLayout {
            Layout.fillWidth: true
            spacing: 8
            Item { Layout.fillWidth: true }
        }
    }
}