#ifndef DATECARGARORDENES_H
#define DATECARGARORDENES_H

#include <QDialog>

namespace Ui {
class dateCargarOrdenes;
}

class dateCargarOrdenes : public QDialog
{
    Q_OBJECT

public:
    explicit dateCargarOrdenes(QWidget *parent = nullptr);
    ~dateCargarOrdenes();

private:
    Ui::dateCargarOrdenes *ui;
};

#endif // DATECARGARORDENES_H
