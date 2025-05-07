import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property var model

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        // 元数据表格视图
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            color: "#F8F8F8"

            // 表头
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

            // 显示空状态
            ColumnLayout {
                anchors.centerIn: parent
                spacing: 10
                visible: (root.model ? root.model.count : 0) === 0

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
                    visible: model.count === 0
                    Layout.alignment: Qt.AlignHCenter
                }
            }

            // 列表视图替代表格视图
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

                        // 键列
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

                        // 值列
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

                                // 复制按钮悬停时显示
                                MouseArea {
                                    id: hoverArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    acceptedButtons: Qt.NoButton
                                }

                                Button {
                                    text: qsTr("Copy")
                                    anchors.right: parent.right
                                    anchors.verticalCenter: parent.verticalCenter
                                    visible: hoverArea.containsMouse && !copyTooltip.visible
                                    height: 30
                                    width: 70

                                    onClicked: {
                                        valueEdit.selectAll()
                                        valueEdit.copy()
                                        copyTooltip.open()
                                    }
                                }

                                ToolTip {
                                    id: copyTooltip
                                    text: qsTr("Copied!")
                                    timeout: 1000
                                }
                            }
                        }
                    }
                }
            }
        }

        // 导出按钮
        Button {
            text: qsTr("Export Metadata")
            icon.source: "qrc:/icons/export.svg"
            enabled: model && model.count > 0
            Layout.alignment: Qt.AlignRight

            onClicked: {
                var outputDir = app.selectOutputDirectory()
                if (outputDir != "") {
                    var options = {"outputDirectory": outputDir}
                    app.processFiles("export", options)
                }
            }
        }
    }
}