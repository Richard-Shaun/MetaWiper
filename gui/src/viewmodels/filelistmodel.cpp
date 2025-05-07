/**
 * @file filelistmodel.cpp
 * @brief 文件列表模型类的实现
 */
#include "viewmodels/filelistmodel.h"
#include <QDateTime>

FileListModel::FileListModel(QObject *parent)
    : QAbstractListModel(parent),
      m_currentIndex(-1)
{
}

int FileListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return static_cast<int>(m_files.size());
}

QVariant FileListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 ||
        index.row() >= static_cast<int>(m_files.size()))
        return QVariant();

    const FileItem &item = m_files[index.row()];

    switch (role) {
    case PathRole:
        return item.path;
    case NameRole:
        return item.name;
    case ExtensionRole:
        return item.extension;
    case SizeRole:
        return item.size;
    case LastModifiedRole:
        return item.lastModified;
    case IsSelectedRole:
        return item.isSelected;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FileListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PathRole] = "filePath";
    roles[NameRole] = "fileName";
    roles[ExtensionRole] = "fileExtension";
    roles[SizeRole] = "fileSize";
    roles[LastModifiedRole] = "lastModified";
    roles[IsSelectedRole] = "isSelected";
    return roles;
}

void FileListModel::setFiles(const QStringList &files)
{
    beginResetModel();

    m_files.clear();
    m_currentIndex = -1;

    for (const QString &filePath : files) {
        QFileInfo fileInfo(filePath);

        FileItem item;
        item.path = filePath;
        item.name = fileInfo.baseName();
        item.extension = fileInfo.suffix();
        item.size = fileInfo.size();
        item.lastModified = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
        item.isSelected = false;

        m_files.push_back(item);
    }

    endResetModel();
    emit countChanged();
    emit currentFileChanged();

    // 如果有文件，自动选择第一个
    if (!m_files.empty()) {
        selectFile(0);
    }
}

void FileListModel::addFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);

    // 检查文件是否已存在
    for (const auto &item : m_files) {
        if (item.path == filePath) {
            return;
        }
    }

    beginInsertRows(QModelIndex(), static_cast<int>(m_files.size()),
                     static_cast<int>(m_files.size()));

    FileItem item;
    item.path = filePath;
    item.name = fileInfo.baseName();
    item.extension = fileInfo.suffix();
    item.size = fileInfo.size();
    item.lastModified = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
    item.isSelected = false;

    m_files.push_back(item);

    endInsertRows();
    emit countChanged();
}

bool FileListModel::removeFile(int index)
{
    if (index < 0 || index >= static_cast<int>(m_files.size()))
        return false;

    beginRemoveRows(QModelIndex(), index, index);
    m_files.erase(m_files.begin() + index);
    endRemoveRows();

    // 更新当前索引
    if (m_currentIndex == index) {
        m_currentIndex = (m_files.empty()) ? -1 : 0;
        emit currentFileChanged();

        if (m_currentIndex >= 0) {
            m_files[m_currentIndex].isSelected = true;
            emit fileSelected(m_files[m_currentIndex].path);
        }
    } else if (m_currentIndex > index) {
        m_currentIndex--;
        emit currentFileChanged();
    }

    emit countChanged();
    return true;
}

void FileListModel::clearFiles()
{
    if (m_files.empty())
        return;

    beginResetModel();
    m_files.clear();
    m_currentIndex = -1;
    endResetModel();

    emit countChanged();
    emit currentFileChanged();
}

void FileListModel::selectFile(int index)
{
    if (index < 0 || index >= static_cast<int>(m_files.size()) || index == m_currentIndex)
        return;

    // 取消当前选择
    if (m_currentIndex >= 0 && m_currentIndex < static_cast<int>(m_files.size())) {
        m_files[m_currentIndex].isSelected = false;
        QModelIndex modelIndex = createIndex(m_currentIndex, 0);
        emit dataChanged(modelIndex, modelIndex, {IsSelectedRole});
    }

    // 设置新选择
    m_currentIndex = index;
    m_files[m_currentIndex].isSelected = true;
    QModelIndex modelIndex = createIndex(m_currentIndex, 0);
    emit dataChanged(modelIndex, modelIndex, {IsSelectedRole});

    emit currentFileChanged();
    emit fileSelected(m_files[m_currentIndex].path);
}

QStringList FileListModel::getAllFiles() const
{
    QStringList files;
    for (const auto &item : m_files) {
        files << item.path;
    }
    return files;
}

QStringList FileListModel::getSelectedFiles() const
{
    QStringList files;
    if (m_currentIndex >= 0 && m_currentIndex < static_cast<int>(m_files.size())) {
        files << m_files[m_currentIndex].path;
    }
    return files;
}

QString FileListModel::getCurrentFile() const
{
    if (m_currentIndex >= 0 && m_currentIndex < static_cast<int>(m_files.size())) {
        return m_files[m_currentIndex].path;
    }
    return QString();
}