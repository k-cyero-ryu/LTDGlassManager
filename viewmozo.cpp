#include "viewmozo.h"
#include "ui_viewmozo.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QIcon>
#include <QUrl>
#include <QFileDialog>


viewMozo::viewMozo(QWidget *parent, int IdOrder,QString WO) :
    QDialog(parent),
    ui(new Ui::viewMozo)
{
    ui->setupUi(this);

    if (IdOrder == 0 ||
            IdOrder == -1)
    {
        ui->label_wo->setText("Old Order!");
    }
    else
    {
        QString where = " id_order = %1 ";

        QSqlTableModel *mozo = new QSqlTableModel(parent,QSqlDatabase::database("GM"));
        mozo->setTable("mozo");

        mozo->setFilter(where.arg(IdOrder));        

        ui->tableView->setModel(mozo);
        ui->label_wo->setText(WO);
        mozo->select();

        //3 cant, 4 with, 5 height

        double totalSQF = 0;
        int cant = 0;
        double parte = 0;
        double ancho = 0;
        double alto = 0;

        int TotalMozo = 0;

        QModelIndex index;
        for (int i = 0; i < ui->tableView->model()->rowCount() ; i++ )
        {
            index = ui->tableView->model()->index(i,3);
            ancho = FractToDec(ui->tableView->model()->data(index).toString()).toDouble();
            ancho = qRound(ancho * 0.5)*2;

            if (ancho < FractToDec(ui->tableView->model()->data(index).toString()).toDouble()) {ancho+=2;}

            index = ui->tableView->model()->index(i,4);
            alto = FractToDec(ui->tableView->model()->data(index).toString()).toDouble();

            alto = qRound(alto * 0.5)*2;

            if (alto < FractToDec(ui->tableView->model()->data(index).toString()).toDouble()) {alto+=2;}

            index = ui->tableView->model()->index(i,2);
            cant = ui->tableView->model()->data(index).toInt();

            parte = cant * (ancho * alto);

            TotalMozo += cant;

            totalSQF += parte;
        }

        ui->label_sqf->setText(QString::number(totalSQF/144.00,'f',2));
        ui->label_totalMozo->setText(QString::number(TotalMozo));
    }
}

viewMozo::~viewMozo()
{
    delete ui;
}

void viewMozo::on_pushButton_clicked()
{
    this->close();
}


QString viewMozo::FractToDec(QString fraction)
{
    double result;
    double entero;
    double n;
    double d;

    if (fraction.contains(' ') && fraction.contains('/'))
    {
        entero = fraction.mid(0,fraction.indexOf(' ')).toDouble();

        n = fraction.mid(fraction.indexOf(' ')+1,
            fraction.indexOf('/')-(fraction.indexOf(' ')+1)).toDouble();
        d = fraction.mid(fraction.indexOf('/')+1,fraction.size()).toDouble();

        result = entero + (n/d);
    }
    else
    {
        result = fraction.toDouble();
    }


    return QString::number(result);
}
