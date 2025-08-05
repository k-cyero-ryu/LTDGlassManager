#include "datecargarordenes.h"
#include "ui_datecargarordenes.h"

dateCargarOrdenes::dateCargarOrdenes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dateCargarOrdenes)
{
    ui->setupUi(this);
}

dateCargarOrdenes::~dateCargarOrdenes()
{
    delete ui;
}
