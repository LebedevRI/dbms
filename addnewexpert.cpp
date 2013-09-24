#include <QMessageBox>
#include <QCloseEvent>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDateTime>

#include "addnewexpert.h"
#include "ui_addnewexpert.h"

addnewexpert::addnewexpert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addnewexpert)
{
    ui->setupUi(this);

    QSqlQuery query;
    query.prepare("SELECT kod FROM expert ORDER BY expert.id DESC LIMIT 1;");
    query.exec();

    query.first();
    int lastKod = query.value(query.record().indexOf("kod")).toInt();
    ui->lineEdit->setText(QString("%1").arg(lastKod+1));

    QSqlQueryModel *model_region = new QSqlQueryModel;
    model_region->setQuery("SELECT DISTINCT reg_obl_city.region FROM reg_obl_city ORDER BY reg_obl_city.region ASC");

    ui->comboBox->setModel(model_region);
    ui->comboBox->setModelColumn(0);

    QSqlQueryModel *model_city = new QSqlQueryModel;
    model_city->setQuery("SELECT reg_obl_city.city FROM reg_obl_city ORDER BY reg_obl_city.city ASC");

    ui->comboBox_2->setModel(model_city);
    ui->comboBox_2->setModelColumn(0);


    ui->dateEdit->setDate(QDate::currentDate());

}

addnewexpert::~addnewexpert()
{
    delete ui;
}

void addnewexpert::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QSqlQueryModel *model_city = new QSqlQueryModel;
    model_city->setQuery(QString("SELECT reg_obl_city.city FROM reg_obl_city WHERE reg_obl_city.region=\"%1\" ORDER BY reg_obl_city.city ASC").arg(arg1));

    ui->comboBox_2->setModel(model_city);
    ui->comboBox_2->setModelColumn(0);
}

void addnewexpert::accept()
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.exec(QString("INSERT INTO expert VALUES(NULL,'%1','%2','%3','%4','%5','%6','%7','%8');")
               .arg(ui->lineEdit->text())
               .arg(ui->lineEdit_2->text())
               .arg(ui->comboBox->currentText())
               .arg(ui->comboBox_2->currentText())
               .arg(ui->lineEdit_3->text())
               .arg(ui->lineEdit_4->text())
               .arg(ui->spinBox->value())
               .arg(ui->dateEdit->date().toString("yyyy-MM-dd")));
    QSqlDatabase::database().commit();

    this->setResult(QDialog::Accepted);
    this->close();
}
