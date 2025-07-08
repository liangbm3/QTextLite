#include "core/FileManager.h"
#include "core/document.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

FileManager::FileManager(QWidget *m_parentWidget)
    : m_parentWidget(m_parentWidget)
{

}

FileManager::~FileManager()
{

}

bool FileManager::saveDocument(Document *document)
{
    if(!document) 
    {
        return false; // 如果文档为空，直接返回失败
    }
    //如果文档没有关联路径，行为等于另存为
    if(document->fileName().isEmpty()) 
    {
        return saveDocumentAs(document);
    }

    QFile file(document->filePath());//创建 QFile 对象
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) //只写+文本模式打开
    {
        //如果打开失败，弹出警告对话框
        QMessageBox::warning(m_parentWidget, QObject::tr("Error"),
                             QObject::tr("Could not write to file %1: %2")
                             .arg(QDir::toNativeSeparators(document->filePath()), file.errorString()));
                             //将路径格式转换为当前操作系统的风格
        return false;
    }

    QTextStream out(&file);
    out << document->content();
    file.close();

    document->setModified(false); // 保存后重置修改状态
    return true;
}

bool FileManager::saveDocumentAs(Document *document)
{
    //打开文件对话框让用户选择保存位置
    QString filePath = QFileDialog::getSaveFileName(m_parentWidget, QObject::tr("Save As"),
                                                    document->filePath(),
                                                    QObject::tr("Text Files (*.txt);;All Files (*)"));
    if (filePath.isEmpty()) 
    {
        return false; // 用户取消了保存操作
    }

    document->setFilePath(filePath); // 设置新的文件路径
    return saveDocument(document); // 调用保存函数
}