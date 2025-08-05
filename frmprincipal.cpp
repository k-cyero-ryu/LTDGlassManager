#include "frmprincipal.h"
#include "ui_frmprincipal.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QIcon>
#include <QUrl>
#include <QFileDialog>
#include <QPainter>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QStandardItemModel>
#include "qtableviewprinter.h"
#include <QInputDialog>
#include "reportsuspand.h"

frmPrincipal::frmPrincipal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::frmPrincipal)
{
    ui->setupUi(this);

    db = new DatabaseController();

    QMessageBox msg;
    msg.setWindowIcon(QIcon(":/Images/aim3.ico"));
    msg.setWindowTitle("GlassManager");

    if(!db->Database("GP")->isOpen())
    {
        msg.setText("GP BADDDD!");
        msg.exec();
    }
    else if(!db->Database("GM")->isOpen())
    {
        msg.setText("GM BADDDD!");
        msg.exec();
    }
    else
    {
        msg.setText("All good!");
        msg.exec();
    }

    if (!Configured())
    {
        qDebug() << "No configured!!";
        Configure();
    }


    ui->pushButton_38->setHidden(true);

    //so lets see
    consumo_r = new QSqlRelationalTableModel(this,*db->Database("GM"));

    consumo_r->setTable("consumo");

    consumo_r->setEditStrategy(QSqlTableModel::OnFieldChange);

    consumo_r->setRelation(1, QSqlRelation("orders", "id", "workorder"));

    consumo_r->setRelation(2, QSqlRelation("cuting_table", "id", "nombre"));

    consumo_r->select();

    //iniciar
    Mesas = new QSqlTableModel(this,*db->Database("GM"));
    Mesas->setTable("cuting_table");
    Mesas->select();

    Glass = new QSqlTableModel(this,*db->Database("GM"));
    Glass->setTable("glass");
    Glass->select();

    Racks = new QSqlTableModel(this,*db->Database("GM"));
    Racks->setTable("rack");
    Racks->select();

    Stock = new QSqlTableModel(this,*db->Database("GM"));
    Stock->setTable("stock");
    Stock->select();

    Lames = new QSqlTableModel(this,*db->Database("GM"));
    Lames->setTable("lames");
    Lames->select();

    LamesProd = new QSqlTableModel(this,*db->Database("GM"));
    LamesProd->setTable("lames_from_retail");
    LamesProd->select();

    LamesTrans = new QSqlTableModel(this,*db->Database("GM"));
    LamesTrans->setTable("lames_transformation");
    LamesTrans->select();

    Consumos  = new QSqlTableModel(this,*db->Database("GM"));
    Consumos->setTable("consumo");
    Consumos->select();

    Orders = new QSqlTableModel(this,*db->Database("GM"));
    Orders->setTable("orders");
    Orders->setFilter("status = 'OK Production' ");
    Orders->select();

    OrdersMaya = new QSqlTableModel(this,*db->Database("GM"));
    OrdersMaya->setTable("orders_maya");
    OrdersMaya->setFilter("status = 'OK Production' ");
    OrdersMaya->select();

    Orders_IN = new QSqlTableModel(this,*db->Database("GM"));
    Orders_IN->setTable("orders");
    Orders_IN->setFilter("status = 'IN Production' ");
    Orders_IN->select();

    Orders_Liv = new QSqlTableModel(this,*db->Database("GM"));
    Orders_Liv->setTable("orders");
    Orders_Liv->setFilter("status = 'OK Delivery' ");
    Orders_Liv->select();

    Orders_Finish = new QSqlTableModel(this,*db->Database("GM"));
    Orders_Finish->setTable("orders");
    Orders_Finish->setFilter("status = 'Finish' ");
    Orders_Finish->select();

    Orders_Search = new QSqlTableModel(this,*db->Database("GM"));
    Orders_Search->setTable("orders");
    Orders_Search->select();

    Orders_Log = new QSqlTableModel(this,*db->Database("GM"));
    Orders_Log->setTable("status_change");
    Orders_Log->select();

    ui->cmd_Orden_Mesa->setModel(Mesas);
    ui->cmd_Orden_Mesa->setModelColumn(0);

    //orders
    //QTableView *tblLan = new QTableView;
    //ui->cmb_orden_repartirorden->setView(tblLan);
    ui->cmb_orden_repartirorden->setModel(Orders);
    ui->cmb_orden_repartirorden->setModelColumn(0);

    //ui->cmb_consumo_workoerder->setView(tblLan);
    ui->cmb_consumo_workoerder->setModel(Orders_IN);
    ui->cmb_consumo_workoerder->setModelColumn(1);

    ui->cmb_consumo_client->setModel(Orders_IN);
    ui->cmb_consumo_client->setModelColumn(2);

    ui->cmb_consumo_table->setModel(Mesas);
    ui->cmb_consumo_table->setModelColumn(0);

    ui->cmb_consumo_vitre->setModel(Glass);
    ui->cmb_consumo_vitre->setModelColumn(0);

    ui->cmb_rack_glass->setModel(Glass);
    ui->cmb_rack_glass->setModelColumn(0);

    ui->cmb_rack->setModel(Racks);
    ui->cmb_rack->setModelColumn(0);

    ui->cmb_stock_glass->setModel(Glass);
    ui->cmb_stock_glass->setModelColumn(0);

    ui->cmb_orden_finalizar->setModel(Orders_IN);
    ui->cmb_orden_finalizar->setModelColumn(1);

    ui->cmb_livre->setModel(Orders_Liv);
    ui->cmb_livre->setModelColumn(1);

    ui->cmb_lames_vitre->setModel(Glass);
    ui->cmb_lames_vitre->setModelColumn(0);

    ui->cmb_lames_vitre_2->setModel(Glass);
    ui->cmb_lames_vitre_2->setModelColumn(0);

    ui->cmb_lames_vitre_3->setModel(Glass);
    ui->cmb_lames_vitre_3->setModelColumn(0);

    ui->cmb_lames_vitre_4->setModel(Glass);
    ui->cmb_lames_vitre_4->setModelColumn(0);

    ui->cmb_cons_table->setModel(Mesas);
    ui->cmb_cons_table->setModelColumn(0);

    ui->cmb_cons_glass->setModel(Glass);
    ui->cmb_cons_glass->setModelColumn(0);

    //ui->cmb_cons_workorder->setModel(Orders_Search);
    //ui->cmb_cons_workorder->setModelColumn(0);


    //Workorders

    Orders->setHeaderData(9, Qt::Horizontal, QObject::tr("ID"));
    Orders->setHeaderData(0, Qt::Horizontal, QObject::tr("Wororder"));
    Orders->setHeaderData(1, Qt::Horizontal, QObject::tr("Invoice"));
    Orders->setHeaderData(2, Qt::Horizontal, QObject::tr("Quote"));
    Orders->setHeaderData(3, Qt::Horizontal, QObject::tr("Client"));
    Orders->setHeaderData(4, Qt::Horizontal, QObject::tr("Invoice Date"));
    Orders->setHeaderData(5, Qt::Horizontal, QObject::tr("Status"));
    Orders->setHeaderData(6, Qt::Horizontal, QObject::tr("Livre Depot"));
    Orders->setHeaderData(7, Qt::Horizontal, QObject::tr("Livre LTD"));
    Orders->setHeaderData(8, Qt::Horizontal, QObject::tr("Cuting Table"));

    ui->tableView->setModel(Orders);
    Orders->select();

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);

    //en coupe
    Orders_IN->setHeaderData(9, Qt::Horizontal, QObject::tr("ID"));
    Orders_IN->setHeaderData(0, Qt::Horizontal, QObject::tr("Wororder"));
    Orders_IN->setHeaderData(1, Qt::Horizontal, QObject::tr("Invoice"));
    Orders_IN->setHeaderData(2, Qt::Horizontal, QObject::tr("Quote"));
    Orders_IN->setHeaderData(3, Qt::Horizontal, QObject::tr("Client"));
    Orders_IN->setHeaderData(4, Qt::Horizontal, QObject::tr("Invoice Date"));
    Orders_IN->setHeaderData(5, Qt::Horizontal, QObject::tr("Status"));
    Orders_IN->setHeaderData(6, Qt::Horizontal, QObject::tr("Livre Depot"));
    Orders_IN->setHeaderData(7, Qt::Horizontal, QObject::tr("Livre LTD"));
    Orders_IN->setHeaderData(8, Qt::Horizontal, QObject::tr("Cuting Table"));

    ui->table_Order_IN->setModel(Orders_IN);

    ui->table_Order_IN->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->table_Order_IN->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->table_Order_IN->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->table_Order_IN->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->table_Order_IN->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->table_Order_IN->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->table_Order_IN->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->table_Order_IN->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->table_Order_IN->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->table_Order_IN->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);

    //coupe
    Orders_Liv->setHeaderData(9, Qt::Horizontal, QObject::tr("ID"));
    Orders_Liv->setHeaderData(0, Qt::Horizontal, QObject::tr("Wororder"));
    Orders_Liv->setHeaderData(1, Qt::Horizontal, QObject::tr("Invoice"));
    Orders_Liv->setHeaderData(2, Qt::Horizontal, QObject::tr("Quote"));
    Orders_Liv->setHeaderData(3, Qt::Horizontal, QObject::tr("Client"));
    Orders_Liv->setHeaderData(4, Qt::Horizontal, QObject::tr("Invoice Date"));
    Orders_Liv->setHeaderData(5, Qt::Horizontal, QObject::tr("Status"));
    Orders_Liv->setHeaderData(6, Qt::Horizontal, QObject::tr("Livre Depot"));
    Orders_Liv->setHeaderData(7, Qt::Horizontal, QObject::tr("Livre LTD"));
    Orders_Liv->setHeaderData(8, Qt::Horizontal, QObject::tr("Cuting Table"));

    ui->table_livre->setModel(Orders_Liv);

    ui->table_livre->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->table_livre->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->table_livre->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->table_livre->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->table_livre->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->table_livre->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->table_livre->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->table_livre->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->table_livre->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->table_livre->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);

    //coupe
    Orders_Finish->setHeaderData(9, Qt::Horizontal, QObject::tr("ID"));
    Orders_Finish->setHeaderData(0, Qt::Horizontal, QObject::tr("Wororder"));
    Orders_Finish->setHeaderData(1, Qt::Horizontal, QObject::tr("Invoice"));
    Orders_Finish->setHeaderData(2, Qt::Horizontal, QObject::tr("Quote"));
    Orders_Finish->setHeaderData(3, Qt::Horizontal, QObject::tr("Client"));
    Orders_Finish->setHeaderData(4, Qt::Horizontal, QObject::tr("Invoice Date"));
    Orders_Finish->setHeaderData(5, Qt::Horizontal, QObject::tr("Status"));
    Orders_Finish->setHeaderData(6, Qt::Horizontal, QObject::tr("Livre Depot"));
    Orders_Finish->setHeaderData(7, Qt::Horizontal, QObject::tr("Livre LTD"));
    Orders_Finish->setHeaderData(8, Qt::Horizontal, QObject::tr("Cuting Table"));

    ui->table_finalize->setModel(Orders_Finish);

    ui->table_finalize->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->table_finalize->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->table_finalize->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->table_finalize->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->table_finalize->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->table_finalize->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->table_finalize->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->table_finalize->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->table_finalize->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->table_finalize->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);

    //search
    Orders_Search->setHeaderData(9, Qt::Horizontal, QObject::tr("ID"));
    Orders_Search->setHeaderData(0, Qt::Horizontal, QObject::tr("Wororder"));
    Orders_Search->setHeaderData(1, Qt::Horizontal, QObject::tr("Invoice"));
    Orders_Search->setHeaderData(2, Qt::Horizontal, QObject::tr("Quote"));
    Orders_Search->setHeaderData(3, Qt::Horizontal, QObject::tr("Client"));
    Orders_Search->setHeaderData(4, Qt::Horizontal, QObject::tr("Invoice Date"));
    Orders_Search->setHeaderData(5, Qt::Horizontal, QObject::tr("Status"));
    Orders_Search->setHeaderData(6, Qt::Horizontal, QObject::tr("Livre Depot"));
    Orders_Search->setHeaderData(7, Qt::Horizontal, QObject::tr("Livre LTD"));
    Orders_Search->setHeaderData(8, Qt::Horizontal, QObject::tr("Cuting Table"));

    ui->tableView_search->setModel(Orders_Search);

    ui->tableView_search->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_search->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView_search->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView_search->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView_search->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableView_search->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->tableView_search->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->tableView_search->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->tableView_search->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->tableView_search->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);

    ui->tableStatusChange->setModel(Orders_Log);

    //CONSUMOS
    consumo_r->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    consumo_r->setHeaderData(1, Qt::Horizontal, QObject::tr("WorkOrder"));
    consumo_r->setHeaderData(2, Qt::Horizontal, QObject::tr("Table"));
    consumo_r->setHeaderData(3, Qt::Horizontal, QObject::tr("Sheets"));
    consumo_r->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));
    consumo_r->setHeaderData(5, Qt::Horizontal, QObject::tr("Extra"));

    ui->tableView_consumos->setModel(consumo_r);
    consumo_r->select();

    ui->tableView_consumos->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_consumos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView_consumos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView_consumos->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView_consumos->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableView_consumos->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    //GLASS STOCK
    Glass->setHeaderData(0, Qt::Horizontal, QObject::tr("Glass"));
    Glass->setHeaderData(1, Qt::Horizontal, QObject::tr("Thicknes"));
    Glass->setHeaderData(2, Qt::Horizontal, QObject::tr("With"));
    Glass->setHeaderData(3, Qt::Horizontal, QObject::tr("Heigh"));
    Glass->setHeaderData(4, Qt::Horizontal, QObject::tr("ID"));
    Glass->setHeaderData(5, Qt::Horizontal, QObject::tr("Cant"));

    ui->tableView_stock->setModel(Glass);
    Glass->select();

    ui->tableView_stock->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_stock->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView_stock->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView_stock->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView_stock->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableView_stock->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    //GLASS RACK STOCK
    Stock->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    Stock->setHeaderData(1, Qt::Horizontal, QObject::tr("Rack"));
    Stock->setHeaderData(2, Qt::Horizontal, QObject::tr("Glass"));
    Stock->setHeaderData(3, Qt::Horizontal, QObject::tr("Cant"));

    ui->tableView_stock_rack->setModel(Stock);
    Stock->select();

    ui->tableView_stock_rack->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_stock_rack->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView_stock_rack->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView_stock_rack->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);


    //lames
    Lames->setHeaderData(0, Qt::Horizontal, QObject::tr("Glass"));
    Lames->setHeaderData(1, Qt::Horizontal, QObject::tr("Cant"));
    Lames->setHeaderData(2, Qt::Horizontal, QObject::tr("Lames"));
    Lames->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    Lames->setHeaderData(4, Qt::Horizontal, QObject::tr("DPT"));
    Lames->setHeaderData(5, Qt::Horizontal, QObject::tr("ID"));

    ui->tableView_lames_stock->setModel(Lames);
    Lames->select();

    ui->tableView_lames_stock->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_lames_stock->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView_lames_stock->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView_lames_stock->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView_lames_stock->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableView_lames_stock->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    //LAMES PROD
    LamesProd->setHeaderData(0, Qt::Horizontal, QObject::tr("Glass"));
    LamesProd->setHeaderData(1, Qt::Horizontal, QObject::tr("Cant"));
    LamesProd->setHeaderData(2, Qt::Horizontal, QObject::tr("Lames"));
    LamesProd->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    LamesProd->setHeaderData(4, Qt::Horizontal, QObject::tr("ID"));

    ui->tableView_lames_prod->setModel(LamesProd);
    LamesProd->select();

    ui->tableView_lames_prod->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_lames_prod->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView_lames_prod->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView_lames_prod->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView_lames_prod->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    //LAMES TRANS
    LamesTrans->setHeaderData(0, Qt::Horizontal, QObject::tr("Glass"));
    LamesTrans->setHeaderData(1, Qt::Horizontal, QObject::tr("Sheets"));
    LamesTrans->setHeaderData(2, Qt::Horizontal, QObject::tr("Cant"));
    LamesTrans->setHeaderData(3, Qt::Horizontal, QObject::tr("Lames"));
    LamesTrans->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));
    LamesTrans->setHeaderData(5, Qt::Horizontal, QObject::tr("ID"));

    ui->tableView_lams_transf->setModel(LamesTrans);
    LamesTrans->select();

    ui->tableView_lams_transf->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView_lams_transf->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView_lams_transf->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView_lams_transf->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView_lams_transf->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableView_lams_transf->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);


    ui->date_Order_Finish->setDate(QDateTime::currentDateTime().date());
    ui->date_delivery->setDate(QDateTime::currentDateTime().date());
    ui->date_CargarOrdenes->setDate(QDateTime::currentDateTime().date());
    ui->date_search->setDate(QDateTime::currentDateTime().date());
    ui->date_invoice_from->setDate(QDateTime::currentDateTime().date());
    ui->date_invoice_to->setDate(QDateTime::currentDateTime().date());
    ui->date_log->setDate(QDateTime::currentDateTime().date());
    ui->date_cons->setDate(QDateTime::currentDateTime().date());
    ui->date_livre->setDate(QDateTime::currentDateTime().date());
    ui->dateRepConsumoGPTotal_from->setDate(QDateTime::currentDateTime().date());
    ui->dateRepConsumoGPTotal_to->setDate(QDateTime::currentDateTime().date());
    ui->dateRepUsoMesaFrom->setDate(QDateTime::currentDateTime().date());
    ui->dateRepUsoMesaTo->setDate(QDateTime::currentDateTime().date());
    ui->dateRepUsoDptFrom->setDate(QDateTime::currentDateTime().date());
    ui->dateRepUsoDptTo->setDate(QDateTime::currentDateTime().date());
    ui->dateRepDPTvsGPFrom->setDate(QDateTime::currentDateTime().date());
    ui->dateRepDPTvsGPTo->setDate(QDateTime::currentDateTime().date());
    ui->dateRepDPTLamesFrom->setDate(QDateTime::currentDateTime().date());
    ui->dateRepDPTLamesTo->setDate(QDateTime::currentDateTime().date());
    ui->dateRepDPTvsGPFrom_2->setDate(QDateTime::currentDateTime().date());
    ui->dateRepDPTvsGPTo_2->setDate(QDateTime::currentDateTime().date());
    ui->dateEdit_lames_date->setDate(QDateTime::currentDateTime().date());
    ui->date_livre_2->setDate(QDateTime::currentDateTime().date());
    ui->dateEdit_lames_date_2->setDate(QDateTime::currentDateTime().date());
    ui->date_livre_3->setDate(QDateTime::currentDateTime().date());

    bool ok;
    QString text = QInputDialog::getText(this, tr("Password"),
                                         tr("Password:"), QLineEdit::Password,
                                         "", &ok);
    if (ok && text == "depo123")
    {
        LoggedUser = "Depo Manager";
    }
    else if (ok && text == "ltd123")
    {
        LoggedUser = "Livrasion Manager";
    }
    else if (ok && text == "vitre123")
    {
        LoggedUser = "Vitre Director";
    }
    else
    {
        LoggedUser = "User";
        ui->centralwidget->setDisabled(true);
    }

    this->setWindowTitle("GlassManager - "+LoggedUser);

    timerId = startTimer(3000000);
}

frmPrincipal::~frmPrincipal()
{
    killTimer(timerId);
    delete ui;
}

DatabaseController *frmPrincipal::GetDatabase()
{
    return db;
}
/*
connection *frmPrincipal::GetDatabase()
{
    return db;
}*/

void frmPrincipal::Configure()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));


    //add datos por defecto

    //mesas
    qryText = "INSERT INTO cuting_table(nombre) VALUES('%1');";

    if(!qry.exec(qryText.arg("Tab #1"))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }

    if(!qry.exec(qryText.arg("Tab #2"))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }

    if(!qry.exec(qryText.arg("Tab #3"))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }

    if(!qry.exec(qryText.arg("CNC #1"))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }

    if(!qry.exec(qryText.arg("CNC #2"))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }

    //maquinaria
    if(!qry.exec(qryText.arg("Pulidora Lamas"))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }
    if(!qry.exec(qryText.arg("Pulidora Laminados #1"))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }
    if(!qry.exec(qryText.arg("Pulidora Laminados #2"))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }
    if(!qry.exec(qryText.arg("OKIDATA Impresora"))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }
    if(!qry.exec(qryText.arg("ZDezigner Impresora"))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }
    if(!qry.exec(qryText.arg("HP Laser Impresora"))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error tabla cuting_table");
        msg->exec();
    }

    //cristales
    qryText = "INSERT INTO glass(cant,color,grosor,alto,ancho)"
              "VALUES (0,'%1',%2,%3,%4)";

    if(!qry.exec(QString(qryText).arg("Bronze 3/16")
                 .arg(5)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }

    if(!qry.exec(QString(qryText).arg("Bronze 1/4")
                 .arg(6)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Clear 1/4")
                 .arg(6)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Bronze Opac 3/16")
                 .arg(5.5)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Clear Opac 3/16")
                 .arg(5.5)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Dark Grey 3/16")
                 .arg(5)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Bronze Reflective 3/16")
                 .arg(5)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Bronze Laminated 1/2")
                 .arg(10)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Clear Laminated 1/2")
                 .arg(10)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Bronze Impact")
                 .arg(12)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Clear Impact")
                 .arg(12)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Mirror Silvered 3/16")
                 .arg(5)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Mirror Silvered 1/4")
                 .arg(6)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Clear Laminated 3/16")
                 .arg(6)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Opac Acidit")
                 .arg(6)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("Dark Blue")
                 .arg(6)
                 .arg(134)
                 .arg(84))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }

    //racks
    qryText = "INSERT INTO rack(nombre,dimension)"
              "VALUES ('%1',%2)";

    if(!qry.exec(QString(qryText).arg("R1").arg(2020))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R2").arg(1945))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R3").arg(2055))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R4A").arg(680))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R4B").arg(680))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R5A").arg(505))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R5B").arg(500))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R6A").arg(520))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R6B").arg(480))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R7A").arg(510))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R7B").arg(500))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R8A").arg(510))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R8B").arg(505))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R9").arg(2130))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R10").arg(2270))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R11").arg(2130))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R12").arg(2130))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R13").arg(2400))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R14A").arg(1010))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R14B").arg(1005))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R15A").arg(980))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R15B").arg(1040))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R16A").arg(660))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R16B").arg(670))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R17A").arg(625))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R17B").arg(700))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R18A").arg(650))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R18B").arg(680))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R19A").arg(680))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R19B").arg(710))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R0A").arg(500))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
    if(!qry.exec(QString(qryText).arg("R0B").arg(500))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }
}

bool frmPrincipal::Configured()
{
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText = "SELECT count(id) FROM cuting_table";

    if(!qry.exec(QString(qryText))){
        msg->setText("error " + qry.lastError().text());
        msg->exec();
    }

    if (qry.isActive())
    {
        if (qry.isSelect())
        {
            qDebug() << "configured heck done!";
            while ( qry.next() )
            {
                if (qry.value(0).toInt() > 0)
                {
                    //qDebug() << qry.value(0).toString() ;
                    return true;
                }else {
                    return false;
                }
            }

        }
    }
    return false;
}

bool frmPrincipal::InsertMozo(QString WorkOrder, int IdOrder)
{
    QSqlQuery qry2 = QSqlQuery(*db->Database("GM"));
    QSqlQuery qry = QSqlQuery(*db->Database("GP"));
    QString qryText;

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText =   "SELECT   OrderQty "
                        ",Dimension1"
                        ",Dimension2"
                        ",WorkorderNumber"
                        ",PartNumber"
                        ",ProductSize"
                        ",DocumentDateTime "
                "FROM [GlasPacLX_LTD].[dbo].[Document] INNER JOIN "
                    "[GlasPacLX_LTD].[dbo].[DocumentDetail] "
                "ON [DocumentGUID] = [DocumentGUID_FK] "
                "WHERE DocumentType_ENUM = 2 AND BranchGUID_FK = '65F02099-8E4C-4483-91B8-90AEF2838E76' "
                "AND WorkorderNumber = '%1' OR DocumentNumber = '%1' "
                "ORDER BY ProductSize,PartNumber";

    /*

SELECT   DocumentNumber,WorkorderNumber,QuoteNumber,OrderQty,Dimension1,Dimension2,PartNumber,ProductSize,PricingDate,DateTimeLastUpdated,DocumentDateTime
FROM [GlasPacLX_LTD].[dbo].[Document]
    INNER JOIN [GlasPacLX_LTD].[dbo].[DocumentDetail]
    ON [DocumentGUID] = [DocumentGUID_FK]
    WHERE DocumentType_ENUM = 2 AND BranchGUID_FK = '65F02099-8E4C-4483-91B8-90AEF2838E76'
    and PricingDate > '2023-08-1 00:00:00.000' and PricingDate < '2023-08-30 00:00:00.000'
    ORDER BY ProductSize,PartNumber


     */

    if(!qry.exec(qryText.arg(WorkOrder)))
    {
        qDebug() << "error load mozos" << qry.lastError().text();
        msg->setText("error load mozos");
        msg->exec();
    }
    else
    {
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                while ( qry.next() )
                {
                    qryText = "INSERT INTO mozo("
                                      "id_order,glass,cant,heigh_,with_,date) "
                              "VALUES(%1,'%2',%3,'%4','%5','%6');";

                    QString j = qry.value(6).toString();
                    j.truncate(10);
                    if (qry.value(4).toString() != "MISC")
                    {
                        if(!qry2.exec(qryText
                                      .arg(IdOrder)
                                      .arg(qry.value(4).toString() +" "+
                                           qry.value(5).toString())
                                      .arg(qry.value(0).toInt())
                                      .arg(qry.value(1).toString())
                                      .arg(qry.value(2).toString())
                                      .arg(j)))
                        {
                            qDebug() << "error insert mozos" << qry2.lastError().text();
                            return false;
                        }
                    }                    
                }
                return true;
            }
        }
    }
    return false;
}

bool frmPrincipal::InsertMozoMaya(QString WorkOrder, int IdOrder)
{
    QSqlQuery qry2 = QSqlQuery(*db->Database("GM"));
    QSqlQuery qry = QSqlQuery(*db->Database("GP"));
    QString qryText;

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText =   "SELECT   OrderQty "
              ",Dimension1"
              ",Dimension2"
              ",WorkorderNumber"
              ",PartNumber"
              ",ProductSize"
              ",DocumentDateTime "
              "FROM [GlasPacLX_LTD].[dbo].[Document] INNER JOIN "
              "[GlasPacLX_LTD].[dbo].[DocumentDetail] "
              "ON [DocumentGUID] = [DocumentGUID_FK] "
              "WHERE DocumentType_ENUM = 2 AND BranchGUID_FK <> '65F02099-8E4C-4483-91B8-90AEF2838E76' "
              "AND WorkorderNumber = '%1' OR DocumentNumber = '%1' "
              "ORDER BY ProductSize,PartNumber";

    /*

SELECT   DocumentNumber,WorkorderNumber,QuoteNumber,OrderQty,Dimension1,Dimension2,PartNumber,ProductSize,PricingDate,DateTimeLastUpdated,DocumentDateTime
FROM [GlasPacLX_LTD].[dbo].[Document]
    INNER JOIN [GlasPacLX_LTD].[dbo].[DocumentDetail]
    ON [DocumentGUID] = [DocumentGUID_FK]
    WHERE DocumentType_ENUM = 2 AND BranchGUID_FK = '65F02099-8E4C-4483-91B8-90AEF2838E76'
    and PricingDate > '2023-08-1 00:00:00.000' and PricingDate < '2023-08-30 00:00:00.000'
    ORDER BY ProductSize,PartNumber


     */

    if(!qry.exec(qryText.arg(WorkOrder)))
    {
        qDebug() << "error load mozos_maya" << qry.lastError().text();
        msg->setText("error load mozos_maya");
        msg->exec();
    }
    else
    {
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                while ( qry.next() )
                {
                    qryText = "INSERT INTO mozo_maya("
                              "id_order,glass,cant,heigh_,with_,date) "
                              "VALUES(%1,'%2',%3,'%4','%5','%6');";

                    QString j = qry.value(6).toString();
                    j.truncate(10);
                    if (qry.value(4).toString() != "MISC")
                    {
                        if(!qry2.exec(qryText
                                           .arg(IdOrder)
                                           .arg(qry.value(4).toString() +" "+
                                                qry.value(5).toString())
                                           .arg(qry.value(0).toInt())
                                           .arg(qry.value(1).toString())
                                           .arg(qry.value(2).toString())
                                           .arg(j)))
                        {
                            qDebug() << "error insert mozos_maya" << qry2.lastError().text();
                            return false;
                        }
                    }
                }
                return true;
            }
        }
    }
    return false;
}

QString frmPrincipal::StatusByWorkOrder(QString WO)
{

}

int frmPrincipal::IdByWorkOrder(QString WO)
{
    QSqlQuery qry = QSqlQuery(*db->Database("GP"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));
    QSqlQuery qry2 = QSqlQuery(*db->Database("GM"));

    qryText =   "SELECT id "
                "FROM orders "
                "WHERE workorder = '%1' ";

    if(!qry2.exec(qryText.arg(WO)))
    {
        qDebug() << "error load status from workorder" << qry2.lastError().text();
        qDebug() << qry2.executedQuery();
        msg->setText("error load status from workorder");
        msg->exec();
    }
    else
    {
        if (qry2.isActive())
        {
            if (qry2.isSelect())
            {
                while ( qry2.next() )
                {
                    qDebug() << qry2.value(0).toInt();
                    return qry2.value(0).toInt();
                }
            }
        }
    }
    return -1;
}

bool frmPrincipal::InsertStatusChange(int IdOrder, QString StatusChange, QString User, QString Date)
{
    QSqlQuery qry2 = QSqlQuery(*db->Database("GM"));
    QSqlQuery qry = QSqlQuery(*db->Database("GP"));
    QString qryText;

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText = "INSERT INTO status_change("
                      "id_order,status_change,user,date) "
              "VALUES(%1,'%2','%3','%4');";

    if(!qry2.exec(qryText
                  .arg(IdOrder)
                  .arg(StatusChange)
                  .arg(User)
                  .arg(Date)))
    {
        qDebug() << "error insert status change" << qry2.lastError().text();
        return false;
    }
    return true;
}

void frmPrincipal::print_find2(QPrinter *printer)
{
    //Definir las propiedades del papel.
    printer->setPageSize(QPageSize::A4);
    //Declarar pincel
    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "Error al abrir la impresión!";
        return;
    }
    //Declarar tablePrinter
    QTableViewPrinter tablePrinter(&painter, printer);
    QVector<int> colWidth;
    for(int i=0;i<ui->tableView_search->model()->columnCount();++i){
        colWidth.push_back(ui->tableView_search->columnWidth(i));
    }

    //Establecer fuente
    QFont headerFont;
    headerFont.setBold(true);
    QFont contentFont;
    contentFont.setPointSize(8);
    tablePrinter.setContentFont(contentFont);

    //Establecer título
    PageTitle* title=new PageTitle(&painter,printer);
    title->setPageTitle("LTD Construction S.A");
    title->setMtitle2(ui->label->text());//report Name
    QStringList side = { "Phone: +(509) 2813-4040     Email: info@ltdconstruction-haiti.com",
                         "From: "+ui->date_search->date().toString(),
                         "    ",
                         };
    /*"From "+ui->date_from->date().toString() +
                         "To "+ui->date_to->date().toString()
    */
    title->setSideTitle(side);
    tablePrinter.setPagerTitle(title);
    tablePrinter.setTitleFlag(TitleFlag::FIRST_PAGE);

    //Establecer pie de página
    PageFooter* pagefooter=new PageFooter(&painter);
    pagefooter->setCreater("Footer");
    tablePrinter.setPagerFooter(pagefooter);

    QStringList headers;
    QStringList headers2;
    for(int i = 0; i < ui->tableView_search->model()->columnCount(); i++)
    {
      headers.append(ui->tableView_search->model()->headerData(i, Qt::Horizontal).toString());
      headers2.append(ui->tableView_search->model()->headerData(i, Qt::Horizontal).toString());
    }

    //QStringList headers = { "título 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    //QStringList headers2 = { "Encabezado 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    if(!tablePrinter.printTable(ui->tableView_search, headers, headers2)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();
}

void frmPrincipal::print_workorder(QPrinter *printer)
{
    //Definir las propiedades del papel.
    printer->setPageSize(QPageSize::A4);
    //Declarar pincel
    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "Error al abrir la impresión!";
        return;
    }
    //Declarar tablePrinter
    QTableViewPrinter tablePrinter(&painter, printer);
    QVector<int> colWidth;
    for(int i=0;i<ui->tableView->model()->columnCount();++i){
        colWidth.push_back(ui->tableView->columnWidth(i));
    }

    //Establecer fuente
    QFont headerFont;
    headerFont.setBold(true);
    QFont contentFont;
    contentFont.setPointSize(8);
    tablePrinter.setContentFont(contentFont);

    //Establecer título
    PageTitle* title=new PageTitle(&painter,printer);
    title->setPageTitle("LTD Construction S.A");
    title->setMtitle2("WorkOrders for: "+ui->date_CargarOrdenes->date().toString());//report Name
    QStringList side = { "Phone: +(509) 2813-4040     Email: info@ltdconstruction-haiti.com",
                         "From: "+ui->date_CargarOrdenes->date().toString(),
                         "    ",
                         };
    /*"From "+ui->date_from->date().toString() +
                         "To "+ui->date_to->date().toString()
    */
    title->setSideTitle(side);
    tablePrinter.setPagerTitle(title);
    tablePrinter.setTitleFlag(TitleFlag::FIRST_PAGE);

    //Establecer pie de página
    PageFooter* pagefooter=new PageFooter(&painter);
    pagefooter->setCreater("Footer");
    tablePrinter.setPagerFooter(pagefooter);

    QStringList headers;
    QStringList headers2;
    for(int i = 0; i < ui->tableView->model()->columnCount(); i++)
    {
      headers.append(ui->tableView->model()->headerData(i, Qt::Horizontal).toString());
      headers2.append(ui->tableView->model()->headerData(i, Qt::Horizontal).toString());
    }

    //QStringList headers = { "título 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    //QStringList headers2 = { "Encabezado 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    if(!tablePrinter.printTable(ui->tableView, headers, headers2)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();
}

void frmPrincipal::on_actionCargar_Ordenes_triggered()
{
    QSqlQuery qry2 = QSqlQuery(*db->Database("GM"));
    QSqlQuery qry = QSqlQuery(*db->Database("GP"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText = "SELECT DocumentDateTime"
                      ",BillToID"
                      ",WorkorderNumber "
                      ",DocumentNumber "
                      ",QuoteNumber "
              "FROM [GlasPacLX_LTD].[dbo].[Document] INNER JOIN "
                    "[GlasPacLX_LTD].[dbo].[DocumentDetail] "
                    "ON [DocumentGUID] = [DocumentGUID_FK] "
              "WHERE DocumentType_ENUM <> 0 AND BranchGUID_FK = '65F02099-8E4C-4483-91B8-90AEF2838E76' "
                    "AND DocumentDateTime >= '%1' AND DocumentDateTime < '%2' "
              "GROUP BY WorkorderNumber,DocumentDateTime,BillToID,DocumentNumber,QuoteNumber "
              "ORDER BY WorkorderNumber;";

    //"AND WorkorderNumber = '%1' OR DocumentNumber = '%1' "
    //DocumentType_ENUM ---->>> 0 Quote -- 1 WOrkOrder --- 2 Invoice

    //QString d = "30/01/2023";
    QDateTime dateFrom;
    QDateTime dateTo;
    //date.setDate(QDate::fromString(d,"dd/MM/yyyy"));
    dateFrom.setDate(ui->date_CargarOrdenes->date());
    dateFrom.setTime(QTime(0,0,0,0));
    dateTo = dateFrom.addDays(1);

    if(!qry.exec(qryText.arg(dateFrom.toString("MM-dd-yyyy hh:mm:ss")).arg(dateTo.toString("MM-dd-yyyy hh:mm:ss"))))
    {
        qDebug() << "error load order" << qry.lastError().text();
        qDebug() << qryText.arg(dateFrom.toString("MM-dd-yyyy hh:mm:ss")).arg(dateTo.toString("MM-dd-yyyy hh:mm:ss"));
        msg->setText("error load order");
        msg->exec();
    }
    else
    {
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                /*
                QSqlQueryModel *test = new QSqlQueryModel(this);
                test->setQuery(qry);
                ui->tableView->setModel(test);
                */

                while ( qry.next() )
                {
                    qryText = "INSERT INTO orders("
                              "workorder,invoice,quote,client_name,date,status,livre_1,livre_2) "
                      "VALUES('%1','%2','%3','%4','%5','%6','%7','%8');";

                    QString j = qry.value(0).toString();
                    j.truncate(10);
                    qDebug() << QDateTime::fromString(j,"yyyy-MM-dd").toString("yy-MM-dd");

                    if(!qry2.exec(qryText
                                  .arg(qry.value(2).toString())
                                  .arg(qry.value(3).toString())
                                  .arg(qry.value(4).toString())
                                  .arg(qry.value(1).toString().remove("'"))
                                  .arg(j)
                                  .arg("OK Production")
                                  .arg("")
                                  .arg("")))
                    {
                        qDebug() << "error orders" << qry2.lastError().text();
                        qDebug() << qry.value(2).toString() + "-" +qry.value(1).toString();
                        if (!qry2.lastError().text().contains("Violation of UNIQUE KEY constraint"))
                        {
                            msg->setText("error inserting orders - "+qry2.lastQuery()+" "+qry2.lastError().text());
                            msg->exec();
                        }
                    }
                    else
                    {
                        qDebug() << qry.value(2).toString() + "-" +qry.value(1).toString();
                        qDebug() << qry2.lastInsertId().toInt();
                        InsertMozo(qry.value(2).toString(), qry2.lastInsertId().toInt());
                        InsertStatusChange(qry2.lastInsertId().toInt(),"Created",LoggedUser,j);
                    }
                }

                msg->setText("Ordenes cargadas correctamente!");
                msg->exec();

                Orders->select();

                ui->tableView->setModel(Orders);
            }
        }

    }

    Orders_IN->select();
    Orders_Liv->select();
}



void frmPrincipal::on_tabWidget_currentChanged(int index)
{
    if (ui->tabWidget->currentIndex() == 0)
    {

    }
}

void frmPrincipal::on_pushButton_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    QString id_order = ui->cmb_orden_repartirorden->currentText();
    QString table = ui->cmd_Orden_Mesa->currentText();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Finish Order", "Are you sure to asign this order?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qryText =   "UPDATE orders "
                    "SET cuting_table = '%1', status = 'IN Production' "
                    "WHERE workorder = '%2';";

        if (!qry.exec(qryText.arg(table).arg(id_order)))
        {
            qDebug() << "error update order" << qry.lastError().text();
            msg->setText("error update order");
            msg->exec();
        }
        else
        {
            int idOr = IdByWorkOrder(id_order);
            InsertStatusChange(idOr,"to En Coupe",LoggedUser,QDate::currentDate().toString("yyyy-MM-dd"));
        }
    }
    Orders->select();
    Orders_IN->select();
    Orders_Liv->select();
}

void frmPrincipal::on_actionHacer_Backup_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,
             tr("Save DB File"), "", tr("DB Files (*.db)"));

    if (QFile::exists(path))
    {
        QFile::remove(path);
    }

    QFile::copy("GlassManager.db", path);
}

void frmPrincipal::on_tableView_doubleClicked(const QModelIndex &index)
{
    int id_order = ui->tableView->model()->index(index.row(),9).data().toInt();
    QString work_order = ui->tableView->model()->index(index.row(),0).data().toString();

    ui->cmb_orden_repartirorden->setCurrentText(work_order);


    viewMozo *frm = new viewMozo(this,id_order,work_order);
    frm->setModal(true);
    frm->show();
    //qDebug() << workorder;
}

QString FractToDec(QString fraction)
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

void frmPrincipal::on_pushButton_11_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));


    //insertar consumo
    qryText = "INSERT INTO consumo(id_order,id_table,glass,sheets,fecha,extra) "
              "VALUES(%1,%2,'%3',%4,'%5','%6');";

    int row = ui->cmb_consumo_workoerder->currentIndex();
    QModelIndex idx = ui->cmb_consumo_workoerder->model()->index(row, 0); // first column
    QVariant data = ui->cmb_consumo_workoerder->model()->data(idx);
    int type_id = data.toInt();

    int row2 = ui->cmb_consumo_table->currentIndex();
    QModelIndex idx2 = ui->cmb_consumo_table->model()->index(row2, 0); // first column
    QVariant data2 = ui->cmb_consumo_table->model()->data(idx2);
    int type_id2 = data2.toInt();

    QString glass = ui->cmb_consumo_vitre->currentText();
    int cant = ui->spb_consumo_cant->value();
    QString fecha = QDate::currentDate().toString("yyyy-MM-dd");
    QString extra = ui->le_consumo_extra->text();


    if(!qry.exec(qryText.arg(type_id).arg(type_id2).arg(glass).
                 arg(cant).arg(fecha).arg(extra))){
        qDebug() << "error tabla" << qry.lastError().text();
        msg->setText("error insert consumo");
        msg->exec();
    }
    else
    {
        Consumos->select();
        consumo_r->select();

        msg->setText("Added");
        msg->exec();

    }
}

void frmPrincipal::on_actionSalir_triggered()
{
    this->close();
}

void frmPrincipal::on_btn_consumo_plusretail_clicked()
{    
    /*QModelIndex index = ui->tableView_consumo->selectionModel()->selectedIndexes().first();

    int row = index.row();
    IdConsumo = ui->tableView_consumo->model()->data(
        ui->tableView_consumo->model()->index(row,0,QModelIndex())).toInt();

    QString glass = ui->tableView_consumo->model()->data(
                ui->tableView_consumo->model()->index(row,3,QModelIndex())).toString();

    QString fecha = ui->tableView_consumo->model()->data(
                ui->tableView_consumo->model()->index(row,5,QModelIndex())).toString();

    newRetailDiag *frm = new newRetailDiag(this);
    frm->setModal(true);
    frm->setDb(db->Database("GP"));
    frm->setIdConsumo(IdConsumo);
    frm->setGlass(glass);
    frm->setFecha(fecha);
    frm->show();*/
}

void frmPrincipal::on_btn_consumo_pluslames_clicked()
{
    /*QModelIndex index = ui->tableView_consumo->selectionModel()->selectedIndexes().first();

    int row = index.row();
    IdConsumo = ui->tableView_consumo->model()->data(
        ui->tableView_consumo->model()->index(row,0,QModelIndex())).toInt();

    QString glass = ui->tableView_consumo->model()->data(
                ui->tableView_consumo->model()->index(row,3,QModelIndex())).toString();

    QString fecha = ui->tableView_consumo->model()->data(
                ui->tableView_consumo->model()->index(row,5,QModelIndex())).toString();

    newLamesDiag *frm = new newLamesDiag(this);
    frm->setDb(db->Database("GP"));
    frm->setGlass(glass);
    frm->setIdConsumo(IdConsumo);
    frm->setFecha(fecha);
    frm->setModal(true);
    frm->show();*/
}

void frmPrincipal::on_tableView_consumo_doubleClicked(const QModelIndex &index)
{
    /*
    int row = index.row();
    int id = ui->tableView_consumo->model()->data(
        ui->tableView_consumo->model()->index(row,9,QModelIndex())).toInt();
    IdConsumo = id;

    int IdIndex = ui->cmb_consumo_workoerder->findData(id);

    if ( IdIndex != -1 ) { // -1 for not found
       ui->cmb_consumo_workoerder->setCurrentIndex(IdIndex);
    }

    viewRetailsLames *frm = new viewRetailsLames(this,id);
    frm->setModal(true);
    frm->show();*/
}

void frmPrincipal::on_cmb_consumo_workoerder_currentIndexChanged(int index)
{
    ui->cmb_consumo_client->setCurrentIndex(index);
}

void frmPrincipal::on_actionConsumo_triggered()
{
    reportConsumo *frm = new reportConsumo(this);
    frm->setModal(true);
    frm->show();

}

void frmPrincipal::on_tableView_consumo_activated(const QModelIndex &index)
{

}

void frmPrincipal::on_cmb_consumo_workoerder_activated(const QString &arg1)
{


}

void frmPrincipal::on_pushButton_2_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    QModelIndex index = ui->table_Order_IN->selectionModel()->selectedIndexes().first();

    int row = index.row();
    int idOrder = ui->table_Order_IN->model()->data(
        ui->table_Order_IN->model()->index(row,9,QModelIndex())).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Finish Order", "Are you sure to finish this order?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qryText =   "UPDATE orders "
                    "SET status = 'OK Delivery' "
                    "WHERE id = %1;";

        if (!qry.exec(qryText.arg(idOrder)))
        {
            qDebug() << "error update2 order" << qry.lastError().text();
            msg->setText("error update2 order");
            msg->exec();
        }
        else
        {
            InsertStatusChange(idOrder,"to Coupe",LoggedUser,QDate::currentDate().toString("yyyy-MM-dd"));
        }
    }
    Orders->select();
    Orders_IN->select();
    Orders_Liv->select();
}

void frmPrincipal::on_pushButton_3_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    QModelIndex index = ui->table_livre->selectionModel()->selectedIndexes().first();

    int row = index.row();
    int idOrder = ui->table_livre->model()->data(
        ui->table_livre->model()->index(row,9,QModelIndex())).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Finish Order", "Are you sure to finish this order?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qryText =   "UPDATE orders "
                    "SET status = 'Finish' "
                    "WHERE id = %1;";

        if (!qry.exec(qryText.arg(idOrder)))
        {
            qDebug() << "error update3 order" << qry.lastError().text();
            msg->setText("error update3 order");
            msg->exec();
        }
        else
        {
            InsertStatusChange(idOrder,"to Livre",LoggedUser,QDate::currentDate().toString("yyyy-MM-dd"));
        }
    }
    else
    {
        //Do nothing so far
    }
    Orders->select();
    Orders_IN->select();
    Orders_Liv->select();
}

void frmPrincipal::on_actionCheck_GlassPack_triggered()
{
    checkGlassPackReport *frm = new checkGlassPackReport(this);
    frm->setModal(true);
    frm->show();
}


void frmPrincipal::on_pushButton_15_clicked()
{
    QString f = "status = 'Finish' ";
    Orders_Finish->setFilter(f.arg(ui->date_livre->date().toString("yyyy-MM-dd")));
    Orders_Finish->select();
}


void frmPrincipal::on_pushButton_14_clicked()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("retail");
    model->setFilter(QString("(heigh > '%1' AND with > '%2') OR (heigh > '%2' and with > '%1')").arg(ui->spb_H->value()).arg(ui->spb_W->value()));
    model->select();

    qDebug() << model->filter();
    ui->table_retail->setModel(model);
}


void frmPrincipal::on_pushButton_13_clicked()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("retail");
    model->select();

    ui->table_retail->setModel(model);
}


void frmPrincipal::on_table_Order_IN_doubleClicked(const QModelIndex &index)
{
    int id_order = ui->table_Order_IN->model()->index(index.row(),9).data().toInt();
    QString work_order = ui->table_Order_IN->model()->index(index.row(),0).data().toString();

    ui->cmb_orden_finalizar->setCurrentText(work_order);


    viewMozo *frm = new viewMozo(this,id_order,work_order);
    frm->setModal(true);
    frm->show();
}


void frmPrincipal::on_table_livre_doubleClicked(const QModelIndex &index)
{
    int id_order = ui->table_livre->model()->index(index.row(),9).data().toInt();
    QString work_order = ui->table_livre->model()->index(index.row(),0).data().toString();

    ui->cmb_livre->setCurrentText(work_order);


    viewMozo *frm = new viewMozo(this,id_order,work_order);
    frm->setModal(true);
    frm->show();
}


void frmPrincipal::on_pushButton_17_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GP"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText = "SELECT DocumentDateTime"
                    ",BillToID"
                    ",WorkorderNumber,DocumentNumber,QuoteNumber "
              "FROM [GlasPacLX_LTD].[dbo].[Document] INNER JOIN "
                    "[GlasPacLX_LTD].[dbo].[DocumentDetail] "
                    "ON [DocumentGUID] = [DocumentGUID_FK] "
              "WHERE DocumentType_ENUM <> 0 AND BranchGUID_FK = '65F02099-8E4C-4483-91B8-90AEF2838E76' "
                    "AND WorkorderNumber = '%1' OR DocumentNumber = '%1' OR QuoteNumber = '%1' ;";

    if(!qry.exec(qryText.arg(ui->le_checkWork->text())))
    {
        qDebug() << "error load check workorder" << qry.lastError().text();
        msg->setText("error load mozos");
        msg->exec();
    }
    else
    {
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                while ( qry.next() )
                {
                    QTableWidgetItem *item;
                    item = new QTableWidgetItem(qry.value(0).toString());
                    ui->table_Check->setItem(0,0,item);

                    item = new QTableWidgetItem(qry.value(1).toString());
                    ui->table_Check->setItem(0,1,item);

                    item = new QTableWidgetItem(qry.value(2).toString());
                    ui->table_Check->setItem(0,2,item);

                    item = new QTableWidgetItem(qry.value(3).toString());
                    ui->table_Check->setItem(0,3,item);

                    item = new QTableWidgetItem(qry.value(4).toString());
                    ui->table_Check->setItem(0,4,item);

                    QSqlQuery qry2 = QSqlQuery(*db->Database("GM"));

                    qryText =   "SELECT status "
                                "FROM orders "
                                "WHERE workorder = '%1' ";

                    if(!qry2.exec(qryText.arg(qry.value(2).toString())))
                    {
                        qDebug() << "error load status from workorder" << qry.lastError().text();
                        qDebug() << qry.executedQuery();
                        msg->setText("error load status from workorder");
                        msg->exec();
                    }
                    else
                    {
                        if (qry2.isActive())
                        {
                            if (qry2.isSelect())
                            {
                                while ( qry2.next() )
                                {
                                    item = new QTableWidgetItem(qry2.value(0).toString());
                                    ui->table_Check->setItem(0,5,item);
                                }
                            }
                        }
                    }

                }
            }
        }
    }
}


void frmPrincipal::on_pushButton_18_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));


    //insertar consumo
    qryText = "INSERT INTO lames_from_retail(glass,cant,lames,fecha) "
              "VALUES('%1',%2,%3,'%4');";

    if(!qry.exec(qryText.arg(ui->cmb_lames_vitre->currentText()).
                 arg(ui->spb_Lames_qty->value()).
                 arg(ui->spb_Lames_L->value()).
                 arg(QDate::currentDate().toString("yyyy-MM-dd")))){
        qDebug() << "error insert lames_prod" << qry.lastError().text();
        msg->setText("error insert lames_prod"+qry.lastError().text());
        msg->exec();
    }
    else
    {
        msg->setText("Added");
        msg->exec();

        LamesProd->select();

        ui->spb_Lames_L->stepUp();
    }
}


void frmPrincipal::on_pushButton_19_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;

    QTableWidgetItem *item;

    double LamesBronze316SqInches = 0;
    double LamesBronze14SqInches = 0;
    double LamesClear14SqInches = 0;
    double LamesBronzeOpac316SqInches = 0;
    double LamesClearOpac316SqInches = 0;
    double LamesDarkGrey316SqInches = 0;
    double LamesBronzeReflective316SqInches = 0;

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText =   "SELECT glass "
                        ",cant "
                        ",lames "
                        ",fecha "
                        ",partida "
                "FROM lames "
                "WHERE partida = '%1' ";

    if(!qry.exec(qryText.arg(ui->cmb_lames_filter->currentText())))
    {
        qDebug() << "error load lames" << qry.lastError().text();
        qDebug() << qry.executedQuery();
        msg->setText("error load lames");
        msg->exec();
    }
    else
    {
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                while ( qry.next() )
                {
                    if (qry.value(0).toString().remove('"') == "Bronze 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronze316SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze 1/4")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronze14SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Clear 1/4")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesClear14SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze Opac 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronzeOpac316SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Clear OP 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesClearOpac316SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze Reflective 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronzeReflective316SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Dark Grey 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesDarkGrey316SqInches += (lame*4)*cant;
                    }
                }
            }
        }
    }

    //bronze 3/16
    item = new QTableWidgetItem(QString::number((LamesBronze316SqInches/144)/75.83,'f',2));
    ui->table_lames->setItem(0,1,item);
    item = new QTableWidgetItem(QString::number((LamesBronze316SqInches/144),'f',2));
    ui->table_lames->setItem(0,2,item);
    //bronze 1/4
    item = new QTableWidgetItem(QString::number((LamesBronze14SqInches/144)/75.83,'f',2));
    ui->table_lames->setItem(1,1,item);
    item = new QTableWidgetItem(QString::number((LamesBronze14SqInches/144),'f',2));
    ui->table_lames->setItem(1,2,item);
    //bronze ref 3/16
    item = new QTableWidgetItem(QString::number((LamesBronzeReflective316SqInches/144)/75.83,'f',2));
    ui->table_lames->setItem(2,1,item);
    item = new QTableWidgetItem(QString::number((LamesBronzeReflective316SqInches/144),'f',2));
    ui->table_lames->setItem(2,2,item);
    //bronze op 3/16
    item = new QTableWidgetItem(QString::number((LamesBronzeOpac316SqInches/144)/47.5,'f',2));
    ui->table_lames->setItem(3,1,item);
    item = new QTableWidgetItem(QString::number((LamesBronzeOpac316SqInches/144),'f',2));
    ui->table_lames->setItem(3,2,item);
    //Clear 1/4
    item = new QTableWidgetItem(QString::number((LamesClear14SqInches/144)/75.83,'f',2));
    ui->table_lames->setItem(4,1,item);
    item = new QTableWidgetItem(QString::number((LamesClear14SqInches/144),'f',2));
    ui->table_lames->setItem(4,2,item);
    //Clear opa
    item = new QTableWidgetItem(QString::number((LamesClearOpac316SqInches/144)/47.5,'f',2));
    ui->table_lames->setItem(5,1,item);
    item = new QTableWidgetItem(QString::number((LamesClearOpac316SqInches/144),'f',2));
    ui->table_lames->setItem(5,2,item);
    //dark gray 3/16
    item = new QTableWidgetItem(QString::number((LamesDarkGrey316SqInches/144)/75.83,'f',2));
    ui->table_lames->setItem(6,1,item);
    item = new QTableWidgetItem(QString::number((LamesDarkGrey316SqInches/144),'f',2));
    ui->table_lames->setItem(6,2,item);
}


void frmPrincipal::on_pushButton_20_clicked()
{
    Orders_Search->setFilter(QString("date = '%1' ").arg(ui->date_search->date().toString("yyyy-MM-dd")));
    Orders_Search->select();
    ui->tableView_search->setModel(Orders_Search);
}


void frmPrincipal::on_pushButton_16_clicked()
{

}


void frmPrincipal::on_pushButton_12_clicked()
{

}


void frmPrincipal::on_pushButton_22_clicked()
{
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_find2(QPrinter*)));
    dialog.exec();
}


void frmPrincipal::on_tableView_search_doubleClicked(const QModelIndex &index)
{
    int id_order = ui->tableView->model()->index(index.row(),9).data().toInt();
    QString work_order = ui->tableView->model()->index(index.row(),0).data().toString();

    ui->cmb_orden_repartirorden->setCurrentText(work_order);

    viewMozo *frm = new viewMozo(this,id_order);
    frm->setModal(true);
    frm->show();
}


void frmPrincipal::on_pushButton_23_clicked()
{
    ui->pushButton_25->setEnabled(true);
    QSqlQuery qry2 = QSqlQuery(*db->Database("GP"));
    QSqlQueryModel *qryModel = new QSqlQueryModel(this);
    QString qryText;

    //db->GetDatabase().database("GLASS")

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText =   "SELECT   DocumentNumber,WorkorderNumber,QuoteNumber,Sum(OrderQty) as ItemQty, "
                "         PricingDate,DateTimeLastUpdated,DocumentDateTime "
                "FROM [GlasPacLX_LTD].[dbo].[Document]"
                "INNER JOIN [GlasPacLX_LTD].[dbo].[DocumentDetail]"
                "ON [DocumentGUID] = [DocumentGUID_FK]"
                "WHERE DocumentType_ENUM = 2 AND BranchGUID_FK = '65F02099-8E4C-4483-91B8-90AEF2838E76' "
                "and DocumentDateTime > '%1' and DocumentDateTime < '%2' "
                "GROUP BY DocumentNumber,WorkorderNumber,QuoteNumber,PricingDate,DateTimeLastUpdated,DocumentDateTime";

    if (!db->Database("GP")->isOpen())
    {
        msg->setText("database is not open wtf!!");
        msg->exec();
    }
    else
    {
        qryModel->setQuery(qryText.arg(ui->date_invoice_from->date().toString("yyyy-MM-dd"))
                                  .arg(ui->date_invoice_from->date().addDays(1).toString("yyyy-MM-dd")),
                           *db->Database("GP"));

        ui->table_invoice->setModel(qryModel);

        //ui->testquery->setText(qryModel->query().lastQuery());

    }

    //workorder,invoice,quote,client_name,date,status,cuting_table
    qryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Invoice"));
    qryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("WorkOrder"));
    qryModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Quote"));
    qryModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Item Quantity"));
    qryModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Quote Date"));
    qryModel->setHeaderData(5, Qt::Horizontal, QObject::tr("WorkOrder Date"));
    qryModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Invoice Date"));
}


void frmPrincipal::on_pushButton_24_clicked()
{
    QSqlQuery qry2 = QSqlQuery(*db->Database("GP"));
    QSqlQueryModel *qryModel = new QSqlQueryModel(this);
    QString qryText;

    //db->GetDatabase().database("GLASS")

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText =   "SELECT   DocumentNumber,WorkorderNumber,QuoteNumber,Sum(OrderQty) as ItemQty, "
                "         PricingDate,DateTimeLastUpdated,DocumentDateTime "
                "FROM [GlasPacLX_LTD].[dbo].[Document]"
                "INNER JOIN [GlasPacLX_LTD].[dbo].[DocumentDetail]"
                "ON [DocumentGUID] = [DocumentGUID_FK]"
                "WHERE DocumentType_ENUM = 2 AND BranchGUID_FK = '65F02099-8E4C-4483-91B8-90AEF2838E76' "
                "GROUP BY DocumentNumber,WorkorderNumber,QuoteNumber,PricingDate,DateTimeLastUpdated,DocumentDateTime";

    if (!db->Database("GP")->isOpen())
    {
        msg->setText("database is not open wtf!!");
        msg->exec();
    }
    else
    {
        qryModel->setQuery(qryText,*db->Database("GP"));

        ui->table_invoice->setModel(qryModel);

        msg->setText(qryModel->lastError().text()+qryModel->query().lastError().text());
        msg->exec();
    }
}

void frmPrincipal::print_invoice(QPrinter *printer)
{
    //Definir las propiedades del papel.
    printer->setPageSize(QPageSize::A4);
    //Declarar pincel
    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "Error al abrir la impresión!";
        return;
    }
    //Declarar tablePrinter
    QTableViewPrinter tablePrinter(&painter, printer);
    QVector<int> colWidth;
    for(int i=0;i<ui->table_invoice->model()->columnCount();++i){
        colWidth.push_back(ui->table_invoice->columnWidth(i));
    }

    //Establecer fuente
    QFont headerFont;
    headerFont.setBold(true);
    QFont contentFont;
    contentFont.setPointSize(8);
    tablePrinter.setContentFont(contentFont);

    //Establecer título
    PageTitle* title=new PageTitle(&painter,printer);
    title->setPageTitle("LTD Construction S.A");
    title->setMtitle2("Invoices for: "+ui->date_invoice_to->date().toString());//report Name
    QStringList side = { "Phone: +(509) 2813-4040     Email: info@ltdconstruction-haiti.com",
                         "From: "+ui->date_invoice_from->date().toString(),
                         "    ",
                         };
    /*"From "+ui->date_from->date().toString() +
                         "To "+ui->date_to->date().toString()
    */
    title->setSideTitle(side);
    tablePrinter.setPagerTitle(title);
    tablePrinter.setTitleFlag(TitleFlag::FIRST_PAGE);

    //Establecer pie de página
    PageFooter* pagefooter=new PageFooter(&painter);
    pagefooter->setCreater("Footer");
    tablePrinter.setPagerFooter(pagefooter);

    QStringList headers;
    QStringList headers2;
    for(int i = 0; i < ui->table_invoice->model()->columnCount(); i++)
    {
      headers.append(ui->table_invoice->model()->headerData(i, Qt::Horizontal).toString());
      headers2.append(ui->table_invoice->model()->headerData(i, Qt::Horizontal).toString());
    }

    //QStringList headers = { "título 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    //QStringList headers2 = { "Encabezado 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    if(!tablePrinter.printTable(ui->table_invoice, headers, headers2)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();

}

void frmPrincipal::print_oncoupe(QPrinter *printer)
{
    //Definir las propiedades del papel.
    printer->setPageSize(QPageSize::A4);
    //Declarar pincel
    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "Error al abrir la impresión!";
        return;
    }
    //Declarar tablePrinter
    QTableViewPrinter tablePrinter(&painter, printer);
    QVector<int> colWidth;
    for(int i=0;i<ui->table_Order_IN->model()->columnCount();++i){
        colWidth.push_back(ui->table_Order_IN->columnWidth(i));
    }

    //Establecer fuente
    QFont headerFont;
    headerFont.setBold(true);
    QFont contentFont;
    contentFont.setPointSize(8);
    tablePrinter.setContentFont(contentFont);

    //Establecer título
    PageTitle* title=new PageTitle(&painter,printer);
    title->setPageTitle("LTD Construction S.A");
    title->setMtitle2("Invoices for: "+ui->date_Order_Finish->date().toString());//report Name
    QStringList side = { "Phone: +(509) 2813-4040     Email: info@ltdconstruction-haiti.com",
                         "From: "+ui->date_Order_Finish->date().toString(),
                         "    ",
                         };
    /*"From "+ui->date_from->date().toString() +
                         "To "+ui->date_to->date().toString()
    */
    title->setSideTitle(side);
    tablePrinter.setPagerTitle(title);
    tablePrinter.setTitleFlag(TitleFlag::FIRST_PAGE);

    //Establecer pie de página
    PageFooter* pagefooter=new PageFooter(&painter);
    pagefooter->setCreater("Footer");
    tablePrinter.setPagerFooter(pagefooter);

    QStringList headers;
    QStringList headers2;
    for(int i = 0; i < ui->table_Order_IN->model()->columnCount(); i++)
    {
      headers.append(ui->table_Order_IN->model()->headerData(i, Qt::Horizontal).toString());
      headers2.append(ui->table_Order_IN->model()->headerData(i, Qt::Horizontal).toString());
    }

    //QStringList headers = { "título 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    //QStringList headers2 = { "Encabezado 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    if(!tablePrinter.printTable(ui->table_Order_IN, headers, headers2)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();

}

void frmPrincipal::print_coupe(QPrinter *printer)
{
    //Definir las propiedades del papel.
    printer->setPageSize(QPageSize::A4);
    //Declarar pincel
    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "Error al abrir la impresión!";
        return;
    }
    //Declarar tablePrinter
    QTableViewPrinter tablePrinter(&painter, printer);
    QVector<int> colWidth;
    for(int i=0;i<ui->table_livre->model()->columnCount();++i){
        colWidth.push_back(ui->table_livre->columnWidth(i));
    }

    //Establecer fuente
    QFont headerFont;
    headerFont.setBold(true);
    QFont contentFont;
    contentFont.setPointSize(8);
    tablePrinter.setContentFont(contentFont);

    //Establecer título
    PageTitle* title=new PageTitle(&painter,printer);
    title->setPageTitle("LTD Construction S.A");
    title->setMtitle2("Invoices for: "+ui->date_delivery->date().toString());//report Name
    QStringList side = { "Phone: +(509) 2813-4040     Email: info@ltdconstruction-haiti.com",
                         "From: "+ui->date_delivery->date().toString(),
                         "    ",
                         };
    /*"From "+ui->date_from->date().toString() +
                         "To "+ui->date_to->date().toString()
    */
    title->setSideTitle(side);
    tablePrinter.setPagerTitle(title);
    tablePrinter.setTitleFlag(TitleFlag::FIRST_PAGE);

    //Establecer pie de página
    PageFooter* pagefooter=new PageFooter(&painter);
    pagefooter->setCreater("Footer");
    tablePrinter.setPagerFooter(pagefooter);

    QStringList headers;
    QStringList headers2;
    for(int i = 0; i < ui->table_livre->model()->columnCount(); i++)
    {
      headers.append(ui->table_livre->model()->headerData(i, Qt::Horizontal).toString());
      headers2.append(ui->table_livre->model()->headerData(i, Qt::Horizontal).toString());
    }

    //QStringList headers = { "título 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    //QStringList headers2 = { "Encabezado 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    if(!tablePrinter.printTable(ui->table_livre, headers, headers2)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();

}

void frmPrincipal::print_livre(QPrinter *printer)
{
    //Definir las propiedades del papel.
    printer->setPageSize(QPageSize::A4);
    //Declarar pincel
    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "Error al abrir la impresión!";
        return;
    }
    //Declarar tablePrinter
    QTableViewPrinter tablePrinter(&painter, printer);
    QVector<int> colWidth;
    for(int i=0;i<ui->table_finalize->model()->columnCount();++i){
        colWidth.push_back(ui->table_finalize->columnWidth(i));
    }

    //Establecer fuente
    QFont headerFont;
    headerFont.setBold(true);
    QFont contentFont;
    contentFont.setPointSize(8);
    tablePrinter.setContentFont(contentFont);

    //Establecer título
    PageTitle* title=new PageTitle(&painter,printer);
    title->setPageTitle("LTD Construction S.A");
    title->setMtitle2("Invoices for: "+ui->date_livre->date().toString());//report Name
    QStringList side = { "Phone: +(509) 2813-4040     Email: info@ltdconstruction-haiti.com",
                         "From: "+ui->date_livre->date().toString(),
                         "    ",
                         };
    /*"From "+ui->date_from->date().toString() +
                         "To "+ui->date_to->date().toString()
    */
    title->setSideTitle(side);
    tablePrinter.setPagerTitle(title);
    tablePrinter.setTitleFlag(TitleFlag::FIRST_PAGE);

    //Establecer pie de página
    PageFooter* pagefooter=new PageFooter(&painter);
    pagefooter->setCreater("Footer");
    tablePrinter.setPagerFooter(pagefooter);

    QStringList headers;
    QStringList headers2;
    for(int i = 0; i < ui->table_finalize->model()->columnCount(); i++)
    {
      headers.append(ui->table_finalize->model()->headerData(i, Qt::Horizontal).toString());
      headers2.append(ui->table_finalize->model()->headerData(i, Qt::Horizontal).toString());
    }

    //QStringList headers = { "título 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    //QStringList headers2 = { "Encabezado 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    if(!tablePrinter.printTable(ui->table_finalize, headers, headers2)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();
}

void frmPrincipal::print_RepGlassPackTotal(QPrinter *printer)
{
    //Definir las propiedades del papel.
    printer->setPageSize(QPageSize::A4);
    //Declarar pincel
    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "Error al abrir la impresión!";
            return;
    }
    //Declarar tablePrinter
    QTableViewPrinter tablePrinter(&painter, printer);
    QVector<int> colWidth;
    for(int i=0;i<ui->tableConsumoGPTota->model()->columnCount();++i){
        colWidth.push_back(ui->tableConsumoGPTota->columnWidth(i));
    }

    //Establecer fuente
    QFont headerFont;
    headerFont.setBold(true);
    QFont contentFont;
    contentFont.setPointSize(8);
    tablePrinter.setContentFont(contentFont);

    //Establecer título
    PageTitle* title=new PageTitle(&painter,printer);
    title->setPageTitle("LTD Construction S.A");
    title->setMtitle2("Glass Pack Usage: ");//report Name

    QStringList side = {"Phone: +(509) 2813-4040 Email: info@ltdconstruction-haiti.com",
                        "From: "+ui->dateRepConsumoGPTotal_from->date().toString()};
    /*"From "+ui->date_from->date().toString() +
                         "To "+ui->date_to->date().toString()
    */
    title->setSideTitle(side);
    tablePrinter.setPagerTitle(title);
    tablePrinter.setTitleFlag(TitleFlag::FIRST_PAGE);

    //Establecer pie de página
    PageFooter* pagefooter=new PageFooter(&painter);
    pagefooter->setCreater("Footer");
    tablePrinter.setPagerFooter(pagefooter);

    QStringList headers;
    QStringList headers2;
    for(int i = 0; i < ui->tableConsumoGPTota->model()->columnCount(); i++)
    {
        headers.append(ui->tableConsumoGPTota->model()->headerData(i, Qt::Horizontal).toString());
        headers2.append(ui->tableConsumoGPTota->model()->headerData(i, Qt::Horizontal).toString());
    }

    //QStringList headers = { "título 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    //QStringList headers2 = { "Encabezado 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    if(!tablePrinter.printTable(ui->tableConsumoGPTota, headers, headers2)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();

}

void frmPrincipal::print_RepUsageWO(QPrinter *printer)
{

}

void frmPrincipal::print_RepUsageTable(QPrinter *printer)
{
    //Definir las propiedades del papel.
    printer->setPageSize(QPageSize::A4);
    //Declarar pincel
    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "Error al abrir la impresión!";
            return;
    }
    //Declarar tablePrinter
    QTableViewPrinter tablePrinter(&painter, printer);
    QVector<int> colWidth;
    for(int i=0;i<ui->table_UsoMesa->model()->columnCount();++i){
        colWidth.push_back(ui->table_UsoMesa->columnWidth(i));
    }

    //Establecer fuente
    QFont headerFont;
    headerFont.setBold(true);
    QFont contentFont;
    contentFont.setPointSize(8);
    tablePrinter.setContentFont(contentFont);

    //Establecer título
    PageTitle* title=new PageTitle(&painter,printer);
    title->setPageTitle("LTD Construction S.A");
    title->setMtitle2("Usage by Table: ");//report Name
    QStringList side = {"Phone: +(509) 2813-4040     Email: info@ltdconstruction-haiti.com",
                        "From: "+ui->dateRepUsoMesaFrom->date().toString()
    };
    /*"From "+ui->date_from->date().toString() +
                         "To "+ui->date_to->date().toString()
    */
    title->setSideTitle(side);
    tablePrinter.setPagerTitle(title);
    tablePrinter.setTitleFlag(TitleFlag::FIRST_PAGE);

    //Establecer pie de página
    PageFooter* pagefooter=new PageFooter(&painter);
    pagefooter->setCreater("Footer");
    tablePrinter.setPagerFooter(pagefooter);

    QStringList headers;
    QStringList headers2;
    for(int i = 0; i < ui->table_UsoMesa->model()->columnCount(); i++)
    {
        headers.append(ui->table_UsoMesa->model()->headerData(i, Qt::Horizontal).toString());
        headers2.append(ui->table_UsoMesa->model()->headerData(i, Qt::Horizontal).toString());
    }

    //QStringList headers = { "título 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    //QStringList headers2 = { "Encabezado 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    if(!tablePrinter.printTable(ui->table_UsoMesa, headers, headers2)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();
}

void frmPrincipal::print_RepUsageDPT(QPrinter *printer)
{
    //Definir las propiedades del papel.
    printer->setPageSize(QPageSize::A4);
    //Declarar pincel
    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "Error al abrir la impresión!";
            return;
    }
    //Declarar tablePrinter
    QTableViewPrinter tablePrinter(&painter, printer);
    QVector<int> colWidth;
    for(int i=0;i<ui->tableRepUsoDPT->model()->columnCount();++i){
        colWidth.push_back(ui->tableRepUsoDPT->columnWidth(i));
    }

    //Establecer fuente
    QFont headerFont;
    headerFont.setBold(true);
    QFont contentFont;
    contentFont.setPointSize(8);
    tablePrinter.setContentFont(contentFont);

    //Establecer título
    PageTitle* title=new PageTitle(&painter,printer);
    title->setPageTitle("LTD Construction S.A");
    title->setMtitle2("DPT Usage: ");//report Name
    QStringList side = {"Phone: +(509) 2813-4040     Email: info@ltdconstruction-haiti.com",
                        "From: "+ui->dateRepUsoDptFrom->date().toString()
    };
    /*"From "+ui->date_from->date().toString() +
                         "To "+ui->date_to->date().toString()
    */
    title->setSideTitle(side);
    tablePrinter.setPagerTitle(title);
    tablePrinter.setTitleFlag(TitleFlag::FIRST_PAGE);

    //Establecer pie de página
    PageFooter* pagefooter=new PageFooter(&painter);
    pagefooter->setCreater("Footer");
    tablePrinter.setPagerFooter(pagefooter);

    QStringList headers;
    QStringList headers2;
    for(int i = 0; i < ui->tableRepUsoDPT->model()->columnCount(); i++)
    {
        headers.append(ui->tableRepUsoDPT->model()->headerData(i, Qt::Horizontal).toString());
        headers2.append(ui->tableRepUsoDPT->model()->headerData(i, Qt::Horizontal).toString());
    }

    //QStringList headers = { "título 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    //QStringList headers2 = { "Encabezado 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    if(!tablePrinter.printTable(ui->tableRepUsoDPT, headers, headers2)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();
}

void frmPrincipal::print_RepGlassPvsDPT(QPrinter *printer)
{
    //Definir las propiedades del papel.
    printer->setPageSize(QPageSize::A4);
    //Declarar pincel
    QPainter painter;
    if(!painter.begin(printer)) {
        qWarning() << "Error al abrir la impresión!";
            return;
    }
    //Declarar tablePrinter
    QTableViewPrinter tablePrinter(&painter, printer);
    QVector<int> colWidth;
    for(int i=0;i<ui->tableRepDPTvsGP->model()->columnCount();++i){
        colWidth.push_back(ui->tableRepDPTvsGP->columnWidth(i));
    }

    //Establecer fuente
    QFont headerFont;
    headerFont.setBold(true);
    QFont contentFont;
    contentFont.setPointSize(8);
    tablePrinter.setContentFont(contentFont);

    //Establecer título
    PageTitle* title=new PageTitle(&painter,printer);
    title->setPageTitle("LTD Construction S.A");
    title->setMtitle2("DPT vs GlassPack Comparison: ");//report Name
    QStringList side = {"Phone: +(509) 2813-4040     Email: info@ltdconstruction-haiti.com",
                        "From: "+ui->dateRepDPTvsGPFrom->date().toString()
    };
    /*"From "+ui->date_from->date().toString() +
                         "To "+ui->date_to->date().toString()
    */
    title->setSideTitle(side);
    tablePrinter.setPagerTitle(title);
    tablePrinter.setTitleFlag(TitleFlag::FIRST_PAGE);

    //Establecer pie de página
    PageFooter* pagefooter=new PageFooter(&painter);
    pagefooter->setCreater("Footer");
    tablePrinter.setPagerFooter(pagefooter);

    QStringList headers;
    QStringList headers2;
    for(int i = 0; i < ui->tableRepDPTvsGP->model()->columnCount(); i++)
    {
        headers.append(ui->tableRepDPTvsGP->model()->headerData(i, Qt::Horizontal).toString());
        headers2.append(ui->tableRepDPTvsGP->model()->headerData(i, Qt::Horizontal).toString());
    }

    //QStringList headers = { "título 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    //QStringList headers2 = { "Encabezado 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    if(!tablePrinter.printTable(ui->tableRepDPTvsGP, headers, headers2)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();
}



void frmPrincipal::on_pushButton_25_clicked()
{
    if (ui->table_invoice->model()->rowCount() > 0)
    {
        try
        {
            QPrintPreviewDialog dialog;
            connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_invoice(QPrinter*)));
            dialog.exec();
        }
        catch (...)
        {
            qDebug() << "empty table";
        }

    }
}


void frmPrincipal::on_pushButton_26_clicked()
{
    if (ui->tableView->model()->rowCount() > 0)
    {
        QPrintPreviewDialog dialog;
        connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_oncoupe(QPrinter*)));
        dialog.exec();
    }
}


void frmPrincipal::on_pushButton_27_clicked()
{
    if (ui->table_Order_IN->model()->rowCount() > 0)
    {
        QPrintPreviewDialog dialog;
        connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_oncoupe(QPrinter*)));
        dialog.exec();
    }
}


void frmPrincipal::on_pushButton_28_clicked()
{
    if (ui->table_livre->model()->rowCount() > 0)
    {
        QPrintPreviewDialog dialog;
        connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_coupe(QPrinter*)));
        dialog.exec();
    }
}


void frmPrincipal::on_pushButton_29_clicked()
{
    if (ui->table_finalize->model()->rowCount() > 0)
    {
        QPrintPreviewDialog dialog;
        connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_livre(QPrinter*)));
        dialog.exec();
    }
}


void frmPrincipal::on_pushButton_30_clicked()
{
    QString f = "date = '%1' and status = 'Finish' ";
    Orders_Finish->setFilter(f.arg(ui->date_livre->date().toString("yyyy-MM-dd")));
    Orders_Finish->select();
}


void frmPrincipal::on_pushButton_31_clicked()
{
    Orders_Liv->setFilter("status = 'OK Delivery'");
    Orders_Liv->select();
}


void frmPrincipal::on_pushButton_32_clicked()
{
    QString f = "date = '%1' and status = 'OK Delivery' ";
    Orders_Liv->setFilter(f.arg(ui->date_delivery->date().toString("yyyy-MM-dd")));
    Orders_Liv->select();
}


void frmPrincipal::on_pushButton_33_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    QModelIndex index = ui->tableView->selectionModel()->selectedIndexes().first();

    int row = index.row();
    int idOrder = ui->tableView->model()->data(
        ui->tableView->model()->index(row,9,QModelIndex())).toInt();

    QString table = ui->cmd_Orden_Mesa->currentText();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Finish Order", "Are you sure to finish this order?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qryText =   "UPDATE orders "
                    "SET cuting_table = '%2', status = 'Finish' "
                    "WHERE id = %1;";

        if (!qry.exec(qryText.arg(idOrder).arg(table)))
        {
            qDebug() << "error update3 order" << qry.lastError().text();
            msg->setText("error update3 order");
            msg->exec();
        }
        else
        {
            InsertStatusChange(idOrder,"to En Coupe",LoggedUser,QDate::currentDate().toString("yyyy-MM-dd"));
            InsertStatusChange(idOrder,"to Coupe",LoggedUser,QDate::currentDate().toString("yyyy-MM-dd"));
            InsertStatusChange(idOrder,"to Livre",LoggedUser,QDate::currentDate().toString("yyyy-MM-dd"));
        }
    }
    else
    {
        //Do nothing so far
    }
    Orders->select();
    Orders_IN->select();
    Orders_Liv->select();
}

void frmPrincipal::timerEvent(QTimerEvent *event)
{
    on_actionCargar_Ordenes_triggered();
    on_pushButton_38_clicked();
}


void frmPrincipal::on_pushButton_34_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    QModelIndex index = ui->tableView->selectionModel()->selectedIndexes().first();

    int row = index.row();
    int idOrder = ui->tableView->model()->data(
        ui->tableView->model()->index(row,9,QModelIndex())).toInt();

    QString table = ui->cmd_Orden_Mesa->currentText();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Finish Order", "Are you sure to finish this order?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qryText =   "UPDATE orders "
                    "SET cuting_table = '%2', status = 'OK Delivery' "
                    "WHERE id = %1;";

        if (!qry.exec(qryText.arg(idOrder).arg(table)))
        {
            qDebug() << "error update3 order" << qry.lastError().text();
            msg->setText("error update3 order");
            msg->exec();
        }
        else
        {

            InsertStatusChange(idOrder,"to En Coupe",LoggedUser,QDate::currentDate().toString("yyyy-MM-dd"));
            InsertStatusChange(idOrder,"to Coupe",LoggedUser,QDate::currentDate().toString("yyyy-MM-dd"));

        }
    }
    else
    {
        //Do nothing so far
    }
    Orders->select();
    Orders_IN->select();
    Orders_Liv->select();
}


void frmPrincipal::on_pushButton_35_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    QModelIndex index = ui->table_Order_IN->selectionModel()->selectedIndexes().first();

    int row = index.row();
    int idOrder = ui->table_Order_IN->model()->data(
        ui->table_Order_IN->model()->index(row,9,QModelIndex())).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Finish Order", "Are you sure to finish this order?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qryText =   "UPDATE orders "
                    "SET status = 'Finish' "
                    "WHERE id = %1;";

        if (!qry.exec(qryText.arg(idOrder)))
        {
            qDebug() << "error update2 order" << qry.lastError().text();
            msg->setText("error update2 order");
            msg->exec();
        }
        else
        {
            InsertStatusChange(idOrder,"to Coupe",LoggedUser,QDate::currentDate().toString("yyyy-MM-dd"));
            InsertStatusChange(idOrder,"to Livre",LoggedUser,QDate::currentDate().toString("yyyy-MM-dd"));
        }
    }
    Orders->select();
    Orders_IN->select();
    Orders_Liv->select();
}


void frmPrincipal::on_pushButton_37_clicked()
{
    int idorder = IdByWorkOrder(ui->edit_log_workorder->text());

    QString f = "id_order = %1 ";
    Orders_Log->setFilter(f.arg(idorder));
    Orders_Log->select();
}


void frmPrincipal::on_pushButton_36_clicked()
{
    QString f = "fecha = '%1' ";
    Orders_Log->setFilter(f.arg(ui->date_log->date().toString("yyyy-MM-dd")));
    Orders_Log->select();
}


void frmPrincipal::on_pushButton_38_clicked()
{
    QSqlQuery qry2 = QSqlQuery(*db->Database("GM"));
    QSqlQuery qry3 = QSqlQuery(*db->Database("GM"));
    QSqlQuery qry = QSqlQuery(*db->Database("GP"));
    QString qryText;
    QString qryText2;
    QString qryText3;

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText =   "SELECT DocumentNumber,QuoteNumber "
                "FROM [GlasPacLX_LTD].[dbo].[Document]"
                "INNER JOIN [GlasPacLX_LTD].[dbo].[DocumentDetail]"
                "ON [DocumentGUID] = [DocumentGUID_FK]"
                "WHERE DocumentType_ENUM = 2 AND BranchGUID_FK = '65F02099-8E4C-4483-91B8-90AEF2838E76' AND WorkorderNumber = '%1' ";


    qryText2 = "select workorder from orders "
               "where invoice is NULL or substr(invoice,1,1) = 'W' ";

    if (qry2.exec(qryText2))
    {
        if (qry2.isActive())
        {
            if (qry2.isSelect())
            {
                while ( qry2.next() )
                {
                    qDebug() << "llega";
                    if (!db->Database("GP")->isOpen())
                    {
                        msg->setText("database is not open wtf!!");
                        msg->exec();
                    }
                    else
                    {
                        qDebug() << qry2.value(0).toString();
                        if (qry.exec(qryText.arg(qry2.value(0).toString())))
                        {
                            qryText3 =      "UPDATE orders "
                                            "SET invoice = '%1', quote = '%2' "
                                            "WHERE workorder = '%3';";

                            if (qry.isActive())
                            {
                                if (qry.isSelect())
                                {
                                    while (qry.next())
                                    {
                                        if (!qry3.exec(qryText3.arg(qry.value(0).toString())
                                                               .arg(qry.value(1).toString())
                                                               .arg(qry2.value(0).toString())))
                                        {
                                            qDebug() << "error update order" << qry.lastError().text();
                                            msg->setText("error update order");
                                            msg->exec();
                                        }
                                        else
                                        {
                                            //problems debug etc
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }
    else
    {
        qDebug() << "error update order" << qry2.lastError().text();
        msg->setText("error update order");
        msg->exec();

    }
}


void frmPrincipal::on_table_invoice_doubleClicked(const QModelIndex &index)
{
    QString work_order = ui->table_invoice->model()->index(index.row(),1).data().toString();

    int id_order = 0;
    id_order = IdByWorkOrder(work_order);

    viewMozo *frm = new viewMozo(this,id_order,work_order);
    frm->setModal(true);
    frm->show();
}


void frmPrincipal::on_table_finalize_doubleClicked(const QModelIndex &index)
{
    QString work_order = ui->table_finalize->model()->index(index.row(),0).data().toString();

    int id_order = IdByWorkOrder(work_order);

    //ui->cmb_orden_repartirorden->setCurrentText(work_order);


    viewMozo *frm = new viewMozo(this,id_order,work_order);
    frm->setModal(true);
    frm->show();
}


void frmPrincipal::on_actionReport_Suspand_triggered()
{
    reportSuspand *frm = new reportSuspand(this);
    frm->setModal(true);
    frm->show();
}


void frmPrincipal::on_pushButton_39_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    int IdOrder = IdByWorkOrder(ui->cmb_cons_workorder->text());
    //int IdTable = ui->cmb_cons_table->model().

    if(IdOrder == -1)
    {
        msg->setText("No Workorder "+ui->cmb_cons_workorder->text());
        msg->exec();

        return;
    }

    int row2 = ui->cmb_cons_table->currentIndex();
    QModelIndex idx2 = ui->cmb_cons_table->model()->index(row2, 2); // first column
    QVariant data2 = ui->cmb_cons_table->model()->data(idx2);
    int IdTable = data2.toInt();

    QString qryText = "INSERT INTO [dbo].[consumo] "
                        "([id_order],[id_table],[glass],[sheets],[fecha],[extra]) "
                      "VALUES (%1,%2,'%3',%4,'%5','%6') ";



    if (!qry.exec(qryText.arg(IdOrder)
                         .arg(IdTable)
                         .arg(ui->cmb_cons_glass->currentText())
                         .arg(QString::number(ui->spin_cons_sheetcant->value()))
                         .arg(ui->date_cons->date().toString("yyyy-MM-dd"))
                         .arg(ui->le_cons_extra->text())))
    {
        qDebug() << qryText.arg(IdOrder)
                        .arg(IdTable)
                        .arg(ui->cmb_cons_glass->currentText())
                        .arg(QString::number(ui->spin_cons_sheetcant->value()))
                        .arg(ui->date_cons->date().toString("yyyy-MM-dd"))
                        .arg(ui->le_cons_extra->text());

        qDebug() << "Error insertando consumo"+qry.lastError().text();
    }
    else
    {
        qDebug() << qryText.arg(IdOrder)
                        .arg(IdTable)
                        .arg(ui->cmb_cons_glass->currentText())
                        .arg(QString::number(ui->spin_cons_sheetcant->value()))
                        .arg(ui->date_cons->date().toString("yyyy-MM-dd"))
                        .arg(ui->le_cons_extra->text());

        qDebug() << row2 <<"-"<< idx2.column()<<"x"<<idx2.row()<<data2<<"-"<<IdTable;

        msg->setText("Added!");
        msg->exec();

        consumo_r->select();
    }
}

void frmPrincipal::populateTableWidgetFromQueryModel(QTableWidget* tableWidget, QSqlQueryModel* queryModel)
{
    // Set the number of rows and columns in the table widget
    int rowCount = queryModel->rowCount();
    int columnCount = queryModel->columnCount();
    tableWidget->setRowCount(rowCount);
    tableWidget->setColumnCount(columnCount);

    // Set the column headers
    QStringList headerLabels;
    for (int column = 0; column < columnCount; ++column) {
        headerLabels << queryModel->headerData(column, Qt::Horizontal).toString();
    }
    tableWidget->setHorizontalHeaderLabels(headerLabels);

    // Populate the table widget with data
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            QTableWidgetItem* item = new QTableWidgetItem(queryModel->data(queryModel->index(row, column)).toString());
            tableWidget->setItem(row, column, item);
        }
    }
}

void frmPrincipal::loadMozo()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));

    QString qryText ="SELECT [workorder],[id] FROM [GlassManager].[dbo].[orders] WHERE [id] > 1224;";

    //"SELECT workorder,id FROM orders WHERE id>1000";

    if (!qry.exec(qryText))
    {
        qDebug() <<"somthing bad happend";
    }
    else
    {
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                while (qry.next() )
                {
                    qDebug() << qry.value(0).toString() + "," +QString::number(qry.value(1).toInt());
                    InsertMozo(qry.value(0).toString(),qry.value(1).toInt());
                }
            }
        }
    }
}

void frmPrincipal::on_pushButton_40_clicked()
{
    QStandardItemModel *model = new QStandardItemModel(this);

    model->setRowCount(10);
    model->setColumnCount(3);

    QSqlQuery qry = QSqlQuery(QSqlDatabase::database("GM"));
    QString qryText;

    QTableWidgetItem *item;

    double Bronze316Total = 0;
    double Bronze14Total = 0;
    double Clear14Total = 0;
    double BronzeOpac316Total = 0;
    double ClearOpac316Total = 0;
    double DarkGrey316Total = 0;
    double DarkGrey14Total = 0;
    double BronzeReflective316Total = 0;
    double MirrorSilvered316Total = 0;
    double MirrorSilvered14Total = 0;

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    //CALCULATE AREA IN SQUARE FEET FOR EACH TYPE OF GLASS.

    qryText =   "SELECT glass   "
              ",cant  "
              ",heigh_ "
              ",with_  "
              ",date  "
              ",id_order  "
              "FROM mozo "
              //"WHERE date(date) <= date('%1') AND date(date) >= date('%2') ";
              "WHERE date = '%1'";

    //.arg(ui->date_from->date().toString("yyyy-MM-dd"))
    if(!qry.exec(qryText.arg(ui->dateRepConsumoGPTotal_from->date().toString("yyyy-MM-dd"))
                  ))
    {
        qDebug() << "error load mozos" << qry.lastError().text();
        msg->setText("error load mozos");
        msg->exec();
    }
    else
    {
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                int i = 0;
                QString IdOrder;
                QString IdOrderCheck;
                double sqfOrder = 0;

                while ( qry.next() )
                {
                    qDebug() << "id check: " << qry.value(5).toString();
                    //Bronze 3/16
                    if (qry.value(0).toString().remove('"') == QString("Bronze 3/16"))
                    {
                        IdOrder = qry.value(5).toString();

                        if (i==0)
                        {IdOrderCheck = IdOrder;}

                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        //qDebug() << "High frac: " << qry.value(2).toString();
                        //qDebug() << "With frac: " << qry.value(3).toString();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        //qDebug() << "High: " << high;
                        //qDebug() << "With: " << with;

                        double area = (high*with)*qry.value(1).toDouble();

                        if (IdOrder == IdOrderCheck)
                        {
                            sqfOrder += area/144;
                        }
                        else
                        {
                            qDebug() << "Area Order: " <<IdOrderCheck <<"->"<< sqfOrder;

                            sqfOrder = 0;

                            IdOrderCheck = IdOrder;
                        }

                        //qDebug() << "Area Inch: " << area;
                        //qDebug() << "Area Feet: " << area/144;
                        Bronze316Total += area/144.00;
                        //qDebug() << "Total: " << Bronze316Total;
                        //qDebug() << "-----";
                    }


                    //Bronze 1/4
                    if (qry.value(0).toString().remove('"') == QString("Bronze 1/4"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}


                        double area = (high*with)*qry.value(1).toDouble();
                        Bronze14Total += area/144;
                    }

                    //Clear 1/4
                    if (qry.value(0).toString().remove('"') == QString("Clear 1/4"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toDouble();
                        Clear14Total += area/144;
                    }

                    //Bronze OP 3/16"
                    if (qry.value(0).toString().remove('"') == QString("Bronze OP 3/16"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toDouble();
                        BronzeOpac316Total += area/144;
                    }

                    //Clear OP 3/16
                    if (qry.value(0).toString().remove('"') == QString("Clear OP 3/16"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        ClearOpac316Total += area/144;
                    }

                    //Esp Bronze 3/16
                    if (qry.value(0).toString().remove('"') == QString("Esp Bronze 3/16"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        BronzeReflective316Total += area/144;
                    }

                    //Dark gray 3/16
                    if (qry.value(0).toString().remove('"') == QString("Dark gray 3/16"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        DarkGrey316Total += area/144;
                    }

                    //Dark gray 1/4
                    if (qry.value(0).toString().remove('"') == QString("Dark gray 1/4"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        DarkGrey14Total += area/144;
                    }

                    //Mirror 3/16
                    if (qry.value(0).toString().remove('"') == QString("Mirror 3/16"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        MirrorSilvered316Total += area/144;
                    }

                    //Mirror Silvered 1/4
                    if (qry.value(0).toString().remove('"') == QString("Miroir 1/4"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        MirrorSilvered14Total += area/144;
                    }

                    i++;
                }
            }
            else
            {
                qDebug() << "Query not Select!!!";
            }
        }
        else
        {
            qDebug() << "Query not active!!!";
        }


    }

    QModelIndex index;

    //bronze 3/16
    index = model->index(0, 0, QModelIndex());
    model->setData(index, "Bronze 3/16");
    index = model->index(0, 1, QModelIndex());
    model->setData(index, QString::number(Bronze316Total,'f',2));
    index = model->index(0, 2, QModelIndex());
    model->setData(index, QString::number(Bronze316Total/75.83,'f',2));

    //bronze 1/4
    index = model->index(1, 0, QModelIndex());
    model->setData(index, "Bronze 1/4");
    index = model->index(1, 1, QModelIndex());
    model->setData(index, QString::number(Bronze14Total,'f',2));
    index = model->index(1, 2, QModelIndex());
    model->setData(index, QString::number(Bronze14Total/75.83,'f',2));

    //bronze ref 3/16
    index = model->index(2, 0, QModelIndex());
    model->setData(index, "Bronze ref 3/16");
    index = model->index(2, 1, QModelIndex());
    model->setData(index, QString::number(BronzeReflective316Total,'f',2));
    index = model->index(2, 2, QModelIndex());
    model->setData(index, QString::number(BronzeReflective316Total/75.83,'f',2));

    //bronze op 3/16
    index = model->index(3, 0, QModelIndex());
    model->setData(index, "Bronze op 3/16");
    index = model->index(3, 1, QModelIndex());
    model->setData(index, QString::number(BronzeOpac316Total,'f',2));
    index = model->index(3, 2, QModelIndex());
    model->setData(index, QString::number(BronzeOpac316Total/75.83,'f',2));

    //Clear 1/4
    index = model->index(4, 0, QModelIndex());
    model->setData(index, "Clear 1/4");
    index = model->index(4, 1, QModelIndex());
    model->setData(index, QString::number(Clear14Total,'f',2));
    index = model->index(4, 2, QModelIndex());
    model->setData(index, QString::number(Clear14Total/75.83,'f',2));

    //Clear opa
    index = model->index(5, 0, QModelIndex());
    model->setData(index, "Clear Opac 3/16");
    index = model->index(5, 1, QModelIndex());
    model->setData(index, QString::number(ClearOpac316Total,'f',2));
    index = model->index(5, 2, QModelIndex());
    model->setData(index, QString::number(ClearOpac316Total/75.83,'f',2));

    //dark gray 3/16
    index = model->index(6, 0, QModelIndex());
    model->setData(index, "Dark Gray 3/16");
    index = model->index(6, 1, QModelIndex());
    model->setData(index, QString::number(DarkGrey316Total,'f',2));
    index = model->index(6, 2, QModelIndex());
    model->setData(index, QString::number(DarkGrey316Total/75.83,'f',2));

    //dark gray 1/4
    index = model->index(7, 0, QModelIndex());
    model->setData(index, "Dark Gray 1/4");
    index = model->index(7, 1, QModelIndex());
    model->setData(index, QString::number(DarkGrey14Total,'f',2));
    index = model->index(7, 2, QModelIndex());
    model->setData(index, QString::number(DarkGrey14Total/75.83,'f',2));


    //mirror 3/16
    index = model->index(8, 0, QModelIndex());
    model->setData(index, "Silver Mirror 3/16");
    index = model->index(8, 1, QModelIndex());
    model->setData(index, QString::number(MirrorSilvered316Total,'f',2));
    index = model->index(8, 2, QModelIndex());
    model->setData(index, QString::number(MirrorSilvered316Total/75.83,'f',2));

    //mirror 1/4
    index = model->index(9, 0, QModelIndex());
    model->setData(index, "Silver Mirror 1/4");
    index = model->index(9, 1, QModelIndex());
    model->setData(index, QString::number(MirrorSilvered14Total,'f',2));
    index = model->index(9, 2, QModelIndex());
    model->setData(index, QString::number(MirrorSilvered14Total/75.83,'f',2));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Glass"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Glasspack SqFt"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("GlassPack Sheets"));

    ui->tableConsumoGPTota->setModel(model);
}


void frmPrincipal::on_pushButton_41_clicked()
{
    QSqlQueryModel *qryModel = new QSqlQueryModel(this);
    QString qryText;

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText = "SELECT cuting_table.nombre as TableName, consumo.glass as Glass, SUM(consumo.sheets) as SheetsQty, SUM(consumo.sheets * 75.83) as SqFt "
              "FROM GlassManager.dbo.consumo "
              "JOIN GlassManager.dbo.cuting_table ON consumo.id_table = cuting_table.id "
              "WHERE consumo.fecha >= '%1' AND consumo.fecha <= '%2' "
              "GROUP BY consumo.id_table, cuting_table.nombre,consumo.glass";

    if (!db->Database("GM")->isOpen())
    {
        msg->setText("database is not open wtf!!");
        msg->exec();
    }
    else
    {
        qryModel->setQuery(qryText.arg(ui->dateRepUsoMesaFrom->date().toString("yyyy-MM-dd"))
                                  .arg(ui->dateRepUsoMesaTo->date().toString("yyyy-MM-dd"))
                           ,*db->Database("GP"));
        ui->table_UsoMesa->setModel(qryModel);

        //msg->setText(qryModel->query().executedQuery());
        //msg->exec();
    }
}

void frmPrincipal::on_pushButton_42_clicked()
{

}


void frmPrincipal::on_pushButton_43_clicked()
{

}


void frmPrincipal::on_pushButton_44_clicked()
{

}


void frmPrincipal::on_pushButton_45_clicked()
{
    QSqlQueryModel *qryModel = new QSqlQueryModel(this);
    QString qryText;

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText = "SELECT consumo.glass as Glass, SUM(consumo.sheets) as SheetsQty,  SUM(consumo.sheets * 75.83) as SqFt "
              "FROM GlassManager.dbo.consumo "
              "WHERE consumo.fecha >= '%1' AND consumo.fecha <= '%2' "
              "GROUP BY consumo.glass ";

    if (!db->Database("GM")->isOpen())
    {
        msg->setText("database is not open wtf!!");
        msg->exec();
    }
    else
    {
        qryModel->setQuery(qryText.arg(ui->dateRepUsoDptFrom->date().toString("yyyy-MM-dd"))
                               .arg(ui->dateRepUsoDptTo->date().toString("yyyy-MM-dd"))
                           ,*db->Database("GP"));
        ui->tableRepUsoDPT->setModel(qryModel);

        msg->setText(qryModel->query().executedQuery()+qryModel->query().lastError().text());
        msg->exec();
    }
}


void frmPrincipal::on_pushButton_46_clicked()
{
    QStandardItemModel *model = new QStandardItemModel(this);

    model->setRowCount(10);
    model->setColumnCount(8);

    QSqlQuery qry = QSqlQuery(QSqlDatabase::database("GM"));
    QString qryText;

    double Bronze316Total = 0;
    double Bronze14Total = 0;
    double Clear14Total = 0;
    double BronzeOpac316Total = 0;
    double ClearOpac316Total = 0;
    double DarkGrey316Total = 0;
    double DarkGrey14Total = 0;
    double BronzeReflective316Total = 0;
    double MirrorSilvered316Total = 0;
    double MirrorSilvered14Total = 0;

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    //CALCULATE AREA IN SQUARE FEET FOR EACH TYPE OF GLASS.

    qryText =   "SELECT glass   "
              ",cant  "
              ",heigh_ "
              ",with_  "
              ",date  "
              ",id_order  "
              "FROM mozo "
              "WHERE date <= '%1' AND date >= '%2' ";

    if(!qry.exec(qryText.arg(ui->dateRepDPTvsGPFrom->date().toString("yyyy-MM-dd"))
                        .arg(ui->dateRepDPTvsGPTo->date().toString("yyyy-MM-dd"))
                  ))
    {
        qDebug() << "error load mozos" << qry.lastError().text();
        msg->setText("error load mozos");
        msg->exec();
    }
    else
    {
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                int i = 0;
                QString IdOrder;
                QString IdOrderCheck;
                double sqfOrder = 0;

                while ( qry.next() )
                {
                    qDebug() << "id check: " << qry.value(5).toString();
                    //Bronze 3/16
                    if (qry.value(0).toString().remove('"') == QString("Bronze 3/16"))
                    {
                        IdOrder = qry.value(5).toString();

                        if (i==0)
                        {IdOrderCheck = IdOrder;}

                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        //qDebug() << "High frac: " << qry.value(2).toString();
                        //qDebug() << "With frac: " << qry.value(3).toString();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        //qDebug() << "High: " << high;
                        //qDebug() << "With: " << with;

                        double area = (high*with)*qry.value(1).toDouble();

                        if (IdOrder == IdOrderCheck)
                        {
                            sqfOrder += area/144;
                        }
                        else
                        {
                            qDebug() << "Area Order: " <<IdOrderCheck <<"->"<< sqfOrder;

                            sqfOrder = 0;

                            IdOrderCheck = IdOrder;
                        }

                        //qDebug() << "Area Inch: " << area;
                        //qDebug() << "Area Feet: " << area/144;
                        Bronze316Total += area/144.00;
                        //qDebug() << "Total: " << Bronze316Total;
                        //qDebug() << "-----";
                    }


                    //Bronze 1/4
                    if (qry.value(0).toString().remove('"') == QString("Bronze 1/4"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}


                        double area = (high*with)*qry.value(1).toDouble();
                        Bronze14Total += area/144;
                    }

                    //Clear 1/4
                    if (qry.value(0).toString().remove('"') == QString("Clear 1/4"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toDouble();
                        Clear14Total += area/144;
                    }

                    //Bronze OP 3/16"
                    if (qry.value(0).toString().remove('"') == QString("Bronze OP 3/16"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toDouble();
                        BronzeOpac316Total += area/144;
                    }

                    //Clear OP 3/16
                    if (qry.value(0).toString().remove('"') == QString("Clear OP 3/16"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        ClearOpac316Total += area/144;
                    }

                    //Esp Bronze 3/16
                    if (qry.value(0).toString().remove('"') == QString("Esp Bronze 3/16"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        BronzeReflective316Total += area/144;
                    }

                    //Dark gray 3/16
                    if (qry.value(0).toString().remove('"') == QString("Dark gray 3/16"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        DarkGrey316Total += area/144;
                    }

                    //Dark gray 1/4
                    if (qry.value(0).toString().remove('"') == QString("Dark gray 1/4"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        DarkGrey14Total += area/144;
                    }

                    //Mirror 3/16
                    if (qry.value(0).toString().remove('"') == QString("Mirror 3/16"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        MirrorSilvered316Total += area/144;
                    }

                    //Mirror Silvered 1/4
                    if (qry.value(0).toString().remove('"') == QString("Miroir 1/4"))
                    {
                        double high = FractToDec(qry.value(2).toString()).toDouble();
                        double with = FractToDec(qry.value(3).toString()).toDouble();

                        high = qRound(high * 0.5)*2;

                        if (high < FractToDec(qry.value(2).toString()).toDouble()) {high+=2;}

                        with = qRound(with * 0.5)*2;

                        if (with < FractToDec(qry.value(3).toString()).toDouble()) {with+=2;}

                        double area = (high*with)*qry.value(1).toInt();
                        MirrorSilvered14Total += area/144;
                    }

                    i++;
                }
            }
            else
            {
                qDebug() << "Query not Select!!!";
            }
        }
        else
        {
            qDebug() << "Query not active!!!";
        }
    }

    QModelIndex index;

    //bronze 3/16
    index = model->index(0, 0, QModelIndex());
    model->setData(index, "Bronze 3/16");
    index = model->index(0, 1, QModelIndex());
    model->setData(index, QString::number(Bronze316Total,'f',2));
    index = model->index(0, 2, QModelIndex());
    model->setData(index, QString::number(Bronze316Total/75.83,'f',2));

    //bronze 1/4
    index = model->index(1, 0, QModelIndex());
    model->setData(index, "Bronze 1/4");
    index = model->index(1, 1, QModelIndex());
    model->setData(index, QString::number(Bronze14Total,'f',2));
    index = model->index(1, 2, QModelIndex());
    model->setData(index, QString::number(Bronze14Total/75.83,'f',2));

    //bronze ref 3/16
    index = model->index(2, 0, QModelIndex());
    model->setData(index, "Esp Bronze 3/16");
    index = model->index(2, 1, QModelIndex());
    model->setData(index, QString::number(BronzeReflective316Total,'f',2));
    index = model->index(2, 2, QModelIndex());
    model->setData(index, QString::number(BronzeReflective316Total/75.83,'f',2));

    //bronze op 3/16
    index = model->index(3, 0, QModelIndex());
    model->setData(index, "Bronze OP 3/16");
    index = model->index(3, 1, QModelIndex());
    model->setData(index, QString::number(BronzeOpac316Total,'f',2));
    index = model->index(3, 2, QModelIndex());
    model->setData(index, QString::number(BronzeOpac316Total/75.83,'f',2));

    //Clear 1/4
    index = model->index(4, 0, QModelIndex());
    model->setData(index, "Clear 1/4");
    index = model->index(4, 1, QModelIndex());
    model->setData(index, QString::number(Clear14Total,'f',2));
    index = model->index(4, 2, QModelIndex());
    model->setData(index, QString::number(Clear14Total/75.83,'f',2));

    //Clear opa
    index = model->index(5, 0, QModelIndex());
    model->setData(index, "Clear OP 3/16");
    index = model->index(5, 1, QModelIndex());
    model->setData(index, QString::number(ClearOpac316Total,'f',2));
    index = model->index(5, 2, QModelIndex());
    model->setData(index, QString::number(ClearOpac316Total/75.83,'f',2));

    //dark gray 3/16
    index = model->index(6, 0, QModelIndex());
    model->setData(index, "Dark Gray 3/16");
    index = model->index(6, 1, QModelIndex());
    model->setData(index, QString::number(DarkGrey316Total,'f',2));
    index = model->index(6, 2, QModelIndex());
    model->setData(index, QString::number(DarkGrey316Total/75.83,'f',2));

    //dark gray 1/4
    index = model->index(7, 0, QModelIndex());
    model->setData(index, "Dark Gray 1/4");
    index = model->index(7, 1, QModelIndex());
    model->setData(index, QString::number(DarkGrey14Total,'f',2));
    index = model->index(7, 2, QModelIndex());
    model->setData(index, QString::number(DarkGrey14Total/75.83,'f',2));


    //mirror 3/16
    index = model->index(8, 0, QModelIndex());
    model->setData(index, "Silver Mirror 3/16");
    index = model->index(8, 1, QModelIndex());
    model->setData(index, QString::number(MirrorSilvered316Total,'f',2));
    index = model->index(8, 2, QModelIndex());
    model->setData(index, QString::number(MirrorSilvered316Total/75.83,'f',2));

    //mirror 1/4
    index = model->index(9, 0, QModelIndex());
    model->setData(index, "Silver Mirror 1/4");
    index = model->index(9, 1, QModelIndex());
    model->setData(index, QString::number(MirrorSilvered14Total,'f',2));
    index = model->index(9, 2, QModelIndex());
    model->setData(index, QString::number(MirrorSilvered14Total/75.83,'f',2));

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Glass"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Glasspack SqFt"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("GlassPack Sheets"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DPT Sheets"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("DPT SqFt"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Sheets Dif"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("SqFt Dif"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Dif Porcentage"));


    QSqlQueryModel *qryModel = new QSqlQueryModel(this);

    qryText = "SELECT consumo.glass as Glass, SUM(consumo.sheets) as SheetsQty,  SUM(consumo.sheets * 75.83) as SqFt "
              "FROM GlassManager.dbo.consumo "
              "WHERE consumo.fecha >= '%1' AND consumo.fecha <= '%2' "
              "GROUP BY consumo.glass ";

    qryModel->setQuery(qryText.arg(ui->dateRepDPTvsGPFrom->date().toString("yyyy-MM-dd"))
                               .arg(ui->dateRepDPTvsGPTo->date().toString("yyyy-MM-dd"))
                           ,*db->Database("GP"));

    //ui->tableRepUsoDPT->setModel(qryModel);
    //msg->setText(qryModel->query().executedQuery()+qryModel->query().lastError().text());
    //msg->exec();

    // Iterate through the QSqlQueryModel and set data in the QStandardItemModel

    for (int row = 0; row < qryModel->rowCount(); ++row)
    {
        double Sheets = 0;
        double SqFt = 0;
        double diffPercentage = 0;
        //QString glass = qryModel->data(qryModel->index(row, 0)).toString();

        if (qryModel->data(qryModel->index(row, 0)).toString() == "Bronze 3/16")
        {
            SqFt = model->data(model->index(0,1)).toDouble();
            Sheets = model->data(model->index(0,2)).toDouble();

            index = model->index(0, 3, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 1)).toString());
            index = model->index(0, 4, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 2)).toString());
            index = model->index(0, 5, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 1)).toDouble()-Sheets,'f',2));
            index = model->index(0, 6, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 2)).toDouble()-SqFt,'f',2));
            index = model->index(0, 7, QModelIndex());
            double total = qryModel->data(qryModel->index(row, 2)).toDouble();
            double part = qryModel->data(qryModel->index(row, 2)).toDouble() - SqFt;
            diffPercentage = (part/total)*100;
            model->setData(index,QString::number(diffPercentage,'f',2)+" %");
        }
        if (qryModel->data(qryModel->index(row, 0)).toString() == "Bronze 1/4")
        {
            SqFt = model->data(model->index(1,1)).toDouble();
            Sheets = model->data(model->index(1,2)).toDouble();

            index = model->index(1, 3, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 1)).toString());
            index = model->index(1, 4, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 2)).toString());
            index = model->index(1, 5, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 1)).toDouble()-Sheets,'f',2));
            index = model->index(1, 6, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 2)).toDouble()-SqFt,'f',2));
            index = model->index(1, 7, QModelIndex());
            double total = qryModel->data(qryModel->index(row, 2)).toDouble();
            double part = qryModel->data(qryModel->index(row, 2)).toDouble() - SqFt;
            diffPercentage = (part/total)*100;
            model->setData(index,QString::number(diffPercentage,'f',2)+" %");
        }
        if (qryModel->data(qryModel->index(row, 0)).toString() == "Bronze Reflective 3/16")
        {
            SqFt = model->data(model->index(2,1)).toDouble();
            Sheets = model->data(model->index(2,2)).toDouble();

            index = model->index(2, 3, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 1)).toString());
            index = model->index(2, 4, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 2)).toString());
            index = model->index(2, 5, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 1)).toDouble()-Sheets,'f',2));
            index = model->index(2, 6, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 2)).toDouble()-SqFt,'f',2));
            index = model->index(2, 7, QModelIndex());
            double total = qryModel->data(qryModel->index(row, 2)).toDouble();
            double part = qryModel->data(qryModel->index(row, 2)).toDouble() - SqFt;
            diffPercentage = (part/total)*100;
            model->setData(index,QString::number(diffPercentage,'f',2)+" %");
        }
        if (qryModel->data(qryModel->index(row, 0)).toString() == "Bronze Opac 3/16")
        {
            SqFt = model->data(model->index(3,1)).toDouble();
            Sheets = model->data(model->index(3,2)).toDouble();

            index = model->index(3, 3, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 1)).toString());
            index = model->index(3, 4, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 2)).toString());
            index = model->index(3, 5, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 1)).toDouble()-Sheets,'f',2));
            index = model->index(3, 6, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 2)).toDouble()-SqFt,'f',2));
            index = model->index(3, 7, QModelIndex());
            double total = qryModel->data(qryModel->index(row, 2)).toDouble();
            double part = qryModel->data(qryModel->index(row, 2)).toDouble() - SqFt;
            diffPercentage = (part/total)*100;
            model->setData(index,QString::number(diffPercentage,'f',2)+" %");
        }
        if (qryModel->data(qryModel->index(row, 0)).toString() == "Clear 1/4")
        {
            SqFt = model->data(model->index(4,1)).toDouble();
            Sheets = model->data(model->index(4,2)).toDouble();

            index = model->index(4, 3, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 1)).toString());
            index = model->index(4, 4, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 2)).toString());
            index = model->index(4, 5, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 1)).toDouble()-Sheets,'f',2));
            index = model->index(4, 6, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 2)).toDouble()-SqFt,'f',2));
            index = model->index(4, 7, QModelIndex());
            double total = qryModel->data(qryModel->index(row, 2)).toDouble();
            double part = qryModel->data(qryModel->index(row, 2)).toDouble() - SqFt;
            diffPercentage = (part/total)*100;
            model->setData(index,QString::number(diffPercentage,'f',2)+" %");
        }
        if (qryModel->data(qryModel->index(row, 0)).toString() == "Clear Opac 3/16")
        {
            SqFt = model->data(model->index(5,1)).toDouble();
            Sheets = model->data(model->index(5,2)).toDouble();

            index = model->index(5, 3, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 1)).toString());
            index = model->index(5, 4, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 2)).toString());
            index = model->index(5, 5, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 1)).toDouble()-Sheets,'f',2));
            index = model->index(5, 6, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 2)).toDouble()-SqFt,'f',2));
            index = model->index(5, 7, QModelIndex());
            double total = qryModel->data(qryModel->index(row, 2)).toDouble();
            double part = qryModel->data(qryModel->index(row, 2)).toDouble() - SqFt;
            diffPercentage = (part/total)*100;
            model->setData(index,QString::number(diffPercentage,'f',2)+" %");
        }
        if (qryModel->data(qryModel->index(row, 0)).toString() == "Dark Grey 3/16")
        {
            SqFt = model->data(model->index(6,1)).toDouble();
            Sheets = model->data(model->index(6,2)).toDouble();

            index = model->index(6, 3, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 1)).toString());
            index = model->index(6, 4, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 2)).toString());
            index = model->index(6, 5, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 1)).toDouble()-Sheets,'f',2));
            index = model->index(6, 6, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 2)).toDouble()-SqFt,'f',2));
            index = model->index(6, 7, QModelIndex());
            double total = qryModel->data(qryModel->index(row, 2)).toDouble();
            double part = qryModel->data(qryModel->index(row, 2)).toDouble() - SqFt;
            diffPercentage = (part/total)*100;
            model->setData(index,QString::number(diffPercentage,'f',2)+" %");
        }
        if (qryModel->data(qryModel->index(row, 0)).toString() == "Dark Gray 1/4")
        {
            SqFt = model->data(model->index(7,1)).toDouble();
            Sheets = model->data(model->index(7,2)).toDouble();

            index = model->index(7, 3, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 1)).toString());
            index = model->index(7, 4, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 2)).toString());
            index = model->index(7, 5, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 1)).toDouble()-Sheets,'f',2));
            index = model->index(7, 6, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 2)).toDouble()-SqFt,'f',2));
            index = model->index(7, 7, QModelIndex());
            double total = qryModel->data(qryModel->index(row, 2)).toDouble();
            double part = qryModel->data(qryModel->index(row, 2)).toDouble() - SqFt;
            diffPercentage = (part/total)*100;
            model->setData(index,QString::number(diffPercentage,'f',2)+" %");
        }
        if (qryModel->data(qryModel->index(row, 0)).toString() == "Mirror 3/16")
        {
            SqFt = model->data(model->index(7,1)).toDouble();
            Sheets = model->data(model->index(7,2)).toDouble();

            index = model->index(8, 3, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 1)).toString());
            index = model->index(8, 4, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 2)).toString());
            index = model->index(8, 5, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 1)).toDouble()-Sheets,'f',2));
            index = model->index(8, 6, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 2)).toDouble()-SqFt,'f',2));
            index = model->index(8, 7, QModelIndex());
            double total = qryModel->data(qryModel->index(row, 2)).toDouble();
            double part = qryModel->data(qryModel->index(row, 2)).toDouble() - SqFt;
            diffPercentage = (part/total)*100;
            model->setData(index,QString::number(diffPercentage,'f',2)+" %");
        }
        if (qryModel->data(qryModel->index(row, 0)).toString() == "Miroir 1/4")
        {
            SqFt = model->data(model->index(9,1)).toDouble();
            Sheets = model->data(model->index(9,2)).toDouble();

            index = model->index(9, 3, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 1)).toString());
            index = model->index(9, 4, QModelIndex());
            model->setData(index, qryModel->data(qryModel->index(row, 2)).toString());
            index = model->index(9, 5, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 1)).toDouble()-Sheets,'f',2));
            index = model->index(9, 6, QModelIndex());
            model->setData(index,QString::number(qryModel->data(qryModel->index(row, 2)).toDouble()-SqFt,'f',2));
            index = model->index(9, 7, QModelIndex());
            double total = qryModel->data(qryModel->index(row, 2)).toDouble();
            double part = qryModel->data(qryModel->index(row, 2)).toDouble() - SqFt;
            diffPercentage = (part/total)*100;
            model->setData(index,QString::number(diffPercentage,'f',2)+" %");
        }
    }

    ui->tableRepDPTvsGP->setModel(model);    
}


void frmPrincipal::on_pushButton_47_clicked()
{
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_RepGlassPackTotal(QPrinter*)));
    dialog.exec();
}


void frmPrincipal::on_pushButton_21_clicked()
{

}


void frmPrincipal::on_pushButton_48_clicked()
{
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_RepUsageTable(QPrinter*)));
    dialog.exec();
}


void frmPrincipal::on_pushButton_49_clicked()
{
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_RepUsageDPT(QPrinter*)));
    dialog.exec();
}


void frmPrincipal::on_pushButton_50_clicked()
{
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_RepGlassPvsDPT(QPrinter*)));
    dialog.exec();
}


void frmPrincipal::on_cmb_RepUsoMesaTable_currentIndexChanged(int index)
{

}


void frmPrincipal::on_cmb_RepUsoMesaTable_currentTextChanged(const QString &arg1)
{

}


void frmPrincipal::on_cmb_cons_workorder_activated(int index)
{

}


void frmPrincipal::on_cmb_cons_workorder_currentIndexChanged(int index)
{

}


void frmPrincipal::on_cmb_cons_workorder_currentTextChanged(const QString &arg1)
{

}


void frmPrincipal::on_cmb_cons_workorder_editTextChanged(const QString &arg1)
{

}


void frmPrincipal::on_pushButton_5_clicked()
{

}


void frmPrincipal::on_pushButton_4_clicked()
{

}


void frmPrincipal::on_date_cons_dateChanged(const QDate &date)
{
    QString filter = "fecha = %1 ";
    consumo_r->setFilter(filter.arg(date.toString("yyyy-MM-dd")));
    consumo_r->select();
}


void frmPrincipal::on_pushButton_55_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));


    //insertar consumo
    qryText = "INSERT INTO lames_transformation(glass,sheets,cant,lames,fecha) "
              "VALUES('%1',%2,%3,%4,'%5');";

    if(!qry.exec(qryText.arg(ui->cmb_lames_vitre->currentText()).
                  arg(ui->spb_Lames_sheets_2->value()).
                  arg(ui->spb_Lames_qty_2->value()).
                  arg(ui->spb_Lames_L_2->value()).
                  arg(QDate::currentDate().toString("yyyy-MM-dd")))){
        qDebug() << "error insert lames_trans" << qry.lastError().text();
        msg->setText("error insert lames_trans"+qry.lastError().text());
        msg->exec();
    }
    else
    {
        msg->setText("Added");
        msg->exec();

        LamesTrans->select();

        ui->spb_Lames_L->stepUp();
    }
}


void frmPrincipal::on_pushButton_58_clicked()
{
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    const QString  &glass = ui->cmb_lames_vitre_3->currentText();
    int cant = ui->spb_Lames_qty_3->value();
    int lamesCount = ui->spb_Lames_L_3->value();
    QDate fecha = QDate::currentDate();
    QString  partida = "";

    // ---------- 1. Start a transaction ----------
    if (!db->Database("GM")->transaction()) {
        qWarning() << "Cannot start transaction: " << db->Database("GM")->lastError().text();
        msg->setText("Cannot start transaction: "+db->Database("GM")->lastError().text());
        msg->exec();
        return;
    }

    QSqlQuery qry(*db->Database("GM"));

    // ---------- 2. Try UPDATE first ----------
    qry.prepare(R"(
        UPDATE lames
        SET    cant    = ?,     -- replace; or  cant = cant + ?  to accumulate
               fecha   = ?,     -- DATETIME2 or DATETIME column
               partida = ?
        WHERE  glass   = ?
          AND  lames   = ?
    )");

    qry.addBindValue(cant);
    qry.addBindValue(fecha.toString("yyyy-MM-dd"));  // "2025-07-09T14:23:00"
    qry.addBindValue(partida);
    qry.addBindValue(glass);
    qry.addBindValue(lamesCount);

    if (!qry.exec()) {
        qWarning() << "UPDATE failed:" << qry.lastError().text();
        msg->setText("UPDATE failed: "+qry.lastError().text());
        msg->exec();
        db->Database("GM")->rollback();
        return;
    }

    // ---------- 3. If no row updated, INSERT ----------
    if (qry.numRowsAffected() == 0) {
        qry.prepare(R"(
            INSERT INTO lames (glass, cant, lames, fecha, partida)
            VALUES (?, ?, ?, ?, ?)
        )");

        qry.addBindValue(glass);
        qry.addBindValue(cant);
        qry.addBindValue(lamesCount);
        qry.addBindValue(fecha.toString("yyyy-MM-dd"));
        qry.addBindValue(partida);

        if (!qry.exec()) {
            // Possible race condition: someone inserted the same (glass,lames)
            // after our UPDATE but before INSERT.  Handle it gracefully:
            const int err = qry.lastError().nativeErrorCode().toInt();
            // 2601 / 2627 = unique-constraint violations in SQL Server.
            if (err == 2601 || err == 2627) {
                // Try the UPDATE again.
                if (!qry.lastError().isValid()) {}            // silence clang-tidy
                qry.clear();
                qry.prepare(R"(
                    UPDATE lames
                    SET    cant    = ?,      fecha = ?,      partida = ?
                    WHERE  glass   = ?  AND  lames = ?
                )");
                qry.addBindValue(cant);
                qry.addBindValue(fecha.toString("yyyy-MM-dd"));
                qry.addBindValue(partida);
                qry.addBindValue(glass);
                qry.addBindValue(lamesCount);
                if (!qry.exec()) {
                    qWarning() << "Second UPDATE failed:" << qry.lastError().text();
                    msg->setText("Second UPDATE failed: "+qry.lastError().text());
                    msg->exec();
                    db->Database("GM")->rollback();
                    return;
                }
            } else {
                qWarning() << "INSERT failed:" << qry.lastError().text();
                msg->setText("INSERT failed: "+qry.lastError().text());
                msg->exec();
                db->Database("GM")->rollback();
                return;
            }
        }
    }

    // ---------- 4. Commit ----------
    if (!db->Database("GM")->commit()) {
        qWarning() << "Commit failed:" << db->Database("GM")->lastError().text();
        msg->setText("INSERT failed: "+db->Database("GM")->lastError().text());
        msg->exec();
        db->Database("GM")->rollback();
        return;
    }
    msg->setText("Ok");
    msg->exec();

    Lames->select();
}


void frmPrincipal::on_pushButton_53_clicked()
{
    QSqlQuery qry = QSqlQuery(*db->Database("GM"));
    QString qryText;

    QTableWidgetItem *item;

    double LamesBronze316SqInches = 0;
    double LamesBronze14SqInches = 0;
    double LamesClear14SqInches = 0;
    double LamesBronzeOpac316SqInches = 0;
    double LamesClearOpac316SqInches = 0;
    double LamesDarkGrey316SqInches = 0;
    double LamesBronzeReflective316SqInches = 0;

    double LamesBronze316SqInchesT = 0;
    double LamesBronze14SqInchesT = 0;
    double LamesClear14SqInchesT = 0;
    double LamesBronzeOpac316SqInchesT = 0;
    double LamesClearOpac316SqInchesT = 0;
    double LamesDarkGrey316SqInchesT = 0;
    double LamesBronzeReflective316SqInchesT = 0;

    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText =   "SELECT glass "
              ",cant "
              ",lames "
              ",fecha "
              "FROM lames_from_retail "
              "WHERE fecha <= '%1' AND fecha >= '%2' ";

    if(!qry.exec(qryText.arg(ui->dateRepDPTLamesTo->date().toString("yyyy-MM-dd"))
                        .arg(ui->dateRepDPTLamesFrom->date().toString("yyyy-MM-dd"))))
    {
        qDebug() << "error load dpt_lames_report" << qry.lastError().text();
        qDebug() << qry.executedQuery();
        msg->setText("error load dpt_lames_report"+qry.lastError().text());
        msg->exec();
    }
    else
    {
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                while ( qry.next() )
                {
                    if (qry.value(0).toString().remove('"') == "Bronze 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronze316SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze 1/4")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronze14SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Clear 1/4")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesClear14SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze Opac 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronzeOpac316SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Clear OP 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesClearOpac316SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze Reflective 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronzeReflective316SqInches += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Dark Grey 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesDarkGrey316SqInches += (lame*4)*cant;
                    }
                }
            }
        }
    }

    qryText =   "SELECT glass "
              ",cant "
              ",lames "
              ",fecha "
              "FROM lames_transformation "
              "WHERE fecha <= '%1' AND fecha >= '%2' ";

    if(!qry.exec(qryText.arg(ui->dateRepDPTLamesTo->date().toString("yyyy-MM-dd"))
                      .arg(ui->dateRepDPTLamesFrom->date().toString("yyyy-MM-dd"))))
    {
        qDebug() << "error load dpt_lamesT_report" << qry.lastError().text();
        qDebug() << qry.executedQuery();
        msg->setText("error load dpt_lamesT_report"+qry.lastError().text());
        msg->exec();
    }
    else
    {
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                while ( qry.next() )
                {
                    if (qry.value(0).toString().remove('"') == "Bronze 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronze316SqInchesT += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze 1/4")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronze14SqInchesT += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Clear 1/4")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesClear14SqInchesT += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze Opac 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronzeOpac316SqInchesT += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Clear OP 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesClearOpac316SqInchesT += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze Reflective 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesBronzeReflective316SqInchesT += (lame*4)*cant;
                    }

                    if (qry.value(0).toString().remove('"') == "Dark Grey 3/16")
                    {
                        int cant = qry.value(1).toInt();
                        int lame = qry.value(2).toInt();
                        LamesDarkGrey316SqInchesT += (lame*4)*cant;
                    }
                }
            }
        }
    }

    //BRONZE 3/16
    //prod
    item = new QTableWidgetItem(QString::number((LamesBronze316SqInches/144)/75.83,'f',2));
    ui->table_lames_2->setItem(0,1,item);
    item = new QTableWidgetItem(QString::number((LamesBronze316SqInches/144),'f',2));
    ui->table_lames_2->setItem(0,2,item);
    //trans
    item = new QTableWidgetItem(QString::number((LamesBronze316SqInchesT/144)/75.83,'f',2));
    ui->table_lames_2->setItem(0,3,item);
    item = new QTableWidgetItem(QString::number((LamesBronze316SqInchesT/144),'f',2));
    ui->table_lames_2->setItem(0,4,item);
    //all
    item = new QTableWidgetItem(QString::number(((LamesBronze316SqInches+LamesBronze316SqInchesT)/144)/75.83,'f',2));
    ui->table_lames_2->setItem(0,5,item);
    item = new QTableWidgetItem(QString::number(((LamesBronze316SqInches+LamesBronze316SqInchesT)/144),'f',2));
    ui->table_lames_2->setItem(0,6,item);

    //BRONZE 1/4
    //prod
    item = new QTableWidgetItem(QString::number((LamesBronze14SqInches/144)/75.83,'f',2));
    ui->table_lames_2->setItem(1,1,item);
    item = new QTableWidgetItem(QString::number((LamesBronze14SqInches/144),'f',2));
    ui->table_lames_2->setItem(1,2,item);
    //trans
    item = new QTableWidgetItem(QString::number((LamesBronze14SqInchesT/144)/75.83,'f',2));
    ui->table_lames_2->setItem(1,3,item);
    item = new QTableWidgetItem(QString::number((LamesBronze14SqInchesT/144),'f',2));
    ui->table_lames_2->setItem(1,4,item);
    //all
    item = new QTableWidgetItem(QString::number(((LamesBronze14SqInches+LamesBronze14SqInchesT)/144)/75.83,'f',2));
    ui->table_lames_2->setItem(1,5,item);
    item = new QTableWidgetItem(QString::number(((LamesBronze14SqInches+LamesBronze14SqInchesT)/144),'f',2));
    ui->table_lames_2->setItem(1,6,item);

    //BRONZE REF 3/16
    //prod
    item = new QTableWidgetItem(QString::number((LamesBronzeReflective316SqInches/144)/75.83,'f',2));
    ui->table_lames_2->setItem(2,1,item);
    item = new QTableWidgetItem(QString::number((LamesBronzeReflective316SqInches/144),'f',2));
    ui->table_lames_2->setItem(2,2,item);
    //trans
    item = new QTableWidgetItem(QString::number((LamesBronzeReflective316SqInchesT/144)/75.83,'f',2));
    ui->table_lames_2->setItem(2,3,item);
    item = new QTableWidgetItem(QString::number((LamesBronzeReflective316SqInchesT/144),'f',2));
    ui->table_lames_2->setItem(2,4,item);
    //all
    item = new QTableWidgetItem(QString::number(((
                LamesBronzeReflective316SqInches+LamesBronzeReflective316SqInchesT)/144)/75.83,'f',2));
    ui->table_lames_2->setItem(2,5,item);
    item = new QTableWidgetItem(QString::number(((
                LamesBronzeReflective316SqInches+LamesBronzeReflective316SqInchesT)/144),'f',2));
    ui->table_lames_2->setItem(2,6,item);

    //BRONZE OP 3/16
    //prod
    item = new QTableWidgetItem(QString::number((LamesBronzeOpac316SqInches/144)/47.5,'f',2));
    ui->table_lames_2->setItem(3,1,item);
    item = new QTableWidgetItem(QString::number((LamesBronzeOpac316SqInches/144),'f',2));
    ui->table_lames_2->setItem(3,2,item);
    //trans
    item = new QTableWidgetItem(QString::number((LamesBronzeOpac316SqInchesT/144)/47.5,'f',2));
    ui->table_lames_2->setItem(3,3,item);
    item = new QTableWidgetItem(QString::number((LamesBronzeOpac316SqInchesT/144),'f',2));
    ui->table_lames_2->setItem(3,4,item);
    //all
    item = new QTableWidgetItem(QString::number(((
                LamesBronzeOpac316SqInches+LamesBronzeOpac316SqInchesT)/144)/47.5,'f',2));
    ui->table_lames_2->setItem(3,5,item);
    item = new QTableWidgetItem(QString::number(((
                LamesBronzeOpac316SqInches+LamesBronzeOpac316SqInchesT)/144),'f',2));
    ui->table_lames_2->setItem(3,6,item);


    //CLEAR 1/4
    //prod
    item = new QTableWidgetItem(QString::number((LamesClear14SqInches/144)/75.83,'f',2));
    ui->table_lames_2->setItem(4,1,item);
    item = new QTableWidgetItem(QString::number((LamesClear14SqInches/144),'f',2));
    ui->table_lames_2->setItem(4,2,item);
    //trans
    item = new QTableWidgetItem(QString::number((LamesClear14SqInchesT/144)/75.83,'f',2));
    ui->table_lames_2->setItem(4,3,item);
    item = new QTableWidgetItem(QString::number((LamesClear14SqInchesT/144),'f',2));
    ui->table_lames_2->setItem(4,4,item);
    //all
    item = new QTableWidgetItem(QString::number(((LamesClear14SqInches+LamesClear14SqInchesT)/144)/75.83,'f',2));
    ui->table_lames_2->setItem(4,5,item);
    item = new QTableWidgetItem(QString::number(((LamesClear14SqInches+LamesClear14SqInchesT)/144),'f',2));
    ui->table_lames_2->setItem(4,6,item);

    //CLEAR OP 3/16
    //prod
    item = new QTableWidgetItem(QString::number((LamesClearOpac316SqInches/144)/47.5,'f',2));
    ui->table_lames_2->setItem(5,1,item);
    item = new QTableWidgetItem(QString::number((LamesClearOpac316SqInches/144),'f',2));
    ui->table_lames_2->setItem(5,2,item);
    //trans
    item = new QTableWidgetItem(QString::number((LamesClearOpac316SqInchesT/144)/47.5,'f',2));
    ui->table_lames_2->setItem(5,3,item);
    item = new QTableWidgetItem(QString::number((LamesClearOpac316SqInchesT/144),'f',2));
    ui->table_lames_2->setItem(5,4,item);
    //all
    item = new QTableWidgetItem(QString::number(((LamesClearOpac316SqInches+LamesClearOpac316SqInchesT)/144)/47.5,'f',2));
    ui->table_lames_2->setItem(5,5,item);
    item = new QTableWidgetItem(QString::number(((LamesClearOpac316SqInches+LamesClearOpac316SqInchesT)/144),'f',2));
    ui->table_lames_2->setItem(5,6,item);

    //DARK GRAY 3/16
    //prod
    item = new QTableWidgetItem(QString::number((LamesDarkGrey316SqInches/144)/75.83,'f',2));
    ui->table_lames_2->setItem(6,1,item);
    item = new QTableWidgetItem(QString::number((LamesDarkGrey316SqInches/144),'f',2));
    ui->table_lames_2->setItem(6,2,item);
    //trans
    item = new QTableWidgetItem(QString::number((LamesDarkGrey316SqInchesT/144)/75.83,'f',2));
    ui->table_lames_2->setItem(6,3,item);
    item = new QTableWidgetItem(QString::number((LamesDarkGrey316SqInchesT/144),'f',2));
    ui->table_lames_2->setItem(6,4,item);
    //all
    item = new QTableWidgetItem(QString::number(((LamesDarkGrey316SqInches+LamesDarkGrey316SqInchesT)/144)/75.83,'f',2));
    ui->table_lames_2->setItem(6,5,item);
    item = new QTableWidgetItem(QString::number(((LamesDarkGrey316SqInches+LamesDarkGrey316SqInchesT)/144),'f',2));
    ui->table_lames_2->setItem(6,6,item);
}


void frmPrincipal::on_actionDownload_Orders_Maya_triggered()
{
    QSqlQuery qry2 = QSqlQuery(*db->Database("GM"));
    QSqlQuery qry = QSqlQuery(*db->Database("GP"));
    QString qryText;
    QMessageBox *msg = new QMessageBox();
    msg->setWindowTitle("GlassManager");
    msg->setWindowIcon(QIcon(QPixmap("/Images/aim3.ico")));

    qryText = "SELECT DocumentDateTime"
              ",BillToID"
              ",WorkorderNumber "
              ",DocumentNumber "
              ",QuoteNumber "
              "FROM [GlasPacLX_LTD].[dbo].[Document] INNER JOIN "
              "[GlasPacLX_LTD].[dbo].[DocumentDetail] "
              "ON [DocumentGUID] = [DocumentGUID_FK] "
              "WHERE DocumentType_ENUM <> 0 AND BranchGUID_FK <> '65F02099-8E4C-4483-91B8-90AEF2838E76' "
              "AND DocumentDateTime >= '%1' AND DocumentDateTime < '%2' "
              "GROUP BY WorkorderNumber,DocumentDateTime,BillToID,DocumentNumber,QuoteNumber "
              "ORDER BY WorkorderNumber;";

    //"AND WorkorderNumber = '%1' OR DocumentNumber = '%1' "
    //DocumentType_ENUM ---->>> 0 Quote -- 1 WOrkOrder --- 2 Invoice

    //QString d = "30/01/2023";
    QDateTime dateFrom;
    QDateTime dateTo;
    //date.setDate(QDate::fromString(d,"dd/MM/yyyy"));
    dateFrom.setDate(ui->date_CargarOrdenes->date());
    dateFrom.setTime(QTime(0,0,0,0));
    dateTo = dateFrom.addDays(1);

    if(!qry.exec(qryText.arg(dateFrom.toString("MM-dd-yyyy hh:mm:ss")).arg(dateTo.toString("MM-dd-yyyy hh:mm:ss"))))
    {
        qDebug() << "error load order_maya" << qry.lastError().text();
        qDebug() << qryText.arg(dateFrom.toString("MM-dd-yyyy hh:mm:ss")).arg(dateTo.toString("MM-dd-yyyy hh:mm:ss"));
        msg->setText("error load order_maya");
        msg->exec();
    }
    else
    {
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                /*
                QSqlQueryModel *test = new QSqlQueryModel(this);
                test->setQuery(qry);
                ui->tableView->setModel(test);
                */

                while ( qry.next() )
                {
                    qryText = "INSERT INTO orders_maya("
                              "workorder,invoice,quote,client_name,date,status,livre_1,livre_2) "
                              "VALUES('%1','%2','%3','%4','%5','%6','%7','%8');";

                    QString j = qry.value(0).toString();
                    j.truncate(10);
                    qDebug() << QDateTime::fromString(j,"yyyy-MM-dd").toString("yy-MM-dd");

                    if(!qry2.exec(qryText
                                       .arg(qry.value(2).toString())
                                       .arg(qry.value(3).toString())
                                       .arg(qry.value(4).toString())
                                       .arg(qry.value(1).toString().remove("'"))
                                       .arg(j)
                                       .arg("OK Production")
                                       .arg("")
                                       .arg("")))
                    {
                        qDebug() << "error orders_maya" << qry2.lastError().text();
                        qDebug() << qry.value(2).toString() + "-" +qry.value(1).toString();
                        if (!qry2.lastError().text().contains("Violation of UNIQUE KEY constraint"))
                        {
                            msg->setText("error inserting orders_maya - "+qry2.lastQuery()+" "+qry2.lastError().text());
                            msg->exec();
                        }
                    }
                    else
                    {
                        qDebug() << qry.value(2).toString() + "-" +qry.value(1).toString();
                        qDebug() << qry2.lastInsertId().toInt();
                        InsertMozoMaya(qry.value(2).toString(), qry2.lastInsertId().toInt());
                        InsertStatusChange(qry2.lastInsertId().toInt(),"Created",LoggedUser,j);
                    }
                }

                msg->setText("Ordenes cargadas correctamente!");
                msg->exec();

                OrdersMaya->select();

                ui->tableView->setModel(Orders);
            }
        }

    }

    Orders_IN->select();
    Orders_Liv->select();
}


void frmPrincipal::on_pushButton_51_clicked()
{
    // assume you already opened your QSqlDatabase `db`

    QSqlQueryModel *model = new QSqlQueryModel(this);

    // assume open QSqlDatabase db + a tableView* in your UI
    QDate from = ui->dateRepDPTvsGPFrom_2->date();   // for example
    QDate to   = ui->dateRepDPTvsGPTo_2->date();

    QSqlQuery qry(*db->Database("GM"));
    qry.prepare(R"(
    SELECT  glass,
            consumo_sheets  AS [Consumo Sheets],
            consumo_sqft    AS [Consumo ft²],
            mozo_sheets     AS [Mozo Sheets],
            mozo_sqft       AS [Mozo ft²],
            lames_sheets    AS [Lames Sheets],
            lames_sqft      AS [Lames ft²],
            diff_sheets     AS [Δ Sheets],
            diff_sqft       AS [Δ ft²]
    FROM dbo.fn_GlassUsageReport(?, ? , ?)      -- ← 3 placeholders
    ORDER BY glass
)");
    qry.addBindValue(from);   // first “?”  (QVariant → DATE)
    qry.addBindValue(to);     // second “?”
    qry.addBindValue(75.83);


    if (!qry.exec())
    {
        qWarning() << "Report query failed:" << qry.lastError().text();
    }

    model->setQuery(qry);
    ui->tableRepDPTvsGP_2->setModel(model);
    ui->tableRepDPTvsGP_2->setAlternatingRowColors(true);
    ui->tableRepDPTvsGP_2->resizeColumnsToContents();
    ui->tableRepDPTvsGP_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableRepDPTvsGP_2->setSortingEnabled(true);
}


void frmPrincipal::on_pushButton_60_clicked()
{
    Lames->select();
}

