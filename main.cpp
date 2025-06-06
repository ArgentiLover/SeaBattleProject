#include "mainwindow.h"
#include <QIcon>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QIcon appIcon(":/images/images/app_icon.png");
    a.setWindowIcon(appIcon);

    MainWindow w;
    w.show();
    return a.exec();
}
