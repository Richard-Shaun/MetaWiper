/**
 * @file mainviewmodel.h
 * @brief 主视图模型类
 */
#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>

/**
 * @class MainViewModel
 * @brief 提供主界面所需的数据和逻辑给QML视图
 */
class MainViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString applicationTitle READ getApplicationTitle CONSTANT)
    Q_PROPERTY(QString version READ getVersion CONSTANT)
    Q_PROPERTY(QString statusMessage READ getStatusMessage WRITE setStatusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(bool isProcessing READ isProcessing WRITE setProcessing NOTIFY processingChanged)
    Q_PROPERTY(int activeTab READ getActiveTab WRITE setActiveTab NOTIFY activeTabChanged)

public:
    /**
     * @brief 构造函数
     * @param parent 父QObject
     */
    explicit MainViewModel(QObject *parent = nullptr);

    /**
     * @brief 获取应用程序标题
     * @return 应用程序标题
     */
    QString getApplicationTitle() const;

    /**
     * @brief 获取版本号
     * @return 版本号
     */
    QString getVersion() const;

    /**
     * @brief 获取状态消息
     * @return 状态消息
     */
    QString getStatusMessage() const;

    /**
     * @brief 设置状态消息
     * @param message 状态消息
     */
    void setStatusMessage(const QString &message);

    /**
     * @brief 获取处理状态
     * @return 如果正在处理则返回true，否则返回false
     */
    bool isProcessing() const;

    /**
     * @brief 设置处理状态
     * @param processing 处理状态
     */
    void setProcessing(bool processing);

    /**
     * @brief 获取当前活动标签
     * @return 当前活动标签索引
     */
    int getActiveTab() const;

    /**
     * @brief 设置当前活动标签
     * @param tab 标签索引
     */
    void setActiveTab(int tab);

public slots:
    /**
     * @brief 获取应用程序信息
     * @return 包含应用程序信息的映射
     */
    QVariantMap getAppInfo() const;

    /**
     * @brief 处理操作完成事件
     * @param success 操作是否成功
     * @param message 操作消息
     */
    void handleOperationCompleted(bool success, const QString &message);

signals:
    /**
     * @brief 状态消息变化信号
     */
    void statusMessageChanged();

    /**
     * @brief 处理状态变化信号
     */
    void processingChanged();

    /**
     * @brief 活动标签变化信号
     */
    void activeTabChanged();

private:
    QString m_statusMessage;
    bool m_processing;
    int m_activeTab;
};