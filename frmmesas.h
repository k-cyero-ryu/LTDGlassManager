#ifndef FRMMESAS_H
#define FRMMESAS_H

#include <QDialog>

namespace Ui {
class frmMesas;
}

class frmMesas : public QDialog
{
    Q_OBJECT

public:
    explicit frmMesas(QWidget *parent = nullptr);
    ~frmMesas();

private:
    Ui::frmMesas *ui;
};

#endif // FRMMESAS_H
