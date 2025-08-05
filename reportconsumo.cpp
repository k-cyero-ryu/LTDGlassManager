#include "reportconsumo.h"
#include "ui_reportconsumo.h"

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
#include "qtableviewprinter.h"
#include <cmath>

using namespace std;

reportConsumo::reportConsumo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reportConsumo)
{
    ui->setupUi(this);

    ui->date_to->setDate(QDate::currentDate());
    ui->date_from->setDate(QDate::currentDate());

    ui->table->setColumnWidth(1,200);
    ui->table->setColumnWidth(2,200);

}

reportConsumo::~reportConsumo()
{
    delete ui;
}

void reportConsumo::on_pushButton_clicked()
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database("GM"));
    //QSqlQuery qry2 = QSqlQuery(QSqlDatabase::database("GM"));
    QString qryText;

    QTableWidgetItem *item;

    double Bronze316Total = 0;
    int Bronze316Sheets = 0;
    double LamesBronze316SqInches = 0;
    double Bronze14Total = 0;
    int Bronze14Sheets = 0;
    double LamesBronze14SqInches = 0;
    double Clear14Total = 0;
    int Clear14Sheets = 0;
    double LamesClear14SqInches = 0;
    double BronzeOpac316Total = 0;
    int BronzeOpac316Sheets = 0;
    double LamesBronzeOpac316SqInches = 0;
    double ClearOpac316Total = 0;
    int ClearOpac316Sheets = 0;
    double LamesClearOpac316SqInches = 0;
    double DarkGrey316Total = 0;
    int DarkGrey316Sheets = 0;
    double LamesDarkGrey316SqInches = 0;
    double DarkGrey14Total = 0;
    int DarkGrey14Sheets = 0;
    double BronzeReflective316Total = 0;
    int BronzeReflective316Sheets = 0;
    double LamesBronzeReflective316SqInches = 0;
    double MirrorSilvered316Total = 0;
    int MirrorSilvered316Sheets = 0;
    double MirrorSilvered14Total = 0;
    int MirrorSilvered14Sheets = 0;
    double OpacAciditTotal = 0;
    int OpacAciditSheets = 0;
    double DarkBlueTotal = 0;
    int DarkBlueSheets = 0;

    double Bronze316Retail = 0;
    double Bronze14Retail = 0;
    double Clear14Retail = 0;
    double BronzeOpacRetail = 0;
    double ClearOpacRetail = 0;
    double DarkGray316Retail = 0;
    double DarkGray14Retail = 0;
    double BronzeReflectiveRetail = 0;
    double Mirror14Retail = 0;



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
    if(!qry.exec(qryText.arg(ui->date_to->date().toString("yyyy-MM-dd"))
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

    //CALCULATE TOTAL AMOUNT OF SHEETS OF GLASSS TAKEN FROM STOCK

    qryText =   "SELECT glass "
                        ",sheets "
                        ",fecha "
                        ",id "
                "FROM consumo "
                "WHERE fecha <= '%1' AND fecha >= '%2' ";

    if(!qry.exec(qryText.arg(ui->date_to->date().toString("yyyy-MM-dd"))
                 .arg(ui->date_from->date().toString("yyyy-MM-dd"))))
    {
        qDebug() << "error load consumo" << qry.lastError().text();
        qDebug() << qry.executedQuery();
        qDebug() << qryText.arg(ui->date_to->date().toString("yyyy-MM-dd"))
                        .arg(ui->date_from->date().toString("yyyy-MM-dd"));
        msg->setText("error load consumo");
        msg->exec();
    }
    else
    {
        //qDebug() << qry2.executedQuery();
        if (qry.isActive())
        {
            if (qry.isSelect())
            {
                while ( qry.next() )
                {
                    if (qry.value(0).toString().remove('"') == "Bronze 3/16")
                    {
                        Bronze316Sheets += qry.value(1).toInt();
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze 1/4")
                    {
                        Bronze14Sheets += qry.value(1).toInt();
                    }

                    if (qry.value(0).toString().remove('"') == "Clear 1/4")
                    {
                        Clear14Sheets += qry.value(1).toInt();
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze Opac 3/16")
                    {
                        BronzeOpac316Sheets += qry.value(1).toInt();
                    }

                    if (qry.value(0).toString().remove('"') == "Clear Opac 3/16")
                    {
                        ClearOpac316Sheets += qry.value(1).toInt();
                    }

                    if (qry.value(0).toString().remove('"') == "Bronze Reflective 3/16")
                    {
                        BronzeReflective316Sheets += qry.value(1).toInt();
                    }

                    if (qry.value(0).toString().remove('"') == "Dark Grey 3/16")
                    {
                        DarkGrey316Sheets += qry.value(1).toInt();
                    }

                    if (qry.value(0).toString().remove('"') == "Dark Grey 1/4")
                    {
                        DarkGrey14Sheets += qry.value(1).toInt();
                    }

                    if (qry.value(0).toString().remove('"') == "Mirror 3/16")
                    {
                        MirrorSilvered316Sheets += qry.value(1).toInt();
                    }

                    if (qry.value(0).toString().remove('"') == "Mirror 1/4")
                    {
                        MirrorSilvered14Sheets += qry.value(1).toInt();
                    }

                }
            }
        }
    }

    //CALCULATE AREA IN SQUARE FEET OF LAMES CUT FROM RETAILS OF EACH TYPE PF GLASS

    qryText =   "SELECT glass "
                        ",cant "
                        ",lames "
                        ",fecha "
                "FROM lames_from_retail "
                "WHERE fecha <= '%1' AND fecha >= '%2' ";

    if(!qry.exec(qryText.arg(ui->date_to->date().toString("yyyy-MM-dd"))
                 .arg(ui->date_from->date().toString("yyyy-MM-dd"))))
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


    //CALCULATE AREA OF THE RETAILS PRODUCED FOR EACH TYOE OF GLASS

    qryText =   "SELECT glass "
                        ",heigh_ "
                        ",with_  "
                        ",fecha "
                "FROM retail "
                "WHERE fecha <= '%1' AND fecha >= '%2' ";

    if(!qry.exec(qryText.arg(ui->date_to->date().toString("yyyy-MM-dd"))
                 .arg(ui->date_from->date().toString("yyyy-MM-dd"))))
    {
        qDebug() << "error load retail" << qry.lastError().text();
        qDebug() << qry.executedQuery();
        msg->setText("error load retail");
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
                    //Bronze 3/16
                    if (qry.value(0).toString().remove('"') == QString("Bronze 3/16"))
                    {
                        double high = FractToDec(qry.value(1).toString()).toDouble();
                        double with = FractToDec(qry.value(2).toString()).toDouble();

                        double area = (high*with);
                        Bronze316Retail += area/144;
                    }

                    //Bronze 1/4
                    if (qry.value(0).toString().remove('"') == QString("Bronze 1/4"))
                    {
                        double high = FractToDec(qry.value(1).toString()).toDouble();
                        double with = FractToDec(qry.value(2).toString()).toDouble();

                        double area = (high*with);
                        Bronze14Retail += area/144;
                    }

                    //Clear 1/4
                    if (qry.value(0).toString().remove('"') == QString("Clear 1/4"))
                    {
                        double high = FractToDec(qry.value(1).toString()).toDouble();
                        double with = FractToDec(qry.value(2).toString()).toDouble();

                        double area = (high*with);
                        Clear14Retail += area/144;
                    }

                    //Bronze OP 3/16"
                    if (qry.value(0).toString().remove('"') == QString("Bronze Opac 3/16"))
                    {
                        double high = FractToDec(qry.value(1).toString()).toDouble();
                        double with = FractToDec(qry.value(2).toString()).toDouble();

                        double area = (high*with);
                        BronzeOpacRetail += area/144;
                    }

                    //Clear OP 3/16
                    if (qry.value(0).toString().remove('"') == QString("Clear Opac 3/16"))
                    {
                        double high = FractToDec(qry.value(1).toString()).toDouble();
                        double with = FractToDec(qry.value(2).toString()).toDouble();

                        double area = (high*with);
                        ClearOpacRetail += area/144;
                    }

                    //Esp Bronze 3/16
                    if (qry.value(0).toString().remove('"') == QString("Bronze Reflective 3/16"))
                    {
                        double high = FractToDec(qry.value(1).toString()).toDouble();
                        double with = FractToDec(qry.value(2).toString()).toDouble();

                        double area = (high*with);
                        BronzeReflectiveRetail += area/144;
                    }

                    //Dark gray 3/16
                    if (qry.value(0).toString().remove('"') == QString("Dark gray 3/16"))
                    {
                        double high = FractToDec(qry.value(1).toString()).toDouble();
                        double with = FractToDec(qry.value(2).toString()).toDouble();

                        double area = (high*with);
                        DarkGray316Retail += area/144;
                    }

                    //Dark gray 1/4
                    if (qry.value(0).toString().remove('"') == QString("Dark gray 1/4"))
                    {
                        double high = FractToDec(qry.value(1).toString()).toDouble();
                        double with = FractToDec(qry.value(2).toString()).toDouble();

                        double area = (high*with);
                        DarkGray14Retail += area/144;
                    }

                    //Mirror Silvered 1/4
                    if (qry.value(0).toString().remove('"') == QString("Miroir 1/4"))
                    {
                        double high = FractToDec(qry.value(1).toString()).toDouble();
                        double with = FractToDec(qry.value(2).toString()).toDouble();

                        double area = (high*with);
                        Mirror14Retail += area/144;
                    }

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

    double sheets =0;
    double sys = 0;
    double lams = 0;
    double retail = 0;

    //bronze 3/16
    item = new QTableWidgetItem(QString::number(Bronze316Sheets));
    ui->table->setItem(0,1,item);
    item = new QTableWidgetItem(QString::number(Bronze316Total/75.83,'f',2));
    ui->table->setItem(0,2,item);
    item = new QTableWidgetItem(QString::number(Bronze316Total,'f',2));
    ui->table->setItem(0,3,item);
    item = new QTableWidgetItem(QString::number((LamesBronze316SqInches/144)/75.83,'f',2));
    ui->table->setItem(0,4,item);
    sheets = QString::number(Bronze316Sheets).toDouble();
    sys = Bronze316Total/75.83;
    lams = (LamesBronze316SqInches/144)/75.83;    

    double dif =  sheets - sys - lams - (Bronze316Retail/75.83);
    item = new QTableWidgetItem(QString::number(Bronze316Retail/75.83,'f',2));
    ui->table->setItem(0,5,item);
    item = new QTableWidgetItem(QString::number(dif,'f',2));
    ui->table->setItem(0,6,item);
    double porcentage = 100 - ((sys / sheets)*100);
    item = new QTableWidgetItem(QString::number(porcentage,'f',2).append("%"));
    ui->table->setItem(0,7,item);

    //bronze 1/4
    item = new QTableWidgetItem(QString::number(Bronze14Sheets));
    ui->table->setItem(1,1,item);
    item = new QTableWidgetItem(QString::number(Bronze14Total/75.83,'f',2));
    ui->table->setItem(1,2,item);
    item = new QTableWidgetItem(QString::number(Bronze14Total,'f',2));
    ui->table->setItem(1,3,item);
    item = new QTableWidgetItem(QString::number((LamesBronze14SqInches/144)/75.83,'f',2));
    ui->table->setItem(1,4,item);
    sheets = QString::number(Bronze14Sheets).toDouble();
    sys = Bronze14Total/75.83;
    lams = (LamesBronze14SqInches/144)/75.83;

    dif =  sheets - sys - lams - (Bronze14Retail/75.83);
    item = new QTableWidgetItem(QString::number(Bronze14Retail/75.83,'f',2));
    ui->table->setItem(1,5,item);
    item = new QTableWidgetItem(QString::number(dif,'f',2));
    ui->table->setItem(1,6,item);
    porcentage = 100 - ((sys / sheets)*100);
    item = new QTableWidgetItem(QString::number(porcentage,'f',2).append("%"));
    ui->table->setItem(1,7,item);

    //bronze ref 3/16
    item = new QTableWidgetItem(QString::number(BronzeReflective316Sheets));
    ui->table->setItem(2,1,item);
    item = new QTableWidgetItem(QString::number(BronzeReflective316Total/75.83,'f',2));
    ui->table->setItem(2,2,item);
    item = new QTableWidgetItem(QString::number(BronzeReflective316Total,'f',2));
    ui->table->setItem(2,3,item);
    item = new QTableWidgetItem(QString::number((LamesBronzeReflective316SqInches/144)/75.83,'f',2));
    ui->table->setItem(2,4,item);
    sheets = QString::number(BronzeReflective316Sheets).toDouble();
    sys = BronzeReflective316Total/75.83;
    lams = (LamesBronzeReflective316SqInches/144)/75.83;

    dif =  sheets - sys - lams - (BronzeReflectiveRetail/75.83);
    item = new QTableWidgetItem(QString::number(BronzeReflectiveRetail/75.83,'f',2));
    ui->table->setItem(2,5,item);
    item = new QTableWidgetItem(QString::number(dif,'f',2));
    ui->table->setItem(2,6,item);
    porcentage = 100 - ((sys / sheets)*100);
    item = new QTableWidgetItem(QString::number(porcentage,'f',2).append("%"));
    ui->table->setItem(2,7,item);

    //bronze op 3/16
    item = new QTableWidgetItem(QString::number(BronzeOpac316Sheets));
    ui->table->setItem(3,1,item);
    item = new QTableWidgetItem(QString::number(BronzeOpac316Total/47.5,'f',2));
    ui->table->setItem(3,2,item);
    item = new QTableWidgetItem(QString::number(BronzeOpac316Total,'f',2));
    ui->table->setItem(3,3,item);
    item = new QTableWidgetItem(QString::number((LamesBronzeOpac316SqInches/144)/75.83,'f',2));
    ui->table->setItem(3,4,item);
    sheets = QString::number(BronzeOpac316Sheets).toDouble();
    sys = BronzeOpac316Total/47.5;
    lams = (LamesBronzeOpac316SqInches/144)/47.5;

    dif =  sheets - sys - lams - (BronzeOpacRetail/47.5);
    item = new QTableWidgetItem(QString::number(BronzeOpacRetail/47.5,'f',2));
    ui->table->setItem(3,5,item);
    item = new QTableWidgetItem(QString::number(dif,'f',2));
    ui->table->setItem(3,6,item);
    porcentage = 100 - ((sys / sheets)*100);
    item = new QTableWidgetItem(QString::number(porcentage,'f',2).append("%"));
    ui->table->setItem(3,7,item);

    //Clear 1/4
    item = new QTableWidgetItem(QString::number(Clear14Sheets));
    ui->table->setItem(4,1,item);
    item = new QTableWidgetItem(QString::number(Clear14Total/75.83,'f',2));
    ui->table->setItem(4,2,item);
    item = new QTableWidgetItem(QString::number(Clear14Total,'f',2));
    ui->table->setItem(4,3,item);
    item = new QTableWidgetItem(QString::number((LamesClear14SqInches/144)/75.83,'f',2));
    ui->table->setItem(4,4,item);
    sheets = QString::number(Clear14Sheets).toDouble();
    sys = Clear14Total/75.83;
    lams = (LamesClear14SqInches/144)/75.83;

    dif =  sheets - sys - lams - (Clear14Retail/75.83);
    item = new QTableWidgetItem(QString::number(Clear14Retail/75.83,'f',2));
    ui->table->setItem(4,5,item);
    item = new QTableWidgetItem(QString::number(dif,'f',2));
    ui->table->setItem(4,6,item);
    porcentage = 100 - ((sys / sheets)*100);
    item = new QTableWidgetItem(QString::number(porcentage,'f',2).append("%"));
    ui->table->setItem(4,7,item);

    //Clear opa
    item = new QTableWidgetItem(QString::number(ClearOpac316Sheets));
    ui->table->setItem(5,1,item);
    item = new QTableWidgetItem(QString::number(ClearOpac316Total/47.5,'f',2));
    ui->table->setItem(5,2,item);
    item = new QTableWidgetItem(QString::number(ClearOpac316Total,'f',2));
    ui->table->setItem(5,3,item);
    item = new QTableWidgetItem(QString::number((LamesClearOpac316SqInches/144)/75.83,'f',2));
    ui->table->setItem(5,4,item);
    sheets = QString::number(ClearOpac316Sheets).toDouble();
    sys = ClearOpac316Total/47.5;
    lams = (LamesClearOpac316SqInches/144)/47.5;

    dif =  sheets - sys - lams - (ClearOpacRetail/47.5);
    item = new QTableWidgetItem(QString::number(ClearOpacRetail/47.5,'f',2));
    ui->table->setItem(5,5,item);
    item = new QTableWidgetItem(QString::number(dif,'f',2));
    ui->table->setItem(5,6,item);
    porcentage = 100 - ((sys / sheets)*100);
    item = new QTableWidgetItem(QString::number(porcentage,'f',2).append("%"));
    ui->table->setItem(5,7,item);

    //dark gray 3/16
    item = new QTableWidgetItem(QString::number(DarkGrey316Sheets));
    ui->table->setItem(6,1,item);
    item = new QTableWidgetItem(QString::number(DarkGrey316Total/75.83,'f',2));
    ui->table->setItem(6,2,item);
    item = new QTableWidgetItem(QString::number(DarkGrey316Total,'f',2));
    ui->table->setItem(6,3,item);
    item = new QTableWidgetItem(QString::number((LamesDarkGrey316SqInches/144)/75.83,'f',2));
    ui->table->setItem(6,4,item);
    sheets = QString::number(DarkGrey316Sheets).toDouble();
    sys = DarkGrey316Total/75.83;
    lams = (LamesDarkGrey316SqInches/144)/75.83;

    dif =  sheets - sys - lams - (DarkGray316Retail/75.83);
    item = new QTableWidgetItem(QString::number(DarkGray316Retail/75.83,'f',2));
    ui->table->setItem(6,5,item);
    item = new QTableWidgetItem(QString::number(dif,'f',2));
    ui->table->setItem(6,6,item);
    porcentage = 100 - ((sys / sheets)*100);
    item = new QTableWidgetItem(QString::number(porcentage,'f',2).append("%"));
    ui->table->setItem(6,7,item);

    /*
    //mirror 3/16
    item = new QTableWidgetItem(QString::number(MirrorSilvered316Sheets));
    ui->table->setItem(7,1,item);
    item = new QTableWidgetItem(QString::number((MirrorSilvered316Total/144)/75.83,'f',2));
    ui->table->setItem(7,2,item);
    item = new QTableWidgetItem(QString::number(MirrorSilvered316Total/144,'f',2));
    ui->table->setItem(7,3,item);

    //mirror 1/4
    item = new QTableWidgetItem(QString::number(MirrorSilvered14Sheets));
    ui->table->setItem(9,1,item);
    item = new QTableWidgetItem(QString::number(MirrorSilvered14Total/75.83,'f',2));
    ui->table->setItem(9,2,item);
    item = new QTableWidgetItem(QString::number(MirrorSilvered14Total/144,'f',2));
    ui->table->setItem(9,3,item);
*/
    ui->table->setColumnWidth(0,170);
    ui->table->setColumnWidth(3,190);
}

QString reportConsumo::FractToDec(QString fraction)
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

void reportConsumo::print(QPrinter *printer)
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
    for(int i=0;i<ui->table->model()->columnCount();++i){
        colWidth.push_back(ui->table->columnWidth(i));
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
                         "From: "+ui->date_from->date().toString()+" To: "+ui->date_to->date().toString(),
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
    for(int i = 0; i < ui->table->model()->columnCount(); i++)
    {
      headers.append(ui->table->model()->headerData(i, Qt::Horizontal).toString());
      headers2.append(ui->table->model()->headerData(i, Qt::Horizontal).toString());
    }

    //QStringList headers = { "título 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    //QStringList headers2 = { "Encabezado 0" , "Contenido 1" , "Contenido 2" , "Contenido 3", "Contenido 4", "Contenido 5", "Contenido 6"};
    if(!tablePrinter.printTable(ui->table, headers, headers2)) {
        qDebug() << tablePrinter.lastError();
    }
    painter.end();
}

void reportConsumo::on_pushButton_2_clicked()
{
    this->close();
}

void reportConsumo::on_pushButton_3_clicked()
{
    QPrintPreviewDialog dialog;
    connect(&dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print(QPrinter*)));
    dialog.exec();
}
