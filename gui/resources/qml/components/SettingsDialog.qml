import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Dialog {
    id: settingsDialog
    title: "Settings"
    modal: true
    standardButtons: Dialog.Close
    closePolicy: Dialog.CloseOnEscape | Dialog.CloseOnPressOutside

    // 设置对话框尺寸
    width: 500
    height: 500

    // 居中显示
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    // 主题匹配主应用窗口
    Material.accent: window.Material.accent
    Material.primary: window.Material.primary
    Material.theme: window.Material.theme

    Label {
        anchors.centerIn: parent
        text: "Settings options will be implemented in a future version."
        font.pixelSize: 14
        opacity: 0.7
        width: parent.width - 40
        wrapMode: Text.Wrap
        horizontalAlignment: Text.AlignHCenter
    }

    /*
    // 注释掉设置实现，因为没有后端支持
    // 如果将来添加了设置支持，可以取消注释这部分代码

    contentItem: ScrollView {
        id: scrollView
        clip: true
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ColumnLayout {
            width: scrollView.width
            spacing: 16

            Label {
                text: "Application Settings"
                font.bold: true
                font.pixelSize: 16
            }

            GroupBox {
                title: "File Processing"
                Layout.fillWidth: true

                ColumnLayout {
                    width: parent.width
                    spacing: 8

                    CheckBox {
                        id: backupFilesCheck
                        text: "Backup files before cleaning metadata"
                        checked: true
                    }

                    CheckBox {
                        id: preserveTimeCheck
                        text: "Preserve original file timestamps"
                        checked: true
                    }

                    CheckBox {
                        id: overwriteCheck
                        text: "Overwrite existing files when exporting"
                        checked: false
                    }
                }
            }

            GroupBox {
                title: "Interface Settings"
                Layout.fillWidth: true

                ColumnLayout {
                    width: parent.width
                    spacing: 8

                    Label {
                        text: "Theme"
                    }

                    ComboBox {
                        id: themeCombo
                        model: ["System", "Light", "Dark"]
                        currentIndex: 0
                        Layout.fillWidth: true
                    }

                    Label {
                        text: "Language"
                    }

                    ComboBox {
                        id: langCombo
                        model: ["System Default", "English", "Simplified Chinese"]
                        currentIndex: 0
                        Layout.fillWidth: true
                    }

                    CheckBox {
                        id: confirmActionsCheck
                        text: "Confirm before actions"
                        checked: true
                    }
                }
            }

            GroupBox {
                title: "Default Directories"
                Layout.fillWidth: true

                GridLayout {
                    width: parent.width
                    columns: 3
                    rowSpacing: 8
                    columnSpacing: 8

                    Label {
                        text: "Input Directory:"
                    }

                    TextField {
                        id: inputDirField
                        Layout.fillWidth: true
                        placeholderText: "Select input directory"
                    }

                    Button {
                        text: "..."
                        onClicked: {
                            var outputDir = app.selectOutputDirectory()
                            if (outputDir != "") {
                                inputDirField.text = outputDir
                            }
                        }
                    }

                    Label {
                        text: "Output Directory:"
                    }

                    TextField {
                        id: outputDirField
                        Layout.fillWidth: true
                        placeholderText: "Select output directory"
                    }

                    Button {
                        text: "..."
                        onClicked: {
                            var outputDir = app.selectOutputDirectory()
                            if (outputDir != "") {
                                outputDirField.text = outputDir
                            }
                        }
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
    */
}