/**
 * @file application.cpp
 * @brief MetaWiper GUI主应用程序类实现
 */
#include "application.h"
#include <QQmlEngine>
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
    // 连接信号和槽
    connect(m_fileListModel.get(), &FileListModel::fileSelected,
            this, [this](const QString& filePath) {
                // 当文件被选中时，读取其元数据
                QVariantMap options;
                processFiles("read", options);
            });
}

Application::~Application() = default;

void Application::registerViewModels()
{
    // 注册自定义类型到QML
    qmlRegisterType<FileListModel>("MetaWiper", 1, 0, "FileListModel");
    qmlRegisterType<MetadataModel>("MetaWiper", 1, 0, "MetadataModel");
    qmlRegisterType<MainViewModel>("MetaWiper", 1, 0, "MainViewModel");

    // 将模型实例导出到QML上下文
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
    // 创建文件选择对话框
    QStringList supportedTypes = getSupportedFileTypes();
    QString filters = "Supported Files (";
    for (const auto& type : supportedTypes) {
        filters += "*" + type + " ";
    }
    filters.chop(1); // 移除最后的空格
    filters += ");;All Files (*)";

    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(filters);
    dialog.setViewMode(QFileDialog::Detail);

    if (dialog.exec()) {
        QStringList files = dialog.selectedFiles();
        m_fileListModel->setFiles(files);
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
    // 检查是否有文件可处理
    QStringList files = m_fileListModel->getSelectedFiles();
    if (files.isEmpty()) {
        emit operationCompleted(false, "No files selected for processing");
        return;
    }

    // 设置处理中状态
    m_processing = true;
    emit processingChanged();

    // 确定操作类型
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
        emit processingChanged();
        emit operationCompleted(false, "Unknown operation: " + operation);
        return;
    }

    // 创建操作选项
    file_handler::operation_options op_options;

    // 如果指定了输出目录
    if (options.contains("outputDirectory")) {
        QUrl outputDir = options["outputDirectory"].toUrl();
        if (outputDir.isValid() && outputDir.isLocalFile()) {
            op_options.output_directory = outputDir.toLocalFile().toStdString();
        }
    }

    // 如果提供了要覆盖的元数据
    if (options.contains("metadata") && options["metadata"].type() == QVariant::Map) {
        QVariantMap metadata = options["metadata"].toMap();
        for (auto it = metadata.begin(); it != metadata.end(); ++it) {
            op_options.overwrite_metadata[it.key().toStdString()] = it.value().toString().toStdString();
        }
    }

    // 在另一个线程中处理文件
    QFuture<void> future = QtConcurrent::run([this, files, op_type, op_options]() {
        std::vector<std::string> filePaths;
        for (const auto& file : files) {
            filePaths.push_back(file.toStdString());
        }

        auto results = m_coreInstance->process_files(filePaths, op_type, op_options);

        // 处理结果
        bool allSuccess = true;
        QString message;

        for (size_t i = 0; i < results.size(); ++i) {
            const auto& result = results[i];
            if (!result.success) {
                allSuccess = false;
                message += QString::fromStdString(filePaths[i]) + ": " +
                          QString::fromStdString(result.message) + "\n";
            }

            // 如果是读取操作，并且是当前选中的文件，更新元数据模型
            if (op_type == file_handler::operation_type::READ &&
                !files.isEmpty() &&
                QString::fromStdString(filePaths[i]) == m_fileListModel->getCurrentFile()) {
                QVariantMap metadata;
                for (const auto& [key, value] : result.metadata) {
                    metadata[QString::fromStdString(key)] = QString::fromStdString(value);
                }

                // 更新UI必须在主线程进行
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

        // 完成处理，更新状态
        QMetaObject::invokeMethod(this, [this, allSuccess, message]() {
            m_processing = false;
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