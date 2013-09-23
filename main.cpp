#include <QApplication>
#include <QSettings>

#include "mainwindow.h"

int main(int argc, char *argv[])
{   
   QApplication a(argc, argv);
   a.setOrganizationName("LRI");
   a.setApplicationName("dbms-v5");

    MainWindow w;
    w.show();

    return a.exec();
}
