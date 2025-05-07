/**
 * need to be checked
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../theme"

Page {
    id: settingsPage
    title: qsTr("设置")

    background: Rectangle {
        color: "white"
    }

    ScrollView {
        anchors.fill: parent
        anchors.margins: 16
        clip: true

        ColumnLayout {
            width: parent.width
            spacing: 16

            Label {
                text: qsTr("应用设置")
                font.bold: true
                font.pixelSize: 18
            }

            GroupBox {
                title: qsTr("文件处理")
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    CheckBox {
                        id: backupFilesCheck
                        text: qsTr("清除元数据前备份文件")
                        checked: mainViewModel.getSettingValue("backupBeforeCleaning", true)
                        onCheckedChanged: {
                            mainViewModel.saveSettingValue("backupBeforeCleaning", checked)
                        }
                    }

                    CheckBox {
                        id: preserveTimeCheck
                        text: qsTr("保留原始文件时间戳")
                        checked: mainViewModel.getSettingValue("preserveTimestamps", true)
                        onCheckedChanged: {
                            mainViewModel.saveSettingValue("preserveTimestamps", checked)
                        }
                    }

                    CheckBox {
                        id: overwriteCheck
                        text: qsTr("导出时覆盖现有文件")
                        checked: mainViewModel.getSettingValue("overwriteExisting", false)
                        onCheckedChanged: {
                            mainViewModel.saveSettingValue("overwriteExisting", checked)
                        }
                    }
                }
            }

            GroupBox {
                title: qsTr("界面设置")
                Layout.fillWidth: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 8

                    Label {
                        text: qsTr("主题")
                    }

                    ComboBox {
                        id: themeCombo
                        model: [qsTr("系统"), qsTr("浅色"), qsTr("深色")]
                        currentIndex: mainViewModel.getSettingValue("themeMode", 0)
                        Layout.fillWidth: true
                        onCurrentIndexChanged: {
                            mainViewModel.saveSettingValue("themeMode", currentIndex)
                        }
                    }

                    Label {
                        text: qsTr("语言")
                    }

                    ComboBox {
                        id: langCombo
                        model: [qsTr("系统默认"), "English", "简体中文"]
                        currentIndex: mainViewModel.getSettingValue("language", 0)
                        Layout.fillWidth: true
                        onCurrentIndexChanged: {
                            mainViewModel.saveSettingValue("language", currentIndex)
                        }
                    }

                    CheckBox {
                        id: confirmActionsCheck
                        text: qsTr("操作前确认")
                        checked: mainViewModel.getSettingValue("confirmActions", true)
                        onCheckedChanged: {
                            mainViewModel.saveSettingValue("confirmActions", checked)
                        }
                    }
                }
            }

            GroupBox {
                title: qsTr("默认目录")
                Layout.fillWidth: true

                GridLayout {
                    anchors.fill: parent
                    columns: 3
                    rowSpacing: 8
                    columnSpacing: 8

                    Label {
                        text: qsTr("输入目录:")
                    }

                    TextField {
                        id: inputDirField
                        text: mainViewModel.getSettingValue("defaultInputDir", "")
                        Layout.fillWidth: true
                        onEditingFinished: {
                            mainViewModel.saveSettingValue("defaultInputDir", text)
                        }
                    }

                    Button {
                        text: "..."
                        onClicked: {
                            var dir = app.selectDirectory(qsTr("选择默认输入目录"))
                            if (dir != "") {
                                inputDirField.text = dir
                                mainViewModel.saveSettingValue("defaultInputDir", dir)
                            }
                        }
                    }

                    Label {
                        text: qsTr("输出目录:")
                    }

                    TextField {
                        id: outputDirField
                        text: mainViewModel.getSettingValue("defaultOutputDir", "")
                        Layout.fillWidth: true
                        onEditingFinished: {
                            mainViewModel.saveSettingValue("defaultOutputDir", text)
                        }
                    }

                    Button {
                        text: "..."
                        onClicked: {
                            var dir = app.selectDirectory(qsTr("选择默认输出目录"))
                            if (dir != "") {
                                outputDirField.text = dir
                                mainViewModel.saveSettingValue("defaultOutputDir", dir)
                            }
                        }
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }

            RowLayout {
                Layout.fillWidth: true

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    text: qsTr("恢复默认设置")
                    onClicked: {
                        // 显示确认对话框
                        resetConfirmDialog.open()
                    }
                }

                Button {
                    text: qsTr("应用")
                    highlighted: true
                    onClicked: {
                        mainViewModel.applySettings()
                        // 显示设置已保存通知
                        settingsNotification.open()
                    }
                }
            }
        }
    }

    // 重置确认对话框
    Dialog {
        id: resetConfirmDialog
        title: qsTr("恢复默认设置")
        modal: true
        standardButtons: Dialog.Yes | Dialog.No
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 400

        Label {
            text: qsTr("确定要将所有设置恢复为默认值吗？此操作无法撤销。")
            wrapMode: Text.Wrap
            width: parent.width
        }

        onAccepted: {
            mainViewModel.resetSettings()

            // 更新UI控件的状态
            backupFilesCheck.checked = mainViewModel.getSettingValue("backupBeforeCleaning", true)
            preserveTimeCheck.checked = mainViewModel.getSettingValue("preserveTimestamps", true)
            overwriteCheck.checked = mainViewModel.getSettingValue("overwriteExisting", false)
            themeCombo.currentIndex = mainViewModel.getSettingValue("themeMode", 0)
            langCombo.currentIndex = mainViewModel.getSettingValue("language", 0)
            confirmActionsCheck.checked = mainViewModel.getSettingValue("confirmActions", true)
            inputDirField.text = mainViewModel.getSettingValue("defaultInputDir", "")
            outputDirField.text = mainViewModel.getSettingValue("defaultOutputDir", "")

            // 显示重置通知
            settingsNotification.text = qsTr("设置已重置为默认值")
            settingsNotification.open()
        }
    }

    // 设置通知
    Popup {
        id: settingsNotification
        x: (parent.width - width) / 2
        y: parent.height - height - 20
        width: 300
        height: 60
        modal: false
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        property string text: qsTr("设置已保存")

        background: Rectangle {
            color: "#4CAF50"
            radius: 6
        }

        contentItem: RowLayout {
            anchors.fill: parent
            anchors.margins: 10

            Label {
                text: settingsNotification.text
                color: "white"
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }
        }

        Timer {
            running: settingsNotification.visible
            interval: 3000
            onTriggered: settingsNotification.close()
        }
    }
}