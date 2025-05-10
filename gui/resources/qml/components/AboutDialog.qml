import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: aboutDialog
    title: qsTr("About MetaWiper")
    modal: true
    standardButtons: Dialog.Close

    // centering the dialog
    anchors.centerIn: parent
    width: 500
    height: 400

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
        anchors.fill: parent
        contentWidth: columnLayout.width

        ColumnLayout {
            id: columnLayout
            width: aboutDialog.width - 40
            spacing: 20
            anchors.margins: 20

            // 应用图标和标题
            RowLayout {
                Layout.fillWidth: true
                spacing: 20

                Image {
                    source: "qrc:/icons/logo.svg"
                    sourceSize.width: 80
                    sourceSize.height: 80
                    Layout.alignment: Qt.AlignTop
                }

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

                Item { height: 20; width: 1 }

                Label {
                    text: aboutDialog.appInfo.copyright || ""
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 12
                    Layout.topMargin: 10
                }
            }
        }
    }
}