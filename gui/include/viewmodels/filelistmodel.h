/**
 * @file filelistmodel.h
 * @brief 文件列表模型类
 */
#pragma once

#include <QAbstractListModel>
#include <QStringList>
#include <QFileInfo>
#include <vector>

/**
 * @class FileListModel
 * @brief 提供文件列表数据给QML视图
 */
class FileListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(QString currentFile READ getCurrentFile NOTIFY currentFileChanged)

public:
    /**
     * @brief 模型角色枚举
     */
    enum Roles {
        PathRole = Qt::UserRole + 1,
        NameRole,
        ExtensionRole,
        SizeRole,
        LastModifiedRole,
        IsSelectedRole
    };

    /**
     * @brief 构造函数
     * @param parent 父QObject
     */
    explicit FileListModel(QObject *parent = nullptr);

    /**
     * @brief 获取行数
     * @param parent 父项索引
     * @return 模型中的行数（文件数量）
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

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
     * @brief 设置文件列表
     * @param files 要设置的文件路径列表
     */
    void setFiles(const QStringList &files);

    /**
     * @brief 添加文件
     * @param filePath 要添加的文件路径
     */
    void addFile(const QString &filePath);

    /**
     * @brief 移除文件
     * @param index 要移除的文件索引
     * @return 是否成功移除
     */
    Q_INVOKABLE bool removeFile(int index);

    /**
     * @brief 清空文件列表
     */
    Q_INVOKABLE void clearFiles();

    /**
     * @brief 选择文件
     * @param index 要选择的文件索引
     */
    Q_INVOKABLE void selectFile(int index);

    /**
     * @brief 获取所有文件路径
     * @return 文件路径列表
     */
    QStringList getAllFiles() const;

    /**
     * @brief 获取选中的文件路径
     * @return 选中的文件路径列表
     */
    QStringList getSelectedFiles() const;

    /**
     * @brief 获取当前选中的文件路径
     * @return 当前选中的文件路径
     */
    QString getCurrentFile() const;

signals:
    /**
     * @brief 文件数量变化信号
     */
    void countChanged();

    /**
     * @brief 当前文件变化信号
     */
    void currentFileChanged();

    /**
     * @brief 文件选中信号
     * @param filePath 被选中的文件路径
     */
    void fileSelected(const QString &filePath);

private:
    struct FileItem {
        QString path;
        QString name;
        QString extension;
        qint64 size;
        QString lastModified;
        bool isSelected;
    };

    std::vector<FileItem> m_files;
    int m_currentIndex;
};