import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: aboutDialog
    title: qsTr("About MetaWiper")
    modal: true
    standardButtons: Dialog.Close
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: 500
    height: 400

    property var appInfo: ({})

    contentItem: Item {
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 20

            // 应用图标和标题
            RowLayout {
                Layout.fillWidth: true
                spacing: 20

                Image {
                    source: "qrc:/icons/logo.svg"
                    width: 80
                    height: 80
                    Layout.alignment: Qt.AlignTop
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: appInfo.title
                        font.pixelSize: 24
                        font.bold: true
                    }

                    Label {
                        text: qsTr("Version ") + appInfo.version
                        font.pixelSize: 14
                    }

                    Label {
                        text: appInfo.description
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
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                ColumnLayout {
                    width: aboutDialog.width - 50
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

                    Item { height: 20 }

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

                    Item { height: 20 }

                    Label {
                        text: appInfo.copyright
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignCenter
                        font.pixelSize: 12
                    }
                }
            }
        }
    }
}