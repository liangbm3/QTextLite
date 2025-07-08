#include "core/document.h"
#include <QFileInfo>

Document::Document(QObject *parent)
    : QObject(parent) {}

Document::~Document() {}

QString Document::filePath() const 
{
    return m_filePath;
}

QString Document::fileName() const 
{
    if(m_filePath.isEmpty()) 
    {
        return QString{"Untitled.txt"};
    }
    return QFileInfo(m_filePath).fileName();
}

QString Document::content() const 
{
    return m_content;
}

bool Document::isModified() const 
{
    return m_isModified;
}

void Document::setFilePath(const QString &filePath) 
{
    if (m_filePath != filePath) 
    {
        m_filePath = filePath;
        emit filePathChanged(m_filePath);
    }
}

void Document::setContent(const QString &content) 
{
    if (m_content != content) 
    {
        m_content = content;
        setModified(true); // 设置为已修改状态
        emit contentChanged();
    }
}

void Document::setModified(bool modified) 
{
    if (m_isModified != modified) 
    {
        m_isModified = modified;
        emit modificationChanged(m_isModified);
    }
}