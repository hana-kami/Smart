#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include "database.h"
namespace Ui {
class logindlg;
}

class logindlg : public QDialog
{
    Q_OBJECT

public:
    explicit logindlg(QWidget *parent = nullptr);
    ~logindlg();

private slots:
    void on_loginBtn_clicked();

    void on_quitBtn_clicked();

private:
    Ui::logindlg *ui;
    Database db;
};

#endif // LOGINDLG_H
