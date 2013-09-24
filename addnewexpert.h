#ifndef ADDNEWEXPERT_H
#define ADDNEWEXPERT_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class addnewexpert;
}

class addnewexpert : public QDialog
{
    Q_OBJECT

public:
    explicit addnewexpert(QWidget *parent = 0);
    ~addnewexpert();

private:
    Ui::addnewexpert *ui;
    bool reallyQuit();

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void accept();
};

#endif // ADDNEWEXPERT_H
