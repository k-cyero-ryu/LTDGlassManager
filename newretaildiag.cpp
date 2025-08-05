#include "newretaildiag.h"
#include "ui_newretaildiag.h"


#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QIcon>
#include <QUrl>
#include <QFileDialog>

void newRetailDiag::setDb(QSqlDatabase *value)
{
    db = value;
}

newRetailDiag::newRetailDiag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newRetailDiag)
{
    ui->setupUi(this);
}

QString newRetailDiag::getGlass() const
{
    return Glass;
}

void newRetailDiag::setGlass(const QString &value)
{
    Glass = value;
}

int newRetailDiag::getIdConsumo() const
{
    return IdConsumo;
}

void newRetailDiag::setIdConsumo(int value)
{
    IdConsumo = value;
}

QString newRetailDiag::getFecha() const
{
    return Fecha;
}

void newRetailDiag::setFecha(const QString &value)
{
    Fecha = value;
}

newRetailDiag::~newRetailDiag()
{
    delete ui;
}

void newRetailDiag::on_pushButton_clicked()
{
    QSqlQuery qry = QSqlQuery(db->database());

    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));


    //insertar consumo
    qryText = "INSERT INTO retail(id_consumo,glass,heigh,with,fecha) "
              "VALUES(%1,'%2',%3,%4,'%5');";

    if(!qry.exec(qryText.arg(IdConsumo).arg(Glass).
                 arg(ui->doubleSpinBox->value()).
                 arg(ui->doubleSpinBox_2->value()).
                 arg(Fecha))){
        qDebug() << "error insert" << qry.lastError().text();
        msg->setText("error insert retail");
        msg->exec();
    }
    else
    {
        msg->setText("Added");
        msg->exec();
    }
}
