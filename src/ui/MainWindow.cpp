#include "ui/MainWindow.h"
#include "core/document.h" // 引入Document类的头文件
#include <QPlainTextEdit>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QStatusBar>//用于显示状态栏信息
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //设置窗口标题
    setWindowTitle("Notepad");
    //设置窗口大小
    resize(800, 600);
    //创建文本编辑器
    editor = new QPlainTextEdit(this);
    //设置布局：QMainWindow有一个特殊的中心区域，把编辑器放进去
    setCentralWidget(editor);
    //创建菜单和动作
    createActions();
    createMenus();
    //创建状态栏：statusBar()首次调用会创建一个状态栏
    statusBar()->showMessage(tr("Ready")); // 显示初始状态信息
    newDocument(); // 启动时自动新建文档
}

MainWindow::~MainWindow() {}

//重写窗口关闭事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    //在关闭窗口前检查文档是否需要保存
    if(maybeSaveDocument())
    {
        event->accept(); // 允许关闭窗口
    }
    else
    {
        event->ignore(); // 忽略关闭事件
    }
}

//创建动作
void MainWindow::createActions()
{
    //新建文件动作
    newAction = new QAction(tr("&New"), this);//tr函数用于国际化
    newAction->setShortcut(QKeySequence::New);//使用标准快捷键
    connect(newAction, &QAction::triggered, this, &MainWindow::newDocument);

    //打开文件动作
    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcut(QKeySequence::Open);
    // connect(openAction, &QAction::triggered, this, [this]() {
    //     // 打开文件逻辑
    // });

    //保存文件动作
    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction,&QAction::triggered, this, &MainWindow::saveDocument);

    //另存为文件动作
    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
}

//创建菜单
//菜单栏是QMainWindow的一个特性，可以通过menuBar()方法获取
void MainWindow::createMenus()
{
    //文件菜单
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction); // 添加另存为动作
}

bool MainWindow::maybeSaveDocument()
{
    //如果文档没有修改，直接返回true
    if(!m_currentDocument || !m_currentDocument->isModified())
    {
        return true;
    }
    //弹出提示对话框，询问用户是否保存修改
    QMessageBox::StandardButton ret = QMessageBox::warning(this, tr("Unsaved Changes"),
                                                           tr("The document has been modified.\n"
                                                              "Do you want to save your changes?"),
                                                           QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if(ret == QMessageBox::Save)
    {
        return saveDocument(); // 调用保存文档的函数
    }
    else if(ret == QMessageBox::Cancel)
    {
        return false; // 取消操作，不关闭窗口
    }
    return true; // 如果选择丢弃修改，直接返回true
}

void MainWindow::newDocument()
{
    if(!maybeSaveDocument())
    {
        return; // 如果用户选择取消，则不创建新文档
    }
    //如果当前文档存在，断开与其信号的连接
    if(m_currentDocument)
    {
        disconnect(m_currentDocument, &Document::modificationChanged, this, &MainWindow::onDocumentModified);
        disconnect(m_currentDocument, &Document::filePathChanged, this, &MainWindow::updateWindowTitle);
    }
    //在MainWindow的声明周期中删除文档对象，因此需要手动delete
    if(m_currentDocument)
    {
        m_currentDocument->deleteLater();
    }
    m_currentDocument = new Document(this); // 创建新的文档对象
    qDebug() << "Creating new document with file name:" << m_currentDocument->fileName();
    //将新的文档连接到信号和槽
    connect(m_currentDocument, &Document::modificationChanged, this, &MainWindow::onDocumentModified);
    connect(m_currentDocument, &Document::filePathChanged, this, &MainWindow::updateWindowTitle);

    //当编辑器内容更改时，更新文档内容
    connect(editor, &QPlainTextEdit::textChanged, this, [this]() {
        //只有当用户输入内容导致变化时才设置，避免在程序加载内容时触发
        if(editor->document()->isModified())
        {
            qDebug() << "Editor content changed, updating document.";
            m_currentDocument->setContent(editor->toPlainText());
        }
    });

    //重置编辑器内容
    editor->setPlainText(m_currentDocument->content());
    //更新窗口标题
    updateWindowTitle();
}

bool MainWindow::saveDocument()
{
    if(!m_currentDocument)
    {
        qWarning() << "No current document to save!";
        return false;
    }
    bool success = m_fileManager.saveDocument(m_currentDocument);
    if(success)
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
    if(!m_currentDocument)
    {
        qWarning() << "No current document to save!";
        return false;
    }
    bool success = m_fileManager.saveDocumentAs(m_currentDocument);
    if(success)
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
    qDebug() << "Updating window title for document:" << m_currentDocument->fileName();
    if (!m_currentDocument) return;
    QString title = QString("%1[*] - %2")
                        .arg(m_currentDocument->fileName())
                        .arg(QCoreApplication::applicationName());
    setWindowTitle(title);
}

void MainWindow::onDocumentModified(bool modified)
{
    qDebug() << "Document modified state changed:" << modified;
    //QT的窗口函数，用来提升文档的修改状态
    setWindowModified(modified);
}