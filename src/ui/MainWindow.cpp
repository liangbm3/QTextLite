#include "ui/MainWindow.h"
#include "core/Document.h" // 引入Document类的头文件
#include "ui/EditorWidget.h"
#include "ui/dialogs/FindDialog.h"
#include <QPlainTextEdit>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QStatusBar> //用于显示状态栏信息
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QTextCursor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置窗口标题
    setWindowTitle("Notepad");
    // 设置窗口大小
    resize(800, 600);
    // 创建文本编辑器
    editor = new EditorWidget(this);
    // 设置布局：QMainWindow有一个特殊的中心区域，把编辑器放进去
    setCentralWidget(editor);
    // 创建菜单和动作
    createActions();
    createMenus();
    // 创建状态栏：statusBar()首次调用会创建一个状态栏
    statusBar()->showMessage(tr("Ready")); // 显示初始状态信息
    newDocument();                         // 启动时自动新建文档

    // 创建查找对话框
    m_findDialog = new FindDialog(this);
    // 连接 FindDialog 的信号到 MainWindow 的槽函数
    connect(m_findDialog, &FindDialog::findNext, this, &MainWindow::findNext);
    connect(m_findDialog, &FindDialog::findPrevious, this, &MainWindow::findPrevious);
    connect(m_findDialog, &FindDialog::replace, this, &MainWindow::replace);
    connect(m_findDialog, &FindDialog::replaceAll, this, &MainWindow::replaceAll);
}

MainWindow::~MainWindow() {}

// 重写窗口关闭事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    // 在关闭窗口前检查文档是否需要保存
    if (maybeSaveDocument())
    {
        event->accept(); // 允许关闭窗口
    }
    else
    {
        event->ignore(); // 忽略关闭事件
    }
}

// 创建动作
void MainWindow::createActions()
{
    // 新建文件动作
    newAction = new QAction(tr("&New"), this); // tr函数用于国际化
    newAction->setShortcut(QKeySequence::New); // 使用标准快捷键
    connect(newAction, &QAction::triggered, this, &MainWindow::newDocument);

    // 打开文件动作
    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openDocument);

    // 保存文件动作
    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveDocument);

    // 另存为文件动作
    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveDocumentAs);

    // 查找动作
    findAction = new QAction(tr("&Find..."), this);
    findAction->setShortcut(QKeySequence::Find);
    connect(findAction, &QAction::triggered, this, &MainWindow::showFindDialog);
}

// 创建菜单
// 菜单栏是QMainWindow的一个特性，可以通过menuBar()方法获取
void MainWindow::createMenus()
{
    // 文件菜单
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction); // 添加另存为动作

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(findAction); // 添加查找动作
}

bool MainWindow::maybeSaveDocument()
{
    // 如果文档没有修改，直接返回true
    if (!m_currentDocument || !m_currentDocument->isModified())
    {
        return true;
    }
    // 弹出提示对话框，询问用户是否保存修改
    QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Unsaved Changes"),
                                                           tr("The document has been modified.\n"
                                                              "Do you want to save your changes?"),
                                                           QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
    {
        return saveDocument(); // 调用保存文档的函数
    }
    else if (ret == QMessageBox::Cancel)
    {
        return false; // 取消操作，不关闭窗口
    }
    return true; // 如果选择丢弃修改，直接返回true
}

void MainWindow::newDocument()
{
    if (!maybeSaveDocument())
    {
        return; // 如果用户选择取消，则不创建新文档
    }
    setCurrentDocument(new Document()); // 创建一个新的Document对象
    qDebug() << "New document created.";
}

void MainWindow::openDocument()
{
    if (!maybeSaveDocument())
    {
        return; // 如果用户选择取消，则不打开新文档
    }
    Document *doc = m_fileManager.openDocument(); // 使用文件管理器打开文档
    if (doc)
    {
        setCurrentDocument(doc);                                             // 设置当前文档
        statusBar()->showMessage(tr("Document opened successfully."), 2000); // 显示打开成功信息
    }
    else
    {
        statusBar()->showMessage(tr("Failed to open document."), 2000); // 显示打开失败信息
    }
}

bool MainWindow::saveDocument()
{
    if (!m_currentDocument)
    {
        qWarning() << "No current document to save!";
        return false;
    }
    bool success = m_fileManager.saveDocument(m_currentDocument);
    if (success)
    {
        statusBar()->showMessage(tr("Document saved successfully."), 2000); // 显示保存成功信息
        return true;
    }
    else
    {
        statusBar()->showMessage(tr("Failed to save document."), 2000); // 显示保存失败信息
        return false;
    }
}

bool MainWindow::saveDocumentAs()
{
    if (!m_currentDocument)
    {
        qWarning() << "No current document to save!";
        return false;
    }
    bool success = m_fileManager.saveDocumentAs(m_currentDocument);
    if (success)
    {
        statusBar()->showMessage(tr("Document saved successfully."), 2000); // 显示保存成功信息
        return true;
    }
    else
    {
        statusBar()->showMessage(tr("Failed to save document."), 2000); // 显示保存失败信息
        return false;
    }
}

void MainWindow::updateWindowTitle()
{
    if (!m_currentDocument)
        return;
    
    qDebug() << "Updating window title for document:" << m_currentDocument->fileName();
    QString title = QString("%1[*] - %2")
                        .arg(m_currentDocument->fileName())
                        .arg(QCoreApplication::applicationName());
    setWindowTitle(title);
}

void MainWindow::onDocumentModified(bool modified)
{
    qDebug() << "Document modified state changed:" << modified;
    // QT的窗口函数，用来提升文档的修改状态
    setWindowModified(modified);
}

void MainWindow::setCurrentDocument(Document *document)
{
    // 如果有旧文档，先断开所有信号连接
    if (m_currentDocument)
    {
        disconnect(m_currentDocument, &Document::modificationChanged, this, &MainWindow::onDocumentModified);
        disconnect(m_currentDocument, &Document::filePathChanged, this, &MainWindow::updateWindowTitle);
        disconnect(editor, &QPlainTextEdit::textChanged, this, nullptr);
        m_currentDocument->deleteLater(); // 删除旧文档对象
    }
    m_currentDocument = document;
    m_currentDocument->setParent(this); // 设置父对象为MainWindow

    // 将新文档的信号连接到MainWindow的槽
    connect(m_currentDocument, &Document::modificationChanged, this, &MainWindow::onDocumentModified);
    connect(m_currentDocument, &Document::filePathChanged, this, &MainWindow::updateWindowTitle);
    connect(editor, &QPlainTextEdit::textChanged, this, [this]()
            {
        if (m_currentDocument) {
            m_currentDocument->setContent(editor->toPlainText());
        } });

    // 清空编辑器之前的修改状态
    editor->document()->setModified(false);
    // 加载新内容
    editor->setPlainText(m_currentDocument->content());

    // 更新窗口标题
    onDocumentModified(m_currentDocument->isModified());
    updateWindowTitle();
    qDebug() << "Current document set to:" << m_currentDocument->fileName();
}

void MainWindow::showFindDialog()
{
    if (m_findDialog)
    {
        // 如果编辑器中有选中的文本，自动填充到查找框
        if (editor->textCursor().hasSelection())
        {
            QString selectedText = editor->textCursor().selectedText();
            m_findDialog->setFindText(selectedText);
        }
        
        m_findDialog->show();           // 显示查找对话框
        m_findDialog->activateWindow(); // 确保对话框在前台
        m_findDialog->focusOnFindLineEdit(); // 聚焦到查找输入框
    }
    else
    {
        qWarning() << "Find dialog is not initialized!";
    }
}

void MainWindow::findNext(const QString &str, Qt::CaseSensitivity cs)
{
    if (str.isEmpty()) {
        return;
    }
    
    QTextDocument::FindFlags flags; // 查找标志
    if (cs == Qt::CaseSensitive)    // 如果区分大小写
    {
        flags |= QTextDocument::FindCaseSensitively; // 设置查找标志为区分大小写
    }

    if (!editor->find(str, flags)) // 如果没找到在状态栏显示结果
    {
        // 如果没找到，尝试从文档开头再找一次
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::Start);
        editor->setTextCursor(cursor);
        
        if (!editor->find(str, flags)) {
            statusBar()->showMessage(tr("String not found: '%1'").arg(str), 2000);
        }
    }
    else
    {
        statusBar()->showMessage(tr("Found: '%1'").arg(str), 1000);
    }
}

void MainWindow::findPrevious(const QString &str, Qt::CaseSensitivity cs)
{
    if (str.isEmpty()) {
        return;
    }
    
    QTextDocument::FindFlags flags = QTextDocument::FindBackward; // 向上查找
    if (cs == Qt::CaseSensitive)
    {
        flags |= QTextDocument::FindCaseSensitively; // 设置标志
    }

    if (!editor->find(str, flags))
    {
        // 如果没找到，尝试从文档末尾再找一次
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::End);
        editor->setTextCursor(cursor);
        
        if (!editor->find(str, flags)) {
            statusBar()->showMessage(tr("String not found: '%1'").arg(str), 2000);
        }
    }
    else
    {
        statusBar()->showMessage(tr("Found: '%1'").arg(str), 1000);
    }
}

void MainWindow::replace(const QString &str)
{
    // 如果没有选中的文本，直接返回
    if (!editor->textCursor().hasSelection())
    {
        return;
    }
    
    // 获取当前光标并插入替换文本
    QTextCursor cursor = editor->textCursor();
    cursor.insertText(str);
    
    // 替换后自动查找下一个
    findNext(m_findDialog->findText(),
             m_findDialog->isCaseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive);
}

void MainWindow::replaceAll(const QString &findStr, const QString &replaceStr, Qt::CaseSensitivity cs)
{
    if (findStr.isEmpty()) {
        return;
    }
    
    // 保存当前光标位置
    QTextCursor originalCursor = editor->textCursor();
    
    QString text = editor->toPlainText();
    int count = 0;

    // 根据大小写敏感性选择替换方式
    if (cs == Qt::CaseSensitive)
    {
        count = text.count(findStr, Qt::CaseSensitive);
        text.replace(findStr, replaceStr, Qt::CaseSensitive);
    }
    else
    {
        count = text.count(findStr, Qt::CaseInsensitive);
        text.replace(findStr, replaceStr, Qt::CaseInsensitive);
    }

    if (count > 0) {
        editor->setPlainText(text);
        
        // 尝试恢复光标位置
        QTextCursor cursor = editor->textCursor();
        cursor.setPosition(qMin(originalCursor.position(), text.length()));
        editor->setTextCursor(cursor);
    }

    statusBar()->showMessage(tr("Replaced %1 occurrence(s).").arg(count), 2000);
}