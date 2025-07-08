#include "ui/MainWindow.h"

#include <QPlainTextEdit>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QStatusBar>//用于显示状态栏信息

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
}

MainWindow::~MainWindow() {}


//创建动作
void MainWindow::createActions()
{
    //新建文件动作
    newAction = new QAction(tr("&New"), this);//tr函数用于国际化
    newAction->setShortcut(QKeySequence::New);//使用标准快捷键
    //TODO
    // connect(newAction, &QAction::triggered, this, [this]() {
    //     editor->clear();
    // });

    //打开文件动作
    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcut(QKeySequence::Open);
    // connect(openAction, &QAction::triggered, this, [this]() {
    //     // 打开文件逻辑
    // });

    //保存文件动作
    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    // connect(saveAction, &QAction::triggered, this, [this]() {
    //     // 保存文件逻辑
    // });
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
}