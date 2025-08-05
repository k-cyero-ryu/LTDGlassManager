#ifndef REPORTSUSPAND_H
#define REPORTSUSPAND_H

#include <QDialog>



class QSqlTableModel;
class QPrinter;

namespace Ui {
class reportSuspand;
}

class reportSuspand : public QDialog
{
    Q_OBJECT

    QSqlTableModel *orders_suspand;


public:
    explicit reportSuspand(QWidget *parent = nullptr);
    ~reportSuspand();

private slots:
    void print_orders(QPrinter *printer);
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::reportSuspand *ui;
};

#endif // REPORTSUSPAND_H
