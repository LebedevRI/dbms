#include <QMessageBox>
#include <QCloseEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::writeSettings()
{
    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::readSettings()
{
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

void MainWindow::on_actionMySQL_Connection_params_triggered()
{
}

MainWindow::~MainWindow()
{
    delete ui;
}
