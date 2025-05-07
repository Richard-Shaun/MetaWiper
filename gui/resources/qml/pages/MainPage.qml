/**
 * need to be checked
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"
import "../theme"

Page {
    id: mainPage

    background: Rectangle {
        color: "white"
    }

    // 主内容分割视图
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // 操作面板
        ActionPanel {
            id: actionPanel
            Layout.fillWidth: true
            height: 60
            isProcessing: app.processing
            hasSelection: fileListModel.count > 0

            onRefreshClicked: {
                app.refreshFiles()
            }

            onSaveClicked: {
                app.saveModifiedFiles()
            }

            onCleanMetadataClicked: {
                confirmDialog.title = qsTr("清除元数据")
                confirmDialog.text = qsTr("这将永久删除所选文件的元数据。是否继续？")
                confirmDialog.operation = "clean"
                confirmDialog.options = {}
                confirmDialog.open()
            }
        }

        // 主内容分割视图
        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: Qt.Horizontal

            // 左侧文件列表
            Rectangle {
                SplitView.preferredWidth: 300
                SplitView.minimumWidth: 200
                color: "#F5F5F5"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 8

                    Label {
                        text: qsTr("文件")
                        font.bold: true
                        font.pixelSize: 16
                    }

                    // 文件列表组件
                    FileList {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: fileListModel
                    }
                }
            }

            // 右侧元数据视图
            Rectangle {
                SplitView.fillWidth: true
                color: "#FFFFFF"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 8

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: qsTr("元数据: ") + (fileListModel.currentFile ? fileListModel.currentFile.split('/').pop() : "")
                            font.bold: true
                            font.pixelSize: 16
                        }

                        Item { Layout.fillWidth: true }

                        TextField {
                            id: searchField
                            placeholderText: qsTr("搜索元数据...")
                            Layout.preferredWidth: 200

                            onTextChanged: {
                                metadataModel.filterMetadata(text)
                            }
                        }
                    }

                    // 元数据视图组件
                    MetadataView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: metadataModel
                    }
                }
            }
        }

        // 底部状态栏
        Rectangle {
            Layout.fillWidth: true
            height: 30
            color: "#F0F0F0"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: 8

                Label {
                    text: mainViewModel.statusMessage
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("文件: ") + fileListModel.count
                }

                Label {
                    text: qsTr("元数据: ") + metadataModel.count
                    Layout.rightMargin: 8
                }
            }
        }
    }

    // 确认对话框
    Dialog {
        id: confirmDialog
        title: qsTr("确认")
        modal: true
        standardButtons: Dialog.Yes | Dialog.No
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 400

        property string operation: ""
        property var options: ({})
        property string text: ""

        Label {
            text: confirmDialog.text
            wrapMode: Text.Wrap
            width: parent.width
        }

        onAccepted: {
            app.processFiles(operation, options)
        }
    }
}