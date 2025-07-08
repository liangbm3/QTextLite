#ifndef CORE_DOCUMENT_H
#define CORE_DOCUMENT_H

#include <QObject>
#include <QString>

// 代表一个文档对象，封装了其内容、文件路径和修改状态等信息
class Document : public QObject
{
    Q_OBJECT
public:
    explicit Document(QObject *parent = nullptr);
    ~Document();

    // --- Getters ---
    QString filePath() const;// 获取文件路径
    QString fileName() const; // 辅助函数，从路径中提取文件名
    QString content() const;//获取文件内容
    bool isModified() const;//获取是否被修改的状态
private:
    QString m_content;         // 文档内容
    QString m_filePath;        // 文件路径
    bool m_isModified = false; // 是否被修改

public slots:
    // --- Setters ---
    //使用槽可以让这些函数连接到其他QT对象的信号
    void setFilePath(const QString &filePath); // 设置文件路径
    void setContent(const QString &content);   // 设置文档内容
    void setModified(bool modified);            // 设置修改状态

signals:
    // --- Signals ---
    // 当文档状态改变时发射这些信号
    void contentChanged();
    void filePathChanged(const QString &newPath);
    void modificationChanged(bool isModified);

};

#endif // CORE_DOCUMENT_H