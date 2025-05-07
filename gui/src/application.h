/**
 * @file application.h
 * @brief MetaWiper GUI主应用程序类
 */
#pragma once

#include <QObject>
#include <QUrl>
#include <QStringList>
#include <memory>
#include <meta_wiper_core.h>
#include "viewmodels/filelistmodel.h"
#include "viewmodels/metadatamodel.h"
#include "viewmodels/mainviewmodel.h"

/**
 * @class Application
 * @brief 主应用程序类，作为GUI和核心功能之间的桥梁
 */
class Application : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList supportedFileTypes READ getSupportedFileTypes CONSTANT)
    Q_PROPERTY(bool processing READ isProcessing NOTIFY processingChanged)

public:
    /**
     * @brief 构造函数
     * @param parent 父QObject
     */
    explicit Application(QObject *parent = nullptr);

    /**
     * @brief 析构函数
     */
    ~Application() override;

    /**
     * @brief 在QML引擎注册视图模型
     */
    void registerViewModels();

    /**
     * @brief 获取支持的文件类型列表
     * @return 支持的文件扩展名列表
     */
    QStringList getSupportedFileTypes() const;

    /**
     * @brief 检查是否正在处理文件
     * @return 如果正在处理则返回true，否则返回false
     */
    bool isProcessing() const;

    /**
     * @brief 获取文件列表模型
     * @return 文件列表模型指针
     */
    FileListModel* getFileListModel() const { return m_fileListModel.get(); }

    /**
     * @brief 获取元数据模型
     * @return 元数据模型指针
     */
    MetadataModel* getMetadataModel() const { return m_metadataModel.get(); }

    /**
     * @brief 获取主视图模型
     * @return 主视图模型指针
     */
    MainViewModel* getMainViewModel() const { return m_mainViewModel.get(); }

public slots:
    /**
     * @brief 打开文件对话框选择文件
     */
    void selectFiles();

    /**
     * @brief 选择输出目录
     * @return 选择的目录URL
     */
    QUrl selectOutputDirectory();

    /**
     * @brief 处理文件
     * @param operation 操作类型 (read, clean, overwrite, export)
     * @param options 操作附加选项
     */
    void processFiles(const QString& operation, const QVariantMap& options = {});

    /**
     * @brief 检查文件类型是否受支持
     * @param fileType 要检查的文件类型
     * @return 如果支持则返回true，否则返回false
     */
    bool isFileTypeSupported(const QString& fileType);

signals:
    /**
     * @brief 处理状态改变时发出的信号
     */
    void processingChanged();

    /**
     * @brief 操作完成时发出的信号
     * @param success 操作是否成功
     * @param message 描述结果的消息
     */
    void operationCompleted(bool success, const QString& message);

private:
    std::unique_ptr<meta_wiper_core::meta_wiper_core_class> m_coreInstance;
    std::unique_ptr<FileListModel> m_fileListModel;
    std::unique_ptr<MetadataModel> m_metadataModel;
    std::unique_ptr<MainViewModel> m_mainViewModel;
    bool m_processing;
};