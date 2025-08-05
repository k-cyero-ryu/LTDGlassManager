#ifndef VIEWRETAILSLAMES_H
#define VIEWRETAILSLAMES_H

#include <QDialog>

namespace Ui {
class viewRetailsLames;
}

class viewRetailsLames : public QDialog
{
    Q_OBJECT

    int IdConsumo;

public:
    explicit viewRetailsLames(QWidget *parent = nullptr, int pIdConsumo = NULL);
    ~viewRetailsLames();

private slots:
    void on_pushButton_clicked();

private:
    Ui::viewRetailsLames *ui;
};

#endif // VIEWRETAILSLAMES_H
