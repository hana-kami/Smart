#include "database.h"

#include <QMessageBox>
#include <QDebug>
Database::Database()
{

}

bool Database::createConnection(QString dbname)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbname);

    if(!db.open()){
        QMessageBox::critical(0,"cannot open database","Unabel to connect",QMessageBox::Cancel);
        return false;
    }
    qDebug()<<"createConnection:" ;
    return true;
}

bool Database::createTables()
{
    QSqlQuery query;
    qDebug()<< "createTables :";
    query.exec("create table users(id Integer Primary key,username Varchar(20),password Intrger)");
    query.exec("insert into users values(1,'小王',117683767)");
    query.exec("insert into users values(2,'admin',111111)");
}

bool Database::closeDatabase()
{
    db.close();
}

bool Database::isOpen()
{
        return db.isOpen();
}

