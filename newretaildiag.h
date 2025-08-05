#ifndef NEWRETAILDIAG_H
#define NEWRETAILDIAG_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class newRetailDiag;
}

class QSqlDatabase;
class newRetailDiag : public QDialog
{
    Q_OBJECT
    int IdConsumo;
    QString Glass;
    QString Fecha;
    QSqlDatabase *db;

public:
    explicit newRetailDiag(QWidget *parent = nullptr);
    ~newRetailDiag();

    void setDb(QSqlDatabase *value);

    QString getGlass() const;
    void setGlass(const QString &value);

    int getIdConsumo() const;
    void setIdConsumo(int value);

    QString getFecha() const;
    void setFecha(const QString &value);

private slots:
    void on_pushButton_clicked();

private:
    Ui::newRetailDiag *ui;
};

#endif // NEWRETAILDIAG_H
