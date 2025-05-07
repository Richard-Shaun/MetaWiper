/**
 * @file mainviewmodel.h
 * @brief Main view model class
 */
#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>

/**
 * @class MainViewModel
 * @brief Provides data and logic for the main interface to QML views
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
     * @brief Constructor
     * @param parent Parent QObject
     */
    explicit MainViewModel(QObject *parent = nullptr);

    /**
     * @brief Get application title
     * @return Application title
     */
    QString getApplicationTitle() const;

    /**
     * @brief Get version number
     * @return Version number
     */
    QString getVersion() const;

    /**
     * @brief Get status message
     * @return Status message
     */
    QString getStatusMessage() const;

    /**
     * @brief Set status message
     * @param message Status message
     */
    void setStatusMessage(const QString &message);

    /**
     * @brief Get processing state
     * @return true if processing, false otherwise
     */
    bool isProcessing() const;

    /**
     * @brief Set processing state
     * @param processing Processing state
     */
    void setProcessing(bool processing);

    /**
     * @brief Get active tab
     * @return Active tab index
     */
    int getActiveTab() const;

    /**
     * @brief Set active tab
     * @param tab Tab index
     */
    void setActiveTab(int tab);

public slots:
    /**
     * @brief Get application information
     * @return Map containing application information
     */
    Q_INVOKABLE QVariantMap getAppInfo() const;

    /**
     * @brief Handle operation completed event
     * @param success Whether operation was successful
     * @param message Operation message
     */
    void handleOperationCompleted(bool success, const QString &message);

signals:
    /**
     * @brief Signal emitted when status message changes
     */
    void statusMessageChanged();

    /**
     * @brief Signal emitted when processing state changes
     */
    void processingChanged();

    /**
     * @brief Signal emitted when active tab changes
     */
    void activeTabChanged();

private:
    QString m_statusMessage;
    bool m_processing;
    int m_activeTab;
};