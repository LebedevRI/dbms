#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QtSql>
#include <QSqlDatabase>

#include "dbconnparams.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSqlDatabase db;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionDB_Connection_Params_triggered();
    void on_actionCheck_fix_data_triggered();
    void on_actionView_triggered();
    void on_actionView_2_triggered();
    void on_actionView_3_triggered();

private:
    Ui::MainWindow *ui;

    void readSettings();
    void writeSettings();
    bool reallyQuit();
};

#endif // MAINWINDOW_H
