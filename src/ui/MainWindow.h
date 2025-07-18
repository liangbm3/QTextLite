#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QMainWindow>

#include "core/FileManager.h"

//前向声明需要用到的QT类
class EditorWidget;
class FindDialog;
class QAction;
class QMenu;
class Document; // 前向声明Document类，避免包含头文件

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    //重写事件处理函数
    void closeEvent(QCloseEvent *event) override; // 处理窗口关闭事件

private slots:
    //槽函数可以连接到动作的信号
    void newDocument(); // 新建文档
    void openDocument(); // 打开文档
    void onDocumentModified(bool modified);// 文档被修改时的处理函数
    bool saveDocument(); // 保存当前文档
    bool saveDocumentAs(); // 另存为当前文档

    // 用于查找/替换的新增槽函数
    void showFindDialog();
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);
    void replace(const QString &str);
    void replaceAll(const QString &findStr, const QString &replaceStr, Qt::CaseSensitivity cs);

    //设置相关
    void showSettingsDialog(); // 显示设置对话框
    void applySettings();

private:
    //UI控件指针
    EditorWidget *editor; // 文本编辑器
    QAction *newAction;     // 新建文件动作
    QAction *openAction;    // 打开文件动作
    QAction *saveAction;    // 保存文件动作
    QAction *saveAsAction; // 另存为文件动作
    QAction *findAction; // 查找动作
    QAction *zoomInAction; // 放大动作
    QAction *zoomOutAction; // 缩小动作
    QAction *zoomResetAction; // 重置缩放动作
    QAction *settingsAction; // 设置动作
    QMenu *fileMenu;       // 文件菜单

    FileManager m_fileManager; // 文件管理器，用于处理文件操作

    Document *m_currentDocument=nullptr; // 当前文档对象

    FindDialog *m_findDialog; // 查找对话框

    //用于创建UI的私有辅助函数
    void createActions();  // 创建动作
    void createMenus();    // 创建菜单

    //根据文档当前状态更新窗口标题
    void updateWindowTitle();

    //检查当前文档是否需要保存，并询问用户，如果用户选择保存、丢弃文档或者无需保存则为true
    //如果用户选择取消，则返回false
    bool maybeSaveDocument();

    void setCurrentDocument(Document *document);
};

#endif // UI_MAINWINDOW_H