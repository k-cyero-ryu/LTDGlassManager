#include "frmmesas.h"
#include "ui_frmmesas.h"

frmMesas::frmMesas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmMesas)
{
    ui->setupUi(this);
}

frmMesas::~frmMesas()
{
    delete ui;
}
