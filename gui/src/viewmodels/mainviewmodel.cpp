/**
* @file mainviewmodel.cpp
 * @brief 主视图模型类的实现
 */
#include "viewmodels/mainviewmodel.h"
#include <QCoreApplication>

MainViewModel::MainViewModel(QObject *parent)
    : QObject(parent),
      m_statusMessage("Ready"),
      m_processing(false),
      m_activeTab(0)
{
}

QString MainViewModel::getApplicationTitle() const
{
    return "MetaWiper";
}

QString MainViewModel::getVersion() const
{
    return QCoreApplication::applicationVersion();
}

QString MainViewModel::getStatusMessage() const
{
    return m_statusMessage;
}

void MainViewModel::setStatusMessage(const QString &message)
{
    if (m_statusMessage != message) {
        m_statusMessage = message;
        emit statusMessageChanged();
    }
}

bool MainViewModel::isProcessing() const
{
    return m_processing;
}

void MainViewModel::setProcessing(bool processing)
{
    if (m_processing != processing) {
        m_processing = processing;
        emit processingChanged();
    }
}

int MainViewModel::getActiveTab() const
{
    return m_activeTab;
}

void MainViewModel::setActiveTab(int tab)
{
    if (m_activeTab != tab) {
        m_activeTab = tab;
        emit activeTabChanged();
    }
}

QVariantMap MainViewModel::getAppInfo() const
{
    QVariantMap info;
    info["title"] = getApplicationTitle();
    info["version"] = getVersion();
    info["description"] = "A tool for viewing and managing file metadata";
    info["copyright"] = "Copyright © 2025";
    return info;
}

void MainViewModel::handleOperationCompleted(bool success, const QString &message)
{
    setStatusMessage(message);
    setProcessing(false);
}