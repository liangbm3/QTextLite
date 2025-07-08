#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序的组织名和应用名
    app.setOrganizationName("MyCompany");
    app.setApplicationName("Notepad");

    // 创建主窗口
    MainWindow mainWindow;
    mainWindow.show();

    // 进入应用程序的事件循环
    return app.exec();
}