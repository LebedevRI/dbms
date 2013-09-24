#include <QMessageBox>
#include <QCloseEvent>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>

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
    ui->setupUi(this);
    readSettings();
}

void MainWindow::on_actionView_triggered()
{
    QSettings settings;
    settings.beginGroup("DbConnection");
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("HostName", "localhost").toString());
    db.setPort(settings.value("Port", "3306").toInt());
    db.setUserName(settings.value("UserName", "dbms").toString());
    db.setPassword(settings.value("Password", "dbms").toString());
    db.setDatabaseName(settings.value("DatabaseName", "dbms").toString());
    settings.endGroup();
    bool ok = db.open();
    qDebug() << ok;

    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("expert");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->removeColumn(0); // don't show the ID
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    db.close();
}

void MainWindow::on_actionView_2_triggered()
{
    QSettings settings;
    settings.beginGroup("DbConnection");
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("HostName", "localhost").toString());
    db.setPort(settings.value("Port", "3306").toInt());
    db.setUserName(settings.value("UserName", "dbms").toString());
    db.setPassword(settings.value("Password", "dbms").toString());
    db.setDatabaseName(settings.value("DatabaseName", "dbms").toString());
    settings.endGroup();
    bool ok = db.open();
    qDebug() << ok;

    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("grntirub");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->removeColumn(0); // don't show the ID
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    db.close();
}

void MainWindow::on_actionView_3_triggered()
{
    QSettings settings;
    settings.beginGroup("DbConnection");
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("HostName", "localhost").toString());
    db.setPort(settings.value("Port", "3306").toInt());
    db.setUserName(settings.value("UserName", "dbms").toString());
    db.setPassword(settings.value("Password", "dbms").toString());
    db.setDatabaseName(settings.value("DatabaseName", "dbms").toString());
    settings.endGroup();
    bool ok = db.open();
    qDebug() << ok;

    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("reg_obl_city");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->removeColumn(0); // don't show the ID
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    db.close();
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
    delete ui;
}

void MainWindow::on_actionDB_Connection_Params_triggered()
{
    dbconnparams *dcp = new dbconnparams(this);

    dcp->show();
    dcp->raise();
    dcp->activateWindow();
}

void MainWindow::on_actionCheck_fix_data_triggered()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlDatabase::database().transaction();
    QSqlQuery query;

    //Update ALL regions in expert table
    //query.exec("UPDATE expert,reg_obl_city SET expert.region=reg_obl_city.region WHERE expert.city=reg_obl_city.city;");

    //Only fix missing
    query.exec("UPDATE expert,reg_obl_city SET expert.region=reg_obl_city.region WHERE expert.region="" AND expert.city=reg_obl_city.city;");

    QSqlTableModel *model = new QSqlTableModel(0, db);
    model->setTable("expert");
    model->setFilter("kod = \"\"");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    for(auto i=0; i<model->rowCount(); i++)
    {
        QSqlRecord record = model->record(i);
        int recId = record.value(record.indexOf("id")).toInt();

        QSqlQuery query_mid;
        query_mid.prepare("SELECT * FROM expert WHERE (id = (SELECT MAX(`id`) FROM expert WHERE `id` <:id) OR `id` = (SELECT MIN(`id`) FROM expert WHERE `id` >:id));");
        query_mid.bindValue(":id", recId);
        query_mid.exec();

        query_mid.first();
        int prevKod = query_mid.value(query_mid.record().indexOf("kod")).toInt();
        query_mid.next();
        int nextKod = query_mid.value(query_mid.record().indexOf("kod")).toInt();

        Q_ASSERT((prevKod < nextKod) && ((prevKod+2) == nextKod));

        record.setValue(record.indexOf("kod"), prevKod+1);
        model->setRecord(i, record);
    }
    model->submitAll();
    QSqlDatabase::database().commit();
    db.close();
}
