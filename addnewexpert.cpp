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
    QSettings settings;

    if(settings.value("wrongway").toString() == "") {
        ui->checkBox->hide();

        QSqlQuery query;
        query.prepare("SELECT kod FROM expert ORDER BY expert.id DESC LIMIT 1;");
        query.exec();

        query.first();
        int lastKod = query.value(query.record().indexOf("kod")).toInt();
        ui->lineEdit->setText(QString("%1").arg(lastKod+1));

        ui->dateEdit->setDate(QDate::currentDate());
    } else {
        QSqlQuery query;
        query.prepare(QString("SELECT * FROM expert WHERE kod='%1';").arg(settings.value("wrongway").toString()));
        query.exec();
        query.first();

        ui->lineEdit->setText(QString("%1").arg(query.value(query.record().indexOf("kod")).toString()));
        ui->lineEdit_2->setText(QString("%1").arg(query.value(query.record().indexOf("name")).toString()));
        ui->comboBox->setCurrentIndex(ui->comboBox->findData(QString("%1").arg(query.value(query.record().indexOf("region")).toString())));
        ui->comboBox_2->setCurrentIndex(ui->comboBox->findData(QString("%1").arg(query.value(query.record().indexOf("city")).toString())));
        ui->lineEdit_3->setText(QString("%1").arg(query.value(query.record().indexOf("grnti")).toString()));
        ui->lineEdit_4->setText(QString("%1").arg(query.value(query.record().indexOf("key_words")).toString()));

        ui->spinBox->setValue(query.value(query.record().indexOf("take_part")).toInt());
        ui->dateEdit->setDate(query.value(query.record().indexOf("input_date")).toDate());
    }

    QSqlQueryModel *model_region = new QSqlQueryModel;
    model_region->setQuery("SELECT DISTINCT reg_obl_city.region FROM reg_obl_city ORDER BY reg_obl_city.region ASC");

    ui->comboBox->setModel(model_region);
    ui->comboBox->setModelColumn(0);

    QSqlQueryModel *model_city = new QSqlQueryModel;
    model_city->setQuery("SELECT reg_obl_city.city FROM reg_obl_city ORDER BY reg_obl_city.city ASC");

    ui->comboBox_2->setModel(model_city);
    ui->comboBox_2->setModelColumn(0);
}

addnewexpert::~addnewexpert()
{
    QSettings settings;
    settings.setValue("wrongway", QString(""));
    delete ui;
}

void addnewexpert::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QSqlQueryModel *model_city = new QSqlQueryModel;
    model_city->setQuery(QString("SELECT reg_obl_city.city FROM reg_obl_city WHERE reg_obl_city.region=\"%1\" ORDER BY reg_obl_city.city ASC").arg(arg1));

    ui->comboBox_2->setModel(model_city);
    ui->comboBox_2->setModelColumn(0);
}

bool addnewexpert::reallyQuit()
{
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"),
                               tr("Do you really want to delete expert?"),
                               QMessageBox::Yes | QMessageBox::Cancel);
    if (ret == QMessageBox::Yes)
        return true;
    else if (ret == QMessageBox::Cancel)
        return false;

    return false;
}

void addnewexpert::accept()
{
    QSettings settings;
    if(ui->checkBox->isHidden())
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
    } else {
        if(ui->checkBox->isChecked())
        {
            if (reallyQuit()) {
                QSqlDatabase::database().transaction();
                QSqlQuery query;
                query.exec(QString("DELETE FROM expert WHERE expert.kod='%1';")
                           .arg(settings.value("wrongway").toString()));
                QSqlDatabase::database().commit();

                settings.setValue("wrongway", QString(""));
                this->setResult(QDialog::Accepted);
                this->close();
            }
        } else {
            QSqlDatabase::database().transaction();
            QSqlQuery query;
            query.exec(QString("UPDATE expert SET kod='%1',name='%2',region='%3',city='%4',grnti='%5',key_words='%6',take_part='%7',input_date='%8' WHERE kod='%9';")
                       .arg(ui->lineEdit->text())
                       .arg(ui->lineEdit_2->text())
                       .arg(ui->comboBox->currentText())
                       .arg(ui->comboBox_2->currentText())
                       .arg(ui->lineEdit_3->text())
                       .arg(ui->lineEdit_4->text())
                       .arg(ui->spinBox->value())
                       .arg(ui->dateEdit->date().toString("yyyy-MM-dd"))
                       .arg(settings.value("wrongway").toString()));
            QSqlDatabase::database().commit();

            settings.setValue("wrongway", QString(""));
            this->setResult(QDialog::Accepted);
            this->close();
        };
    };
}
