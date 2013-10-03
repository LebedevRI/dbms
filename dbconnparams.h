#ifndef DBCONNPARAMS_H
#define DBCONNPARAMS_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class dbconnparams;
}

class dbconnparams : public QDialog
{
    Q_OBJECT

public:
    explicit dbconnparams(QWidget *parent = 0);
    ~dbconnparams();

private:
    Ui::dbconnparams *ui;

private slots:
    void accept();
    void reject();
};

#endif // DBCONNPARAMS_H
