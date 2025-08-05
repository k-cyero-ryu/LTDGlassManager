#ifndef VIEWMOZO_H
#define VIEWMOZO_H

#include <QDialog>

namespace Ui {
class viewMozo;
}

class viewMozo : public QDialog
{
    Q_OBJECT

public:
    QString FractToDec(QString fraction);
    explicit viewMozo(QWidget *parent = nullptr, int IdOrder = NULL, QString WO = NULL);
    ~viewMozo();

private slots:
    void on_pushButton_clicked();

private:
    Ui::viewMozo *ui;
};

#endif // VIEWMOZO_H
