#include "checkglasspackreport.h"
#include "ui_checkglasspackreport.h"

checkGlassPackReport::checkGlassPackReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::checkGlassPackReport)
{
    ui->setupUi(this);
}

checkGlassPackReport::~checkGlassPackReport()
{
    delete ui;
}

void checkGlassPackReport::on_pushButton_clicked()
{

}

