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

    QSettings settings;
    settings.setValue("wrongway", false);
    settings.beginGroup("DbConnection");
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("HostName", "localhost").toString());
    db.setPort(settings.value("Port", "3306").toInt());
    db.setUserName(settings.value("UserName", "dbms").toString());
    db.setPassword(settings.value("Password", "dbms").toString());
    db.setDatabaseName(settings.value("DatabaseName", "dbms").toString());
    settings.endGroup();
    bool ok = db.open();

    if(!ok) w.on_actionDB_Connection_Params_triggered();

    qDebug() << ok;

    return a.exec();

    db.close();
}
