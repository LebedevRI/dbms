#include <QMessageBox>
#include <QCloseEvent>
#include <QtSql>
#include <QSqlDatabase>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dbconnparams.h"
#include "dbconnparams.h"

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings;

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    readSettings();

    QSettings settings;
    settings.beginGroup("DbConnection");
    db = QSqlDatabase::addDatabase("QMYSQL", "mydb");
    db.setHostName(settings.value("HostName", "localhost").toString());
    db.setPort(settings.value("Port", "3306").toInt());
    db.setUserName(settings.value("UserName", "dbms").toString());
    db.setPassword(settings.value("Password", "dbms").toString());
    db.setDatabaseName(settings.value("DatabaseName", "dbms").toString());
    bool ok = db.open();
    qDebug() << ok;

    settings.endGroup();

    ui->setupUi(this);
}

bool MainWindow::reallyQuit()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"),
                               tr("Do you really want to quit?"),
                               QMessageBox::Yes | QMessageBox::Cancel);
    if (ret == QMessageBox::Yes)
        return true;
    else if (ret == QMessageBox::Cancel)
        return false;

    return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (reallyQuit()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::on_actionDB_Connection_Params_triggered()
{
    dbconnparams *dcp = new dbconnparams(this);

    dcp->show();
    dcp->raise();
    dcp->activateWindow();
}
