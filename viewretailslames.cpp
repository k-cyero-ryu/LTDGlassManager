#include "viewretailslames.h"
#include "ui_viewretailslames.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QIcon>
#include <QUrl>
#include <QFileDialog>

viewRetailsLames::viewRetailsLames(QWidget *parent, int pIdConsumo) :
    QDialog(parent),
    ui(new Ui::viewRetailsLames)
{
    ui->setupUi(this);

    IdConsumo = pIdConsumo;
    QString where = " id_consumo = %1 ";

    QSqlTableModel *retails = new QSqlTableModel(parent);
    retails->setTable("retail");

    retails->setFilter(where.arg(IdConsumo));
    retails->select();

    QSqlTableModel *lames = new QSqlTableModel(parent);
    lames->setTable("lames_from_retail");
    lames->setFilter(where.arg(IdConsumo));
    lames->select();

    ui->tableView->setModel(retails);
    ui->tableView_2->setModel(lames);
}

viewRetailsLames::~viewRetailsLames()
{
    delete ui;
}

void viewRetailsLames::on_pushButton_clicked()
{
    this->close();
}
