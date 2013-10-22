#include <QMessageBox>
#include <QCloseEvent>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dbconnparams.h"
#include "dbconnparams.h"
#include "addnewexpert.h"

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

    QSqlQueryModel *model_region = new QSqlQueryModel;
    model_region->setQuery("SELECT DISTINCT reg_obl_city.region FROM reg_obl_city ORDER BY reg_obl_city.region ASC;");

    ui->comboBox->setModel(model_region);
    ui->comboBox->setModelColumn(0);

    QSqlQueryModel *model_city = new QSqlQueryModel;
    model_city->setQuery("SELECT reg_obl_city.city FROM reg_obl_city ORDER BY reg_obl_city.city ASC;");

    ui->comboBox_2->setModel(model_city);
    ui->comboBox_2->setModelColumn(0);

}

void MainWindow::on_actionView_triggered()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("expert");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->removeColumn(0); // don't show the ID

    QSortFilterProxyModel *proxy_model = new QSortFilterProxyModel();
    proxy_model->invalidate();
    proxy_model->setFilterKeyColumn(-1);
    proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy_model->setDynamicSortFilter(true);
    proxy_model->setSourceModel(model);

    ui->tableView->setModel(proxy_model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

void MainWindow::on_actionView_2_triggered()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("grntirub");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->removeColumn(0); // don't show the ID

    QSortFilterProxyModel *proxy_model = new QSortFilterProxyModel();
    proxy_model->setFilterKeyColumn(-1);
    proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy_model->setDynamicSortFilter(true);
    proxy_model->setSourceModel(model);

    ui->tableView->setModel(proxy_model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

void MainWindow::on_actionView_3_triggered()
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("reg_obl_city");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->removeColumn(0); // don't show the ID

    QSortFilterProxyModel *proxy_model = new QSortFilterProxyModel();
    proxy_model->setFilterKeyColumn(-1);
    proxy_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy_model->setDynamicSortFilter(true);
    proxy_model->setSourceModel(model);

    ui->tableView->setModel(proxy_model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

void MainWindow::on_actionAdd_new_expert_triggered()
{
    QSettings settings;
    settings.setValue("wrongway", false);

    addnewexpert *ane = new addnewexpert(this);

    ane->show();
    ane->raise();
    ane->activateWindow();
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

    dcp->setModal(true);
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
    query.exec("UPDATE expert,reg_obl_city SET expert.region=reg_obl_city.region WHERE expert.city=reg_obl_city.city;");

    //Only fix missing
    //query.exec("UPDATE expert,reg_obl_city SET expert.region=reg_obl_city.region WHERE expert.region=`` AND expert.city=reg_obl_city.city;");

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
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    const QAbstractItemModel * model = index.model();

    QSettings settings;
    settings.setValue("wrongway", model->data(model->index(row, 0), Qt::DisplayRole));

    addnewexpert ane(this);
    ane.setModal(true);
    ane.show();
    ane.raise();
    ane.activateWindow();
    ane.exec();

// --------------

    this->on_actionView_triggered();
    dynamic_cast<QSortFilterProxyModel*>(ui->tableView->model())->invalidate();

    for(int i = 0; i < ui->tableView->model()->rowCount(); ++i)
    {
        if(ui->tableView->model()->data(ui->tableView->model()->index(i, 0), 0).toInt() == settings.value("wrongway").toInt())
        {
            ui->tableView->selectionModel()->setCurrentIndex(ui->tableView->model()->index(i, 0), QItemSelectionModel::SelectCurrent);
            ui->tableView->scrollTo(model->index(i, 0));
            break;
        }
    }

    settings.setValue("wrongway", false);
}

void MainWindow::on_filter_textChanged(const QString &arg1)
{
    dynamic_cast<QSortFilterProxyModel*>(ui->tableView->model())->setFilterRegExp(QRegExp(ui->filter->text(), Qt::CaseInsensitive, QRegExp::FixedString));
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QSqlQueryModel *model_city = new QSqlQueryModel;
    model_city->setQuery(QString("SELECT reg_obl_city.city FROM reg_obl_city WHERE reg_obl_city.region=\"%1\" ORDER BY reg_obl_city.city ASC").arg(arg1));

    ui->comboBox_2->setModel(model_city);
    ui->comboBox_2->setModelColumn(0);
}
