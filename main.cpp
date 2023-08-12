#include "mainwindow.h"
#include "loadfileqss.h"
#include "logindlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("智慧果园系统");
    LoadFileQss::setStyle(":/res/Allthe.qss"); //加载qss 样式表，用静态方式
    logindlg login;
    MainWindow w;
    if(login.exec() == QDialog::Accepted)
    {
        w.show();
    return a.exec();
    }
    return a.exec();
}
