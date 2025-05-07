/**
 * need to be checked
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    // 属性声明
    property bool isProcessing: false
    property bool hasSelection: false

    // 信号声明
    signal refreshClicked()
    signal saveClicked()
    signal cleanMetadataClicked()

    // 面板样式
    color: "#f5f5f5"
    border.color: "#e0e0e0"
    border.width: 1
    radius: 4

    // 主布局
    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 12

        // 刷新按钮
        Button {
            id: refreshButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 40

            enabled: !isProcessing

            ToolTip.visible: hovered
            ToolTip.text: "刷新文件列表"

            background: Rectangle {
                color: refreshButton.hovered ? "#e0e0e0" : "transparent"
                radius: width / 2
            }

            contentItem: Image {
                source: "qrc:/icons/refresh.svg"
                sourceSize.width: 24
                sourceSize.height: 24
            }

            onClicked: root.refreshClicked()
        }

        // 保存按钮
        Button {
            id: saveButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 40

            enabled: !isProcessing && hasSelection

            ToolTip.visible: hovered
            ToolTip.text: "保存修改后的文件"

            background: Rectangle {
                color: saveButton.hovered ? "#e0e0e0" : "transparent"
                radius: width / 2
            }

            contentItem: Image {
                source: "qrc:/icons/save.svg"
                sourceSize.width: 24
                sourceSize.height: 24
            }

            onClicked: root.saveClicked()
        }

        // 间隔
        Item {
            Layout.fillWidth: true
        }

        // 清除元数据按钮
        Button {
            id: cleanButton
            Layout.preferredHeight: 40

            enabled: !isProcessing && hasSelection
            text: "清除元数据"

            background: Rectangle {
                color: cleanButton.hovered ? "#2979ff" : "#2196f3"
                opacity: cleanButton.enabled ? 1.0 : 0.5
                radius: 4
            }

            contentItem: Text {
                text: cleanButton.text
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: root.cleanMetadataClicked()
        }

        // 处理中指示器
        BusyIndicator {
            visible: isProcessing
            running: isProcessing
            Layout.preferredWidth: 32
            Layout.preferredHeight: 32
        }
    }
}