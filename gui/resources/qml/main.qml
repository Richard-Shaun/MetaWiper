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
    title: mainViewModel ? (mainViewModel.applicationTitle + " v" + mainViewModel.version) : "MetaWiper"

    // Material theme settings
    Material.theme: Material.Light
    Material.accent: Material.Blue
    Material.primary: Material.Indigo

    // Top menu bar
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
                enabled: fileListModel && fileListModel.count > 0 && !app.processing
                onTriggered: app.processFiles("read")
            }
            Action {
                text: qsTr("Clean Metadata")
                enabled: fileListModel && fileListModel.count > 0 && !app.processing
                onTriggered: {
                    // Show confirmation dialog
                    confirmDialog.title = qsTr("Clean Metadata")
                    confirmDialog.text = qsTr("This will permanently remove metadata from the selected file. Continue?")
                    confirmDialog.operation = "clean"
                    confirmDialog.options = {}
                    confirmDialog.open()
                }
            }
            // Add new Overwrite action
            Action {
                text: qsTr("Overwrite Metadata")
                enabled: fileListModel && fileListModel.count > 0 && !app.processing
                onTriggered: {
                    overwriteDialog.open()
                }
            }
            Action {
                text: qsTr("Export Metadata")
                enabled: fileListModel && fileListModel.count > 0 && !app.processing
                onTriggered: {
                    // Select output directory
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

    // Main content area
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Top toolbar
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

                // 添加刷新按钮到工具栏
                /*ToolButton {
                    icon.source: "qrc:/icons/refresh.svg"
                    text: qsTr("Refresh")
                    display: AbstractButton.TextUnderIcon
                    enabled: fileListModel && fileListModel.count > 0 && !app.processing
                    onClicked: app.processFiles("read")
                }*/

                ToolButton {
                    icon.source: "qrc:/icons/clean.svg"
                    text: qsTr("Clean")
                    display: AbstractButton.TextUnderIcon
                    enabled: fileListModel && fileListModel.count > 0 && !app.processing
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
                    enabled: fileListModel && fileListModel.count > 0 && !app.processing
                    onClicked: {
                        var outputDir = app.selectOutputDirectory()
                        if (outputDir != "") {
                            var options = {"outputDirectory": outputDir}
                            app.processFiles("export", options)
                        }
                    }
                }

                // Add Overwrite button to toolbar
                ToolButton {
                    icon.source: "qrc:/icons/overwrite.svg"
                    text: qsTr("Overwrite")
                    display: AbstractButton.TextUnderIcon
                    enabled: fileListModel && fileListModel.count > 0 && !app.processing
                    onClicked: {
                        overwriteDialog.open()
                    }
                }

                Item { Layout.fillWidth: true }

                BusyIndicator {
                    visible: app.processing
                    running: app.processing
                }


                ToolButton {
                    icon.source: "qrc:/icons/settings.svg"
                    text: qsTr("Settings")
                    display: AbstractButton.TextUnderIcon
                    onClicked: settingsDialog.open()
                }


                ToolButton {
                    icon.source: "qrc:/icons/info.svg"
                    text: qsTr("About")
                    display: AbstractButton.TextUnderIcon
                    onClicked: aboutDialog.open()
                }
            }
        }

        // Main content split view
        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: Qt.Horizontal

            // Left side file list
            Rectangle {
                SplitView.preferredWidth: 300
                SplitView.minimumWidth: 250
                color: "#F5F5F5"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 8

                    Label {
                        text: qsTr("Files")
                        font.bold: true
                        font.pixelSize: 16
                    }

                    // File list component
                    FileList {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: fileListModel
                    }
                }
            }

            // Right side metadata view
            Rectangle {
                SplitView.fillWidth: true
                color: "#FFFFFF"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 8

                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: qsTr("Metadata: ") + (fileListModel && fileListModel.currentFile ? fileListModel.currentFile.split('/').pop() : "")
                            font.bold: true
                            font.pixelSize: 16
                        }

                        Item { Layout.fillWidth: true }

                        TextField {
                            id: searchField
                            placeholderText: qsTr("Search metadata...")
                            Layout.preferredWidth: 200

                            onTextChanged: {
                                if (metadataModel) {
                                    metadataModel.filterMetadata(text)
                                }
                            }
                        }
                    }

                    // Metadata view component
                    MetadataView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: metadataModel

                        // 添加连接刷新信号
                        onRefreshRequested: {
                            app.processFiles("read")
                        }
                    }
                }
            }
        }

        // Bottom status bar
        Rectangle {
            Layout.fillWidth: true
            height: 30
            color: "#F0F0F0"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: 8

                Label {
                    text: mainViewModel ? mainViewModel.statusMessage : "Ready"
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("Files: ") + (fileListModel ? fileListModel.count : 0)
                }

                Label {
                    text: qsTr("Metadata: ") + (metadataModel ? metadataModel.count : 0)
                    Layout.rightMargin: 8
                }
            }
        }
    }

    // Confirmation dialog
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

    SettingsDialog {
        id: settingsDialog
    }

    // About dialog component
    AboutDialog {
        id: aboutDialog
    }

    // Overwrite dialog component
    OverwriteDialog {
        id: overwriteDialog

        onMetadataSubmitted: function(metadata) {
            var options = { "metadata": metadata }
            app.processFiles("overwrite", options)
        }
    }

    // Connect signals and slots
    Connections {
        target: app

        function onOperationCompleted(success, message) {
            if (mainViewModel) {
                mainViewModel.handleOperationCompleted(success, message)
            }

            // If operation is successful, show a temporary notification
            if (success) {
                successNotification.text = message
                successNotification.open()
            } else {
                errorNotification.text = message
                errorNotification.open()
            }
        }

        // 移除会导致错误的函数调用
        // function onProcessingChanged() {
        //     if (mainViewModel) {
        //         mainViewModel.setProcessing(app.processing)
        //     }
        // }
    }

    // Success notification
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

    // Error notification
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