/**
 * @file metadatamodel.h
 * @brief 元数据模型类
 */
#pragma once

#include <QAbstractTableModel>
#include <QVariantMap>
#include <vector>
#include <QPair>

/**
 * @class MetadataModel
 * @brief 提供元数据表格数据给QML视图
 */
class MetadataModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(QVariantMap metadata READ getMetadata WRITE setMetadata NOTIFY metadataChanged)

public:
    /**
     * @brief 模型角色枚举
     */
    enum Roles {
        KeyRole = Qt::UserRole + 1,
        ValueRole,
        IsEditableRole
    };

    /**
     * @brief 构造函数
     * @param parent 父QObject
     */
    explicit MetadataModel(QObject *parent = nullptr);

    /**
     * @brief 获取行数
     * @param parent 父项索引
     * @return 模型中的行数（元数据条目数量）
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief 获取列数
     * @param parent 父项索引
     * @return 模型中的列数（固定为2列：键和值）
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief 获取模型数据
     * @param index 项索引
     * @param role 数据角色
     * @return 请求的数据
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief 获取角色名
     * @return 角色名映射
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief 获取列头数据
     * @param section 列索引
     * @param orientation 方向
     * @param role 数据角色
     * @return 列头数据
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    /**
     * @brief 获取元数据映射
     * @return 元数据键值对映射
     */
    QVariantMap getMetadata() const;

public slots:
    /**
     * @brief 设置元数据
     * @param metadata 元数据键值对映射
     */
    void setMetadata(const QVariantMap &metadata);

    /**
     * @brief 清空元数据
     */
    void clearMetadata();

    /**
     * @brief 更新单个元数据项
     * @param key 元数据键
     * @param value 元数据值
     */
    void updateMetadata(const QString &key, const QString &value);

    /**
     * @brief 根据过滤条件筛选元数据
     * @param filter 过滤关键字
     */
    void filterMetadata(const QString &filter);

signals:
    /**
     * @brief 元数据数量变化信号
     */
    void countChanged();

    /**
     * @brief 元数据变化信号
     */
    void metadataChanged();

private:
    std::vector<QPair<QString, QString>> m_metadataItems;
    QVariantMap m_metadata;
    QVariantMap m_originalMetadata;
    QString m_filter;
};