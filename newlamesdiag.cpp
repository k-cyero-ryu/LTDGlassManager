#include "newlamesdiag.h"
#include "ui_newlamesdiag.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QIcon>
#include <QUrl>
#include <QFileDialog>

int newLamesDiag::getIdConsumo() const
{
    return IdConsumo;
}

void newLamesDiag::setIdConsumo(int value)
{
    IdConsumo = value;
}

QString newLamesDiag::getGlass() const
{
    return Glass;
}

QString newLamesDiag::getFecha() const
{
    return Fecha;
}

void newLamesDiag::setFecha(const QString &value)
{
    Fecha = value;
}

void newLamesDiag::setGlass(const QString &value)
{
    Glass = value;
}

void newLamesDiag::setDb(QSqlDatabase *value)
{
    db = value;
}

newLamesDiag::newLamesDiag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newLamesDiag)
{
    ui->setupUi(this);
}

newLamesDiag::~newLamesDiag()
{
    delete ui;
}

void newLamesDiag::on_pushButton_clicked()
{
    QSqlQuery qry = QSqlQuery(db->database());
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));


    //insertar consumo
    qryText = "INSERT INTO lames_from_retail(id_consumo,glass,cant,lames,fecha) "
              "VALUES(%1,'%2',%3,%4,'%5');";

    if(!qry.exec(qryText.arg(IdConsumo).arg(Glass).
                 arg(ui->spinBox->value()).
                 arg(ui->spinBox_2->value()).
                 arg(Fecha))){
        qDebug() << "error insert" << qry.lastError().text();
        msg->setText("error insert lames from retail");
        msg->exec();
    }
    else
    {
        msg->setText("Added");
        msg->exec();
    }
}
