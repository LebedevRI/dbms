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
    db.setHostName(settings.value("HostName").toString());
    db.setPort(settings.value("Port").toInt());
    db.setUserName(settings.value("UserName").toString());
    db.setPassword(settings.value("Password").toString());
    db.setDatabaseName(settings.value("DatabaseName").toString());
    settings.endGroup();
    bool ok = db.open();

    if(!ok) w.on_actionDB_Connection_Params_triggered();

    qDebug() << ok;

    return a.exec();

    db.close();
}
