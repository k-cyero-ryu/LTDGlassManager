#ifndef CHECKGLASSPACKREPORT_H
#define CHECKGLASSPACKREPORT_H

#include <QDialog>

namespace Ui {
class checkGlassPackReport;
}

class checkGlassPackReport : public QDialog
{
    Q_OBJECT

public:
    explicit checkGlassPackReport(QWidget *parent = nullptr);
    ~checkGlassPackReport();

private slots:
    void on_pushButton_clicked();

private:
    Ui::checkGlassPackReport *ui;
};

#endif // CHECKGLASSPACKREPORT_H
