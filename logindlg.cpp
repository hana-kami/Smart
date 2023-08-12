#include "logindlg.h"
#include "ui_logindlg.h"
#include "QMessageBox"
#include <QSqlQuery>
#include <QDebug>
logindlg::logindlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logindlg)
{
    ui->setupUi(this);
    //设置窗口标题
    setWindowTitle("智慧果园系统");
    db.createConnection("smartOrchard");
    db.createTables();
}

logindlg::~logindlg()
{
    delete ui;

}

void logindlg::on_loginBtn_clicked()
{
//    if(!db.isOpen())
//    {
//        QMessageBox::warning(this,tr("错误"),tr("无法连接到数据库"),QMessageBox::Ok);
//        return;
//    }
    if(ui->key_lineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("请输入密码"), tr("请输入密码再登录"), QMessageBox::Ok);
        ui->lineEdit->setFocus();
    }else
    {
        QSqlQuery query(QSqlDatabase::database("smartOrchard"));
        query.prepare("select password from users where username=:username");
        query.bindValue(":username", ui->lineEdit->text());
        query.exec();
        if(query.next() > 0)
        {

            qDebug()<<query.value(0).toString();
            if(query.value(0).toString() == ui->key_lineEdit->text())
            {
                QDialog::accept();
            }
            else
            {
                QMessageBox::warning(this,tr("密码错误"),tr("请输入正确密码后在登陆"),QMessageBox::Ok);
                ui->key_lineEdit->clear();
                ui->key_lineEdit->setFocus();
            }
        }else{
            QMessageBox::warning(this,tr("账号错误"),tr("请输入正确账号"),QMessageBox::Ok);
        }
    }
}


void logindlg::on_quitBtn_clicked()
{
    QCoreApplication::quit();
}

