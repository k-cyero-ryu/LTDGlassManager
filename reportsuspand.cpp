#include "reportsuspand.h"
#include "ui_reportsuspand.h"
#include <QUrl>
#include <QFileDialog>
#include <QPainter>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include "qtableviewprinter.h"
#include <QSqlRelationalTableModel>

reportSuspand::reportSuspand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reportSuspand)
{
    ui->setupUi(this);

    orders_suspand = new QSqlTableModel(this);
    orders_suspand->setTable("orders");

    QString f = " status = 'OK Delivery' or status = 'IN Production' ";
    orders_suspand->setFilter(f);

    ui->tableView->setModel(orders_suspand);

    //workorder,invoice,quote,client_name,date,status,cuting_table
    orders_suspand->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    orders_suspand->setHeaderData(1, Qt::Horizontal, QObject::tr("WorkOrder"));
    orders_suspand->setHeaderData(2, Qt::Horizontal, QObject::tr("Invoice"));
    orders_suspand->setHeaderData(3, Qt::Horizontal, QObject::tr("Quote"));
    orders_suspand->setHeaderData(4, Qt::Horizontal, QObject::tr("Client"));
    orders_suspand->setHeaderData(5, Qt::Horizontal, QObject::tr("Invoice Date"));
    orders_suspand->setHeaderData(6, Qt::Horizontal, QObject::tr("Status"));
    orders_suspand->setHeaderData(7, Qt::Horizontal, QObject::tr("Livre Depo"));
    orders_suspand->setHeaderData(8, Qt::Horizontal, QObject::tr("Livre LTD"));
    orders_suspand->setHeaderData(9, Qt::Horizontal, QObject::tr("Cuting Table"));

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);

    orders_suspand->select();



}

reportSuspand::~reportSuspand()
{
    delete ui;
}

void reportSuspand::print_orders(QPrinter *printer)
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
    title->setMtitle2("Orders Suspand: "+ QDate::currentDate().toString("yyyy-MM-dd"));//report Name
    QStringList side = { "Phone: +(509) 2813-4040     Email: info@ltdconstruction-haiti.com",
                         "    ",
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

void reportSuspand::on_pushButton_2_clicked()
{
    this->close();
}


void reportSuspand::on_pushButton_clicked()
{
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print_orders(QPrinter*)));
    dialog.exec();
}

