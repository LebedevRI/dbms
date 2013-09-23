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

    ui->lineEdit->setText(settings.value("HostName", "localhost").toString());
    ui->lineEdit_2->setText(settings.value("Port", "3306").toString());
    ui->lineEdit_3->setText(settings.value("UserName", "dbms").toString());
    ui->lineEdit_4->setText(settings.value("Password", "dbms").toString());
    ui->lineEdit_5->setText(settings.value("DatabaseName", "dbms").toString());

}

dbconnparams::~dbconnparams()
{
    delete ui;
}

void dbconnparams::accept()
{
    QSettings settings;

    settings.beginGroup("DbConnection");
    settings.setValue("HostName", ui->lineEdit->text());
    settings.setValue("Port", ui->lineEdit_2->text());
    settings.setValue("UserName", ui->lineEdit_3->text());
    settings.setValue("Password", ui->lineEdit_4->text());
    settings.setValue("DatabaseName", ui->lineEdit_5->text());
    settings.endGroup();

    this->setResult(QDialog::Accepted);
    this->close();
}
