#include <QApplication>
#include <QSettings>

#include "mainwindow.h"

int main(int argc, char *argv[])
{   
    QCoreApplication::setOrganizationName("LRI");
    QCoreApplication::setApplicationName("dbms-v5");
    QSettings settings;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
