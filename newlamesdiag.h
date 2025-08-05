#ifndef NEWLAMESDIAG_H
#define NEWLAMESDIAG_H

#include <QDialog>

namespace Ui {
class newLamesDiag;
}

class QSqlDatabase;
class newLamesDiag : public QDialog
{
    Q_OBJECT

    int IdConsumo;
    QString Glass;
    QString Fecha;

    QSqlDatabase *db;

public:
    explicit newLamesDiag(QWidget *parent = nullptr);
    ~newLamesDiag();

    int getIdConsumo() const;
    void setIdConsumo(int value);

    QString getGlass() const;
    void setGlass(const QString &value);

    QString getFecha() const;
    void setFecha(const QString &value);



    void setDb(QSqlDatabase *value);

private slots:
    void on_pushButton_clicked();

private:
    Ui::newLamesDiag *ui;
};

#endif // NEWLAMESDIAG_H
