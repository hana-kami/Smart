#ifndef DATABASE_H
#define DATABASE_H



#include <QSqlDatabase>
#include <QSqlQuery>
class Database
{
public:
    Database();
    bool createConnection(QString dbname);
    bool createTables();
    bool closeDatabase();
    bool isOpen();
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
