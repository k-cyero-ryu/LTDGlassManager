#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QSqlDatabase>
#include <QMessageBox>

class connection : public QObject
{
    Q_OBJECT
    QSqlDatabase GlassPack;
    QSqlDatabase GlassManager;

    QSqlDatabase data;

    QString FileName;
    QString FileName2;

    QString FileNameGlassPack;
    QString FileNameGlassManager;

    void Config();    

public:
    explicit connection(QObject *parent = 0);

    void SetName(QString pName);
    bool Open();

    bool connectToDatabase(const QString& dbName, const QString& dbType, const QString& connectionName,
                           const QString& host = "", const QString& user = "", const QString& password = "", const QString& database = "");

    QSqlDatabase GetDatabase();

    QSqlDatabase* getGlassPack();

    QSqlDatabase* getGlassManager();

    QString getFileNameGlassPack() const;
    void setFileNameGlassPack(const QString &value);

    QString getFileNameglassManager() const;
    void setFileNameglassManager(const QString &value);

    void setFileName(QString value);

signals:

public slots:

};

#endif // CONNECTION_H
