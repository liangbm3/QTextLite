#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QMainWindow>

//前向声明需要用到的QT类
class QPlainTextEdit;
class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QPlainTextEdit *editor; // 文本编辑器

    QAction *newAction;     // 新建文件动作
    QAction *openAction;    // 打开文件动作
    QAction *saveAction;    // 保存文件动作
    QMenu *fileMenu;       // 文件菜单

private:
    //用于创建UI的私有辅助函数
    void createActions();  // 创建动作
    void createMenus();    // 创建菜单
};

#endif // UI_MAINWINDOW_H