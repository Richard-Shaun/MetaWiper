/**
* @file application.cpp
* @brief MetaWiper GUI main application class implementation
*/
#include "application.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QFileDialog>
#include <QUrl>
#include <QDebug>
#include <QString>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

Application::Application(QObject *parent)
    : QObject(parent),
      m_coreInstance(std::make_unique<meta_wiper_core::meta_wiper_core_class>()),
      m_fileListModel(std::make_unique<FileListModel>()),
      m_metadataModel(std::make_unique<MetadataModel>()),
      m_mainViewModel(std::make_unique<MainViewModel>()),
      m_processing(false)
{
    // Connect signals and slots
    connect(m_fileListModel.get(), &FileListModel::fileSelected,
            this, [this](const QString& filePath) {
                // When a file is selected, read its metadata
                QVariantMap options;
                processFiles("read", options);
            });
}

Application::~Application() = default;

void Application::registerViewModels()
{
    // Register custom types to QML
    qmlRegisterType<FileListModel>("MetaWiper", 1, 0, "FileListModel");
    qmlRegisterType<MetadataModel>("MetaWiper", 1, 0, "MetadataModel");
    qmlRegisterType<MainViewModel>("MetaWiper", 1, 0, "MainViewModel");

    // Set object ownership for model instances
    QQmlEngine::setObjectOwnership(m_fileListModel.get(), QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(m_metadataModel.get(), QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(m_mainViewModel.get(), QQmlEngine::CppOwnership);
}

QStringList Application::getSupportedFileTypes() const
{
    QStringList types;
    for (const auto& type : m_coreInstance->get_supported_file_types()) {
        types.append(QString::fromStdString(type));
    }
    return types;
}

bool Application::isProcessing() const
{
    return m_processing;
}

void Application::selectFiles()
{
    // Create file selection dialog
    QStringList supportedTypes = getSupportedFileTypes();
    QString filters = "Supported Files (";
    for (const auto& type : supportedTypes) {
        filters += "*" + type + " ";
    }
    filters.chop(1); // Remove last space
    filters += ");;All Files (*)";

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(filters);
    dialog.setViewMode(QFileDialog::Detail);

    if (dialog.exec()) {
        QStringList files = dialog.selectedFiles();
        // Modified: Use addFile instead of setFiles to allow multiple files
        for (const auto& file : files) {
            m_fileListModel->addFile(file);
        }
    }
}

QUrl Application::selectOutputDirectory()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    if (dialog.exec()) {
        QStringList dirs = dialog.selectedFiles();
        if (!dirs.isEmpty()) {
            return QUrl::fromLocalFile(dirs.first());
        }
    }

    return QUrl();
}

void Application::processFiles(const QString& operation, const QVariantMap& options)
{
    // Check if there are files to process
    QStringList files = m_fileListModel->getSelectedFiles();
    if (files.isEmpty()) {
        emit operationCompleted(false, "No files selected for processing");
        return;
    }

    // Set processing state
    m_processing = true;
    emit processingChanged();

    // Notify MainViewModel of processing status
    m_mainViewModel->setProcessing(true);

    // Determine operation type
    file_handler::operation_type op_type;
    if (operation == "read") {
        op_type = file_handler::operation_type::READ;
    } else if (operation == "clean") {
        op_type = file_handler::operation_type::CLEAN;
    } else if (operation == "overwrite") {
        op_type = file_handler::operation_type::OVERWRITE;
    } else if (operation == "export") {
        op_type = file_handler::operation_type::EXPORT;
    } else if (operation == "restore") {
        op_type = file_handler::operation_type::RESTORE;
    } else {
        m_processing = false;
        m_mainViewModel->setProcessing(false);
        emit processingChanged();
        emit operationCompleted(false, "Unknown operation: " + operation);
        return;
    }

    // Create operation options
    file_handler::operation_options op_options;

    // If output directory is specified
    if (options.contains("outputDirectory")) {
        QUrl outputDir = options["outputDirectory"].toUrl();
        if (outputDir.isValid() && outputDir.isLocalFile()) {
            op_options.output_directory = outputDir.toLocalFile().toStdString();
        }
    }

    // If metadata to overwrite is provided
    if (options.contains("metadata") && options["metadata"].type() == QVariant::Map) {
        QVariantMap metadata = options["metadata"].toMap();
        for (auto it = metadata.begin(); it != metadata.end(); ++it) {
            op_options.overwrite_metadata[it.key().toStdString()] = it.value().toString().toStdString();
        }
    }

    // Process files in another thread
    QFuture<void> future = QtConcurrent::run([this, files, op_type, op_options]() {
        std::vector<std::string> filePaths;
        for (const auto& file : files) {
            filePaths.push_back(file.toStdString());
        }

        auto results = m_coreInstance->process_files(filePaths, op_type, op_options);

        // Process results
        bool allSuccess = true;
        QString message;

        for (size_t i = 0; i < results.size(); ++i) {
            const auto& result = results[i];
            if (!result.success) {
                allSuccess = false;
                message += QString::fromStdString(filePaths[i]) + ": " +
                          QString::fromStdString(result.message) + "\n";
            }

            // If this is a read operation and it's the currently selected file, update metadata model
            if (op_type == file_handler::operation_type::READ &&
                !files.isEmpty() &&
                QString::fromStdString(filePaths[i]) == m_fileListModel->getCurrentFile()) {
                QVariantMap metadata;
                for (const auto& [key, value] : result.metadata) {
                    metadata[QString::fromStdString(key)] = QString::fromStdString(value);
                }

                // UI updates must be done on the main thread
                QMetaObject::invokeMethod(m_metadataModel.get(), "setMetadata",
                                         Qt::QueuedConnection,
                                         Q_ARG(QVariantMap, metadata));
            }
        }

        if (allSuccess) {
            message = "Operation completed successfully";
        } else if (message.isEmpty()) {
            message = "Operation failed";
        }

        // Finish processing, update status
        QMetaObject::invokeMethod(this, [this, allSuccess, message]() {
            m_processing = false;
            m_mainViewModel->setProcessing(false);
            emit processingChanged();
            emit operationCompleted(allSuccess, message);
        }, Qt::QueuedConnection);
    });
}

bool Application::isFileTypeSupported(const QString& fileType)
{
    std::string type = fileType.toStdString();
    return m_coreInstance->type_supported(type);
}