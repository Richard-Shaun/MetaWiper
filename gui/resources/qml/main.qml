import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Window
import MetaWiper 1.0
import "components"

ApplicationWindow {
    id: window
    visible: true
    width: 1200
    height: 800
    title: mainViewModel.applicationTitle + " v" + mainViewModel.version

    // Material 主题设置
    Material.theme: Material.Light
    Material.accent: Material.Blue
    Material.primary: Material.Indigo

    // 获取视图模型实例
    property FileListModel fileListModel: app.getFileListModel()
    property MetadataModel metadataModel: app.getMetadataModel()
    property MainViewModel mainViewModel: app.getMainViewModel()

    // 顶部菜单栏
    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            Action {
                text: qsTr("Open Files...")
                onTriggered: app.selectFiles()
            }
            MenuSeparator {}
            Action {
                text: qsTr("Exit")
                onTriggered: Qt.quit()
            }
        }

        Menu {
            title: qsTr("Operations")
            Action {
                text: qsTr("Read Metadata")
                enabled: fileListModel.count > 0 && !app.processing
                onTriggered: app.processFiles("read")
            }
            Action {
                text: qsTr("Clean Metadata")
                enabled: fileListModel.count > 0 && !app.processing
                onTriggered: {
                    // 显示确认对话框
                    confirmDialog.title = qsTr("Clean Metadata")
                    confirmDialog.text = qsTr("This will permanently remove metadata from the selected file. Continue?")
                    confirmDialog.operation = "clean"
                    confirmDialog.options = {}
                    confirmDialog.open()
                }
            }
            Action {
                text: qsTr("Export Metadata")
                enabled: fileListModel.count > 0 && !app.processing
                onTriggered: {
                    // 选择输出目录
                    var outputDir = app.selectOutputDirectory()
                    if (outputDir != "") {
                        var options = {"outputDirectory": outputDir}
                        app.processFiles("export", options)
                    }
                }
            }
        }

        Menu {
            title: qsTr("Help")
            Action {
                text: qsTr("About MetaWiper")
                onTriggered: aboutDialog.open()
            }
        }
    }

    // 主内容区域
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // 顶部工具栏
        ToolBar {
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                spacing: 8

                ToolButton {
                    icon.source: "qrc:/icons/add.svg"
                    text: qsTr("Add Files")
                    display: AbstractButton.TextUnderIcon
                    onClicked: app.selectFiles()
                }

                ToolButton {
                    icon.source: "qrc:/icons/clean.svg"
                    text: qsTr("Clean")
                    display: AbstractButton.TextUnderIcon
                    enabled: fileListModel.count > 0 && !app.processing
                    onClicked: {
                        confirmDialog.title = qsTr("Clean Metadata")
                        confirmDialog.text = qsTr("This will permanently remove metadata from the selected file. Continue?")
                        confirmDialog.operation = "clean"
                        confirmDialog.options = {}
                        confirmDialog.open()
                    }
                }

                ToolButton {
                    icon.source: "qrc:/icons/export.svg"
                    text: qsTr("Export")
                    display: AbstractButton.TextUnderIcon
                    enabled: fileListModel.count > 0 && !app.processing
                    onClicked: {
                        var outputDir = app.selectOutputDirectory()
                        if (outputDir != "") {
                            var options = {"outputDirectory": outputDir}
                            app.processFiles("export", options)
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                BusyIndicator {
                    visible: app.processing
                    running: app.processing
                }

                ToolButton {
                    icon.source: "qrc:/icons/info.svg"
                    text: qsTr("About")
                    display: AbstractButton.TextUnderIcon
                    onClicked: aboutDialog.open()
                }
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
                        text: qsTr("Files")
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
                            text: qsTr("Metadata: ") + (fileListModel.currentFile ? fileListModel.currentFile.split('/').pop() : "")
                            font.bold: true
                            font.pixelSize: 16
                        }

                        Item { Layout.fillWidth: true }

                        TextField {
                            id: searchField
                            placeholderText: qsTr("Search metadata...")
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
                    text: qsTr("Files: ") + fileListModel.count
                }

                Label {
                    text: qsTr("Metadata: ") + metadataModel.count
                    Layout.rightMargin: 8
                }
            }
        }
    }

    // 确认对话框
    Dialog {
        id: confirmDialog
        title: qsTr("Confirm")
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

    // 关于对话框组件
    AboutDialog {
        id: aboutDialog
        appInfo: mainViewModel.getAppInfo()
    }

    // 连接信号和槽
    Connections {
        target: app

        function onOperationCompleted(success, message) {
            mainViewModel.handleOperationCompleted(success, message)

            // 如果操作成功，显示一个暂时通知
            if (success) {
                successNotification.text = message
                successNotification.open()
            } else {
                errorNotification.text = message
                errorNotification.open()
            }
        }

        function onProcessingChanged() {
            mainViewModel.setProcessing(app.processing)
        }
    }

    // 成功通知
    Popup {
        id: successNotification
        x: (parent.width - width) / 2
        y: parent.height - height - 20
        width: 300
        height: 60
        modal: false
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        property string text: ""

        background: Rectangle {
            color: "#4CAF50"
            radius: 6
        }

        contentItem: RowLayout {
            anchors.fill: parent
            anchors.margins: 10

            Label {
                text: successNotification.text
                color: "white"
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }
        }

        Timer {
            running: successNotification.visible
            interval: 3000
            onTriggered: successNotification.close()
        }
    }

    // 错误通知
    Popup {
        id: errorNotification
        x: (parent.width - width) / 2
        y: parent.height - height - 20
        width: 300
        height: 60
        modal: false
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        property string text: ""

        background: Rectangle {
            color: "#F44336"
            radius: 6
        }

        contentItem: RowLayout {
            anchors.fill: parent
            anchors.margins: 10

            Label {
                text: errorNotification.text
                color: "white"
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }
        }

        Timer {
            running: errorNotification.visible
            interval: 5000
            onTriggered: errorNotification.close()
        }
    }
}