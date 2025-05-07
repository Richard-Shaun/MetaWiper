/**
 * @file application.h
 * @brief MetaWiper GUI main application class
 */
#pragma once

#include <QObject>
#include <QUrl>
#include <QStringList>
#include <QQmlApplicationEngine>
#include <memory>
#include <meta_wiper_core.h>
#include "viewmodels/filelistmodel.h"
#include "viewmodels/metadatamodel.h"
#include "viewmodels/mainviewmodel.h"

/**
 * @class Application
 * @brief Main application class, serving as a bridge between GUI and core functionality
 */
class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList supportedFileTypes READ getSupportedFileTypes CONSTANT)
    Q_PROPERTY(bool processing READ isProcessing NOTIFY processingChanged)

public:
    /**
     * @brief Constructor
     * @param parent Parent QObject
     */
    explicit Application(QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~Application() override;

    /**
     * @brief Register view models to QML engine
     */
    void registerViewModels();

    /**
     * @brief Get list of supported file types
     * @return List of supported file extensions
     */
    QStringList getSupportedFileTypes() const;

    /**
     * @brief Check if file processing is in progress
     * @return true if processing, false otherwise
     */
    bool isProcessing() const;

    /**
     * @brief Get file list model
     * @return Pointer to file list model
     */
    Q_INVOKABLE FileListModel* getFileListModel() const { return m_fileListModel.get(); }

    /**
     * @brief Get metadata model
     * @return Pointer to metadata model
     */
    Q_INVOKABLE MetadataModel* getMetadataModel() const { return m_metadataModel.get(); }

    /**
     * @brief Get main view model
     * @return Pointer to main view model
     */
    Q_INVOKABLE MainViewModel* getMainViewModel() const { return m_mainViewModel.get(); }

public slots:
    /**
     * @brief Open file dialog to select files
     */
    void selectFiles();

    /**
     * @brief Select output directory
     * @return URL of selected directory
     */
    QUrl selectOutputDirectory();

    /**
     * @brief Process files
     * @param operation Operation type (read, clean, overwrite, export)
     * @param options Additional operation options
     */
    void processFiles(const QString& operation, const QVariantMap& options = {});

    /**
     * @brief Check if file type is supported
     * @param fileType File type to check
     * @return true if supported, false otherwise
     */
    bool isFileTypeSupported(const QString& fileType);

signals:
    /**
     * @brief Signal emitted when processing state changes
     */
    void processingChanged();

    /**
     * @brief Signal emitted when operation completes
     * @param success Whether the operation was successful
     * @param message Message describing the result
     */
    void operationCompleted(bool success, const QString& message);

private:
    std::unique_ptr<meta_wiper_core::meta_wiper_core_class> m_coreInstance;
    std::unique_ptr<FileListModel> m_fileListModel;
    std::unique_ptr<MetadataModel> m_metadataModel;
    std::unique_ptr<MainViewModel> m_mainViewModel;
    bool m_processing;
};