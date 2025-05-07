/**
 * @file metadatamodel.cpp
 * @brief 元数据模型类的实现
 */
#include "viewmodels/metadatamodel.h"

MetadataModel::MetadataModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int MetadataModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return static_cast<int>(m_metadataItems.size());
}

int MetadataModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2; // 键和值两列
}

QVariant MetadataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 ||
        index.row() >= static_cast<int>(m_metadataItems.size()))
        return QVariant();

    const auto &item = m_metadataItems[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0)
            return item.first;
        else if (index.column() == 1)
            return item.second;
    } else if (role == KeyRole) {
        return item.first;
    } else if (role == ValueRole) {
        return item.second;
    } else if (role == IsEditableRole) {
        // 暂时将所有值设为不可编辑，后续可根据实际需求修改
        return false;
    }

    return QVariant();
}

QHash<int, QByteArray> MetadataModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[KeyRole] = "key";
    roles[ValueRole] = "value";
    roles[IsEditableRole] = "isEditable";
    return roles;
}

QVariant MetadataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Key");
        case 1:
            return tr("Value");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariantMap MetadataModel::getMetadata() const
{
    return m_metadata;
}

void MetadataModel::setMetadata(const QVariantMap &metadata)
{
    beginResetModel();

    m_metadata = metadata;
    m_originalMetadata = metadata;
    m_metadataItems.clear();

    // 如果有过滤条件，则应用过滤
    if (!m_filter.isEmpty()) {
        for (auto it = metadata.constBegin(); it != metadata.constEnd(); ++it) {
            if (it.key().contains(m_filter, Qt::CaseInsensitive) ||
                it.value().toString().contains(m_filter, Qt::CaseInsensitive)) {
                m_metadataItems.push_back({it.key(), it.value().toString()});
            }
        }
    } else {
        // 否则显示所有元数据
        for (auto it = metadata.constBegin(); it != metadata.constEnd(); ++it) {
            m_metadataItems.push_back({it.key(), it.value().toString()});
        }
    }

    endResetModel();
    emit countChanged();
    emit metadataChanged();
}

void MetadataModel::clearMetadata()
{
    beginResetModel();
    m_metadata.clear();
    m_originalMetadata.clear();
    m_metadataItems.clear();
    endResetModel();

    emit countChanged();
    emit metadataChanged();
}

void MetadataModel::updateMetadata(const QString &key, const QString &value)
{
    // 更新内部映射
    m_metadata[key] = value;

    // 查找并更新对应项
    for (size_t i = 0; i < m_metadataItems.size(); ++i) {
        if (m_metadataItems[i].first == key) {
            m_metadataItems[i].second = value;
            QModelIndex modelIndex = createIndex(static_cast<int>(i), 1);
            emit dataChanged(modelIndex, modelIndex, {Qt::DisplayRole, ValueRole});
            break;
        }
    }

    emit metadataChanged();
}

void MetadataModel::filterMetadata(const QString &filter)
{
    if (m_filter == filter)
        return;

    m_filter = filter;

    // 重新应用过滤条件
    setMetadata(m_originalMetadata);
}