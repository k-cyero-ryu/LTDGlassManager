#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlDriverPlugin>
#include <QMessageBox>


class DatabaseController
{
    QSqlDatabase data;

    QHash<QString, QSqlDatabase> databases;


public:
    DatabaseController();
    bool connectToDataBase();
    QSqlDatabase* Database(QString pDatabase);


public slots:

};

#endif // DATABASECONTROLLER_H
