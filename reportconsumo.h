#ifndef REPORTCONSUMO_H
#define REPORTCONSUMO_H

#include <QDialog>

namespace Ui {
class reportConsumo;
}

class QPrinter;

class reportConsumo : public QDialog
{
    Q_OBJECT

public:
    explicit reportConsumo(QWidget *parent = nullptr);
    ~reportConsumo();


    QString FractToDec(QString fraction);

private slots:
    void print(QPrinter *printer);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::reportConsumo *ui;
};

#endif // REPORTCONSUMO_H
