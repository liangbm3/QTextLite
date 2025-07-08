#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>

class Document;
class QWidget;

// 封装所有与文件系统的交互操作
class FileManager
{
public:
    explicit FileManager(QWidget *m_parentWidget = nullptr);
    ~FileManager();

    //保存文档，保存成功或用户取消操作为true，保存失败为false
    bool saveDocument(Document *document);

    //另存为，成功为true，用户取消操作或失败为false
    bool saveDocumentAs(Document *document);

    Document* openDocument();

private:
    QWidget *m_parentWidget; // 父窗口，用于对话框的父级
};

#endif // FILEMANAGER_H