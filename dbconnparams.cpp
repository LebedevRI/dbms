#include <QLineEdit>

#include "dbconnparams.h"
#include "ui_dbconnparams.h"
#include "mainwindow.h"

dbconnparams::dbconnparams(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dbconnparams)
{
    ui->setupUi(this);

    QSettings settings;
    settings.beginGroup("DbConnection");
    ui->hostName->setText(settings.value("HostName").toString());
    ui->port->setText(settings.value("Port").toString());
    ui->userName->setText(settings.value("UserName").toString());
    ui->password->setText(settings.value("Password").toString());
    ui->databaseName->setText(settings.value("DatabaseName").toString());
    settings.endGroup();
}

dbconnparams::~dbconnparams()
{
    delete ui;
}

void dbconnparams::accept()
{
    QSettings settings;

    settings.beginGroup("DbConnection");
    settings.setValue("HostName", ui->hostName->text());
    settings.setValue("Port", ui->port->text());
    settings.setValue("UserName", ui->userName->text());
    settings.setValue("Password", ui->password->text());
    settings.setValue("DatabaseName", ui->databaseName->text());
    settings.endGroup();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    settings.beginGroup("DbConnection");
    db.setHostName(settings.value("HostName").toString());
    db.setPort(settings.value("Port").toInt());
    db.setUserName(settings.value("UserName").toString());
    db.setPassword(settings.value("Password").toString());
    db.setDatabaseName(settings.value("DatabaseName").toString());
    settings.endGroup();
    bool ok = db.open();

    if(ok) {
        this->setResult(QDialog::Accepted);
        this->close();
        this->done(0);
    }
}

void dbconnparams::reject()
{
/*    QSettings settings;

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    settings.beginGroup("DbConnection");
    db.setHostName(settings.value("HostName").toString());
    db.setPort(settings.value("Port").toInt());
    db.setUserName(settings.value("UserName").toString());
    db.setPassword(settings.value("Password").toString());
    db.setDatabaseName(settings.value("DatabaseName").toString());
    settings.endGroup();
    db.open();*/

//    if(ok) {
        this->setResult(QDialog::Rejected);
        this->close();
        this->done(0);
//    };
}
