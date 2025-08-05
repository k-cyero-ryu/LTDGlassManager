#include "databasecontroller.h"

DatabaseController::DatabaseController()
{
    connectToDataBase();
}

bool DatabaseController::connectToDataBase() {
    QString qodbc = "QODBC";

    QString FileName2 = "DRIVER={ODBC Driver 11 for SQL SERVER};"
                        "SERVER=192.168.60.10;"
                        "DATABASE=GlasPacLX_LTD;"
                        "UID=glasreader;"
                        "PWD=glasreader;"
                        "Trusted_Connection=NO";

    QString FileName1 = "DRIVER={ODBC Driver 11 for SQL SERVER};"
                        "SERVER=192.168.60.10;"
                        "DATABASE=GlassManager;"
                        "UID=glasreader;"
                        "PWD=glasreader;"
                        "Trusted_Connection=NO";

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");

    // Connect to the first database (GP)
    if (QSqlDatabase::isDriverAvailable(qodbc)) {
        QSqlDatabase dbGP = QSqlDatabase::addDatabase(qodbc, "GP");
        dbGP.setDatabaseName(FileName2);

        if (dbGP.open()) {
            databases.insert("GP", dbGP);

            // Connect to the second database (GM)
            if (QSqlDatabase::isDriverAvailable(qodbc)) {
                QSqlDatabase dbGM = QSqlDatabase::addDatabase(qodbc, "GM");
                dbGM.setDatabaseName(FileName1);

                if (dbGM.open()) {
                    databases.insert("GM", dbGM);
                    qDebug() << "Both databases opened successfully!";
                    return true;
                }
            } else {
                msg->setText("Error loading QODBC driver for GM: " + dbGP.lastError().text());
                msg->exec();
            }
        } else {
            msg->setText("Error opening GP database: " + dbGP.lastError().text());
            msg->exec();
        }
    } else {
        msg->setText("QODBC driver not available for GP");
        msg->exec();
    }

    return false;
}

QSqlDatabase *DatabaseController::Database(QString pDatabase)
{
    auto it = databases.find(pDatabase);
    if (it != databases.end()) {
        return &it.value();
    }
    return nullptr;
}
