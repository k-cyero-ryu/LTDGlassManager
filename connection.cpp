#include "connection.h"
#include <QSqlQuery>
#include <QSqlDriver>
#include <QDebug>
#include <QSqlError>
#include <QIcon>
#include <QSqlRelationalTableModel>

void connection::setFileName(QString value)
{
    FileName = value;
}

connection::connection(QObject *parent) :
    QObject(parent)
{
    FileName2 = "DRIVER={ODBC Driver 11 for SQL SERVER};"
                   "SERVER=192.168.60.10;"
                   "DATABASE=GlasPacLX_LTD;"
                   "UID=glasreader;"
                   "PWD=glasreader;"
                "Trusted_Connection=NO";
}



void connection::SetName(QString pName)
{
    FileName = pName;
}


// Function to connect to a database
bool connection::connectToDatabase(const QString& dbName, const QString& dbType, const QString& connectionName,
                       const QString& host, const QString& user, const QString& password, const QString& database) {
    QSqlDatabase db = QSqlDatabase::addDatabase(dbType, connectionName);

    if (dbType == "QSQLITE") {
        db.setDatabaseName(dbName);
    } else if (dbType == "QMYSQL") {
        db.setHostName(host);
        db.setUserName(user);
        db.setPassword(password);
        db.setDatabaseName(database);
    }

    if (!db.open()) {
        qDebug() << "Error: Unable to open database" << dbName;
        qDebug() << db.lastError().text();
        return false;
    } else {
        qDebug() << "Connected to database" << dbName;
        return true;
    }
}

bool connection::Open(){
    QString control = "QSQLITE";
    QString qodbc = "QODBC";

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");

    if(QSqlDatabase::isDriverAvailable(control))
    {
        data = QSqlDatabase::addDatabase(control);
        data.setDatabaseName(FileName);

        if(data.open()){
            //Config();

            if(QSqlDatabase::isDriverAvailable(qodbc))
            {
                data.addDatabase(qodbc,"GLASS");
                data.database("GLASS").setDatabaseName(FileName2);

                if (data.database("GLASS").open())
                {
                    if (data.database("GLASS").driver()->isOpen())
                    {
                        qDebug() << "Opens OK!!";
                    }
                    return true;
                }
            }
            else if (! data.isValid())
            {
                msg->setText("error plugin error");
                msg->exec();
            }
            else
            {
                msg->setText("error driver QODBC");
                msg->exec();
            }
        }
        /*if(!data.database("GlassPack").open())
        {
            QMessageBox::critical(0, tr("database GlassPack error"), data.lastError().text());
        }

        GlassPack = QSqlDatabase::addDatabase("QODBC3", "GlassPack");
        GlassPack.setDatabaseName(FileNameGlassPack);

        if(!GlassPack.open())
        {
            QMessageBox::critical(0, tr("database GlassManager error"), GlassPack.lastError().text());
        }
        else
        {
            if (GlassPack.database("GlassPack").driver()->isOpen())
            {
                qDebug() << "abre bien esta otra mierda";
            }
        }

        GlassManager = QSqlDatabase::addDatabase("QODBC3", "GlassManager");
        GlassManager.setDatabaseName(FileNameGlassManager);

        if(!GlassManager.open())
        {
            QMessageBox::critical(0, tr("database GlassPack error"), GlassManager.lastError().text());
        }
        else
        {
            if (GlassManager.database("GlassManager").driver()->isOpen())
            {
                qDebug() << "abre bien esta mierda";
            }
        }*/
    }
    else
    {
        msg->setText("error driver QSQLITE");
        msg->exec();
    }
    return false;

}

QSqlDatabase connection::GetDatabase()
{
    return data;
}

QSqlDatabase* connection::getGlassPack()
{
    return &data;
}

QSqlDatabase* connection::getGlassManager()
{
    return &GlassManager;
}

QString connection::getFileNameGlassPack() const
{
    return FileNameGlassPack;
}

void connection::setFileNameGlassPack(const QString &value)
{
    FileNameGlassPack = value;
}

QString connection::getFileNameglassManager() const
{
    return FileNameGlassManager;
}

void connection::setFileNameglassManager(const QString &value)
{
    FileNameGlassManager = value;
}

void connection::Config(){
    QSqlQuery qry;
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");

    //tabla cuting_table
    qryText = "CREATE TABLE IF NOT EXISTS cuting_table"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       nombre      TEXT NOT NULL,"
            "       encargado   TEXT"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla cuting_table" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }

    //tabla empleado
    qryText = "CREATE TABLE IF NOT EXISTS empleado"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       name        TEXT NOT NULL,"
            "       table_id    INTEGER NOT NULL"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla empleado" << qry.lastError().text();
        msg->setText("error tabla empleado");
        msg->exec();
    }



    //tabla orders(workorder,invoice,quote,client_name,date,status,cuting_table
    qryText = "CREATE TABLE IF NOT EXISTS orders"
            "("
            "        id              INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "        workorder       TEXT NOT NULL UNIQUE,"
            "        invoice         TEXT UNIQUE,"
            "        quote           TEXT UNIQUE,"
            "        client_name     TEXT NOT NULL,"
            "        date            TEXT NOT NULL,"
            "        status          TEXT NOT NULL,"
            "        livre_1         TEXT,"
            "        livre_2         TEXT,"
            "        cuting_table    TEXT"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla orders" << qry.lastError().text();
        msg->setText("error tabla orders");
        msg->exec();
    }

    //tabla orders_maya(workorder,invoice,quote,client_name,date,status,cuting_table
    qryText = "CREATE TABLE IF NOT EXISTS orders_maya" //NEW
              "("
              "        id              INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
              "        workorder       TEXT NOT NULL UNIQUE,"
              "        invoice         TEXT UNIQUE,"
              "        quote           TEXT UNIQUE,"
              "        client_name     TEXT NOT NULL,"
              "        date            TEXT NOT NULL,"
              "        status          TEXT NOT NULL,"
              "        livre_1         TEXT,"
              "        livre_2         TEXT,"
              "        cuting_table    TEXT"
              ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla orders_maya" << qry.lastError().text();
        msg->setText("error tabla orders_maya");
        msg->exec();
    }

    //tabla mozo
    qryText = "CREATE TABLE IF NOT EXISTS mozo"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       id_order    INTEGER NOT NULL,"
            "       glass       TEXT NOT NULL,"
            "       cant        INTEGER NOT NULL,"
            "       heigh       TEXT NOT NULL,"
            "       with        TEXT NOT NULL,"
            "       date        TEXT NOT NULL"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla mozo" << qry.lastError().text();
        msg->setText("error tabla mozo");
        msg->exec();
    }

    //tabla mozo_maya
    qryText = "CREATE TABLE IF NOT EXISTS mozo_maya" //NEW
              "("
              "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
              "       id_order    INTEGER NOT NULL,"
              "       glass       TEXT NOT NULL,"
              "       cant        INTEGER NOT NULL,"
              "       heigh       TEXT NOT NULL,"
              "       with        TEXT NOT NULL,"
              "       date        TEXT NOT NULL"
              ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla mozo_maya" << qry.lastError().text();
        msg->setText("error tabla mozo_maya");
        msg->exec();
    }

    //tabla rack
    qryText = "CREATE TABLE IF NOT EXISTS rack"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       nombre      TEXT NOT NULL,"
            "       dimension   INTEGER NOT NULL"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla rack" << qry.lastError().text();
        msg->setText("error tabla rack");
        msg->exec();
    }

    //tabla glass
    qryText = "CREATE TABLE IF NOT EXISTS glass"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       cant        INTEGER NOT NULL," //NEW
            "       color       TEXT NOT NULL,"
            "       grosor      REAL NOT NULL,"
            "       alto        REAL NOT NULL,"
            "       ancho       REAL NOT NULL"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla glass");
        msg->exec();
    }

    //tabla stock
    qryText = "CREATE TABLE IF NOT EXISTS stock"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       rack        INTEGER NOT NULL,"
            "       glass       INTEGER NOT NULL,"
            "       cantidad    INTEGER NOT NULL"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla stock" << qry.lastError().text();
        msg->setText("error tabla stock");
        msg->exec();
    }

    //tabla item
    qryText = "CREATE TABLE IF NOT EXISTS item"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       nombre      TEXT NOT NULL,"
            "       category    TEXT NOT NULL,"
            "       cantidad    INTEGER NOT NULL,"
            "       desc        TEXT NOT NULL"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla item" << qry.lastError().text();
        msg->setText("error tabla item");
        msg->exec();
    }

    //tabla asignaciones
    qryText = "CREATE TABLE IF NOT EXISTS asignaciones"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       receptor    TEXT NOT NULL,"
            "       item        INTEGER NOT NULL,"
            "       cantidad    INTEGER NOT NULL,"
            "       fecha       TEXT NOT NULL"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla asignaciones" << qry.lastError().text();
        msg->setText("error tabla asignaciones");
        msg->exec();
    }

    //tabla templados
    qryText = "CREATE TABLE IF NOT EXISTS templados"
            "("
            "       id              INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       pv              TEXT NOT NULL,"
            "       cliente         TEXT NOT NULL,"
            "       fecha           TEXT NOT NULL,"
            "       dimension       TEXT NOT NULL,"
            "       descripcion     TEXT,"
            "       idespecialista  INTEGER NOT NULL,"
            "       estado          TEXT NOT NULL"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla templados" << qry.lastError().text();
        msg->setText("error tabla templados");
        msg->exec();
    }

    //tabla consumo
    qryText = "CREATE TABLE IF NOT EXISTS consumo"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       id_order    INTEGER NOT NULL,"
            "       id_table    INTEGER NOT NULL,"
            "       glass       TEXT NOT NULL,"
            "       sheets      INTEGER NOT NULL,"
            "       fecha       TEXT NOT NULL,"
            "       extra       TEXT"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla consumo" << qry.lastError().text();
        msg->setText("error tabla consumo");
        msg->exec();
    }

    //tabla retail
    qryText = "CREATE TABLE IF NOT EXISTS retail"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       id_consumo  INTEGER NOT NULL,"
            "       glass       TEXT NOT NULL,"
            "       heigh       DECIMAL NOT NULL,"
            "       with        DECIMAL NOT NULL,"
            "       fecha       TEXT NOT NULL"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla retail" << qry.lastError().text();
        msg->setText("error tabla retail");
        msg->exec();
    }

    //tabla lames_from_retail
    qryText = "CREATE TABLE IF NOT EXISTS lames_from_retail"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            //"       id_consumo  INTEGER NOT NULL," //NEW
            "       glass       TEXT NOT NULL,"
            "       cant        INTEGER NOT NULL,"
            "       lames       INTEGER NOT NULL,"
            "       fecha       TEXT NOT NULL"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla lames_from_retail" << qry.lastError().text();
        msg->setText("error tabla lames_from_retail");
        msg->exec();
    }

    //tabla lames_from_retail //NEW
    qryText = "CREATE TABLE IF NOT EXISTS lames_transformation"
              "("
              "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
              "       glass       TEXT NOT NULL,"
              "       sheets      INTEGER NOT NULL,"
              "       cant        INTEGER NOT NULL,"
              "       lames       INTEGER NOT NULL,"
              "       fecha       TEXT NOT NULL"
              ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla lames_transformation" << qry.lastError().text();
        msg->setText("error tabla lames_transformation");
        msg->exec();
    }

    //tabla lames
    qryText = "CREATE TABLE IF NOT EXISTS lames"
            "("
            "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       glass       TEXT NOT NULL,"
            "       cant        INTEGER NOT NULL,"
            "       lames       INTEGER NOT NULL,"
            "       fecha       TEXT NOT NULL,"
            "       partida     TEXT"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla lames" << qry.lastError().text();
        msg->setText("error tabla lames");
        msg->exec();
    }


    //tabla contenedor
    qryText = "CREATE TABLE IF NOT EXISTS contenedor"
              "("
              "       id          INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
              "       no          INTEGER NOT NULL,"
              "       glass       INTEGER NOT NULL,"
              "       cantidad    INTEGER NOT NULL,"
              "       fecha_s     TEXT NOT NULL,"
              "       fecha_ll    TEXT NOT NULL"
              ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla contenedor" << qry.lastError().text();
        msg->setText("error tabla contenedor");
        msg->exec();
    }

    //tabla mozo
    qryText = "CREATE TABLE IF NOT EXISTS status_change"
            "("
            "       id              INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
            "       id_order        INTEGER NOT NULL,"
            "       status_change   TEXT NOT NULL,"
            "       user            TEXT NOT NULL,"
            "       date            TEXT NOT NULL"
            ");";

    if(!qry.exec(qryText)){
        qDebug() << "error tabla status_change" << qry.lastError().text();
        msg->setText("error tabla status_change");
        msg->exec();
    }

    //para situacion configure
}


