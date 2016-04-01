#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("focus-ebeam");   //设置QSettings存储
    a.setOrganizationDomain("focus-ebeam.com");
    a.setApplicationName("imageGrapAssistant");
    MainWindow w;
    w.show();

    return a.exec();
}
