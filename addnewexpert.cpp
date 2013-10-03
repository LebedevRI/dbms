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

    QSqlQueryModel *model_region = new QSqlQueryModel;
    model_region->setQuery("SELECT DISTINCT reg_obl_city.region FROM reg_obl_city ORDER BY reg_obl_city.region ASC;");

    ui->comboBox->setModel(model_region);
    ui->comboBox->setModelColumn(0);

    QSqlQueryModel *model_city = new QSqlQueryModel;
    model_city->setQuery("SELECT reg_obl_city.city FROM reg_obl_city ORDER BY reg_obl_city.city ASC;");

    ui->comboBox_2->setModel(model_city);
    ui->comboBox_2->setModelColumn(0);

    if(!settings.value("wrongway").toBool()) {
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
        query.prepare("SELECT * FROM expert WHERE kod=:kod;");
        query.bindValue(":kod", settings.value("wrongway").toInt());
        query.exec();
        query.first();

        ui->lineEdit->setText(query.value(query.record().indexOf("kod")).toString());
        ui->lineEdit_2->setText(query.value(query.record().indexOf("name")).toString());

        int index = ui->comboBox->findText(query.value(query.record().indexOf("region")).toString());
        ui->comboBox->setCurrentIndex(index);

        int index2 = ui->comboBox_2->findText(query.value(query.record().indexOf("city")).toString());
        ui->comboBox_2->setCurrentIndex(index2);

        ui->lineEdit_3->setText(query.value(query.record().indexOf("grnti")).toString());
        ui->lineEdit_4->setText(query.value(query.record().indexOf("key_words")).toString());

        ui->spinBox->setValue(query.value(query.record().indexOf("take_part")).toInt());
        ui->dateEdit->setDate(query.value(query.record().indexOf("input_date")).toDate());
    }
}

addnewexpert::~addnewexpert()
{
    QSettings settings;
    settings.setValue("wrongway", false);
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
        query.prepare("INSERT INTO expert VALUES(NULL,:kod,:name,:region,:city,:grnti,:key_words,:take_part,:input_date);");
        query.bindValue(":kod", ui->lineEdit->text());
        query.bindValue(":name", ui->lineEdit_2->text());
        query.bindValue(":region", ui->comboBox->currentText());
        query.bindValue(":city", ui->comboBox_2->currentText());
        query.bindValue(":grnti", ui->lineEdit_3->text());
        query.bindValue(":key_words", ui->lineEdit_4->text());
        query.bindValue(":take_part", ui->spinBox->value());
        query.bindValue(":input_date", ui->dateEdit->date().toString("yyyy-MM-dd"));
        query.exec();
        QSqlDatabase::database().commit();

        this->setResult(QDialog::Accepted);
        this->close();
    } else {
        if(ui->checkBox->isChecked())
        {
            if (reallyQuit()) {
                QSqlDatabase::database().transaction();
                QSqlQuery query;
                query.prepare("DELETE FROM expert WHERE expert.kod=:kod");
                query.bindValue(":kod", settings.value("wrongway").toString());
                query.exec();
                QSqlDatabase::database().commit();

                settings.setValue("wrongway", false);
                this->setResult(QDialog::Accepted);
                this->close();
            }
        } else {
            QSqlDatabase::database().transaction();
            QSqlQuery query;
            query.prepare("UPDATE expert SET kod=:kod,name=:name,region=:region,city=:city,grnti=:grnti,key_words=:key_words,take_part=:take_part,input_date=:input_date WHERE kod=:kod_where;");
            query.bindValue(":kod", ui->lineEdit->text());
            query.bindValue(":name", ui->lineEdit_2->text());
            query.bindValue(":region", ui->comboBox->currentText());
            query.bindValue(":city", ui->comboBox_2->currentText());
            query.bindValue(":grnti", ui->lineEdit_3->text());
            query.bindValue(":key_words", ui->lineEdit_4->text());
            query.bindValue(":take_part", ui->spinBox->value());
            query.bindValue(":input_date", ui->dateEdit->date().toString("yyyy-MM-dd"));
            query.bindValue(":kod_where", settings.value("wrongway").toString());
            query.exec();
            QSqlDatabase::database().commit();

            settings.setValue("wrongway", false);
            this->setResult(QDialog::Accepted);
            this->close();
        };
    };
}
