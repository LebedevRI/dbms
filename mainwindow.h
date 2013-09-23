#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSettings settings;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionMySQL_Connection_params_triggered();

private:
    Ui::MainWindow *ui;
    void readSettings();
    void writeSettings();
    bool reallyQuit();
};

#endif // MAINWINDOW_H
