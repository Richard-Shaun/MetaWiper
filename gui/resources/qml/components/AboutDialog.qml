import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: aboutDialog
    title: qsTr("About MetaWiper")
    modal: true
    standardButtons: Dialog.Close
    closePolicy: Dialog.CloseOnEscape | Dialog.CloseOnPressOutside

    // 固定宽高而不使用锚点
    width: parent.width/2.3
    height: parent.height/2

    // 居中显示
    x: (parent ? (parent.width - width) / 2 : 0)
    y: (parent ? (parent.height - height) / 2 : 0)

    // 确保appInfo对象有默认值，防止初始未设置时出错
    property var appInfo: ({
        title: "MetaWiper",
        version: "1.0.0",
        description: "A tool for viewing and cleaning file metadata",
        copyright: "© 2025 MetaWiper"
    })

    contentItem: ScrollView {
        id: scrollView
        clip: true
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        padding: 5  // 添加内边距来替代锚点设置

        ColumnLayout {
            id: columnLayout
            width: scrollView.width - 20
            spacing: 20
            anchors.left: parent.left
            anchors.leftMargin: 10

            // 应用图标和标题
            RowLayout {
                Layout.fillWidth: true
                spacing: 20

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: aboutDialog.appInfo.title || ""
                        font.pixelSize: 24
                        font.bold: true
                    }

                    Label {
                        text: qsTr("Version ") + (aboutDialog.appInfo.version || "")
                        font.pixelSize: 14
                    }

                    Label {
                        text: aboutDialog.appInfo.description || ""
                        wrapMode: Text.Wrap
                        Layout.fillWidth: true
                    }
                }
            }

            // 分隔线
            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#E0E0E0"
            }

            // 软件特性介绍
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 10

                Label {
                    text: qsTr("Features")
                    font.bold: true
                    font.pixelSize: 16
                }

                Label {
                    text: qsTr("• View metadata from various file types, including PDF, JPEG, etc.")
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("• Clean metadata to protect your privacy")
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("• Export metadata to JSON format")
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("• Overwrite metadata with custom values")
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }

                Item { height: 20; width: 1 }

                Label {
                    text: qsTr("Technologies")
                    font.bold: true
                    font.pixelSize: 16
                }

                Label {
                    text: qsTr("• Qt 6.8.3 with QML and Material Design")
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("• C++17 language standard")
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("• PoDoFo 0.10.4 for PDF metadata handling")
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("• Exiv2 for image metadata handling")
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }

                Item {
                    height: 20
                    width: 1
                    Layout.fillWidth: true
                }

                Label {
                    text: aboutDialog.appInfo.copyright || ""
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 12
                    Layout.topMargin: 10
                    opacity: 0.7
                }
            }
        }
    }
}