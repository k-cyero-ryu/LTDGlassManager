#ifndef FRMPRINCIPAL_H
#define FRMPRINCIPAL_H

#include <QMainWindow>
#include <QTimerEvent>
//#include <connection.h>
#include "newlamesdiag.h"
#include "newretaildiag.h"
#include "viewretailslames.h"
#include "reportconsumo.h"
#include "viewmozo.h"
#include "checkglasspackreport.h"
#include "databasecontroller.h"


class QSqlTableModel;
class QSqlRelationalTableModel;
class QTableWidget;
class QSqlQueryModel;

QT_BEGIN_NAMESPACE
namespace Ui { class frmPrincipal; }
QT_END_NAMESPACE

class frmPrincipal : public QMainWindow
{
    Q_OBJECT
    //class connection *db;
    DatabaseController *db;

    //Modelos
    QSqlTableModel *Mesas;
    QSqlTableModel *Glass;
    QSqlTableModel *Racks;
    QSqlTableModel *Stock; //NEW
    QSqlTableModel *Lames; //NEW
    QSqlTableModel *LamesProd; //NEW
    QSqlTableModel *LamesTrans; //NEW
    QSqlTableModel *Consumos;
    QSqlTableModel *Orders;
    QSqlTableModel *OrdersMaya; //NEW
    QSqlTableModel *Orders_IN;
    QSqlTableModel *Orders_Liv;
    QSqlTableModel *Orders_Finish;
    QSqlTableModel *Orders_Search;
    QSqlTableModel *Orders_Log;
    QSqlRelationalTableModel *consumo_r;

    //ids
    int IdConsumo;

    int timerId;
    QString LoggedUser;

public:
    frmPrincipal(QWidget *parent = nullptr);
    ~frmPrincipal();

    //connection* GetDatabase();
    DatabaseController* GetDatabase();

    void Configure();
    bool Configured();

    bool InsertMozo(QString WorkOrder,int IdOrder);
    bool InsertMozoMaya(QString WorkOrder,int IdOrder);
    QString StatusByWorkOrder(QString WO);
    int IdByWorkOrder(QString WO);
    bool InsertStatusChange(int IdOrder, QString StatusChange, QString User, QString Date);
    void populateTableWidgetFromQueryModel(QTableWidget* tableWidget, QSqlQueryModel* queryModel);

    void loadMozo();


private slots:
    void print_find2(QPrinter *printer);
    void print_workorder(QPrinter *printer);
    void print_invoice(QPrinter *printer);
    void print_oncoupe(QPrinter *printer);
    void print_coupe(QPrinter *printer);
    void print_livre(QPrinter *printer);
    void print_RepGlassPackTotal(QPrinter *printer);
    void print_RepUsageWO(QPrinter *printer);
    void print_RepUsageTable(QPrinter *printer);
    void print_RepUsageDPT(QPrinter *printer);
    void print_RepGlassPvsDPT(QPrinter *printer);

    void on_actionCargar_Ordenes_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_clicked();

    void on_actionHacer_Backup_triggered();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_11_clicked();

    void on_actionSalir_triggered();

    void on_btn_consumo_plusretail_clicked();

    void on_btn_consumo_pluslames_clicked();

    void on_tableView_consumo_doubleClicked(const QModelIndex &index);

    void on_cmb_consumo_workoerder_currentIndexChanged(int index);

    void on_actionConsumo_triggered();

    void on_tableView_consumo_activated(const QModelIndex &index);

    void on_cmb_consumo_workoerder_activated(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_actionCheck_GlassPack_triggered();

    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_table_Order_IN_doubleClicked(const QModelIndex &index);

    void on_table_livre_doubleClicked(const QModelIndex &index);

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_22_clicked();

    void on_tableView_search_doubleClicked(const QModelIndex &index);

    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_37_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_38_clicked();

    void on_table_invoice_doubleClicked(const QModelIndex &index);

    void on_table_finalize_doubleClicked(const QModelIndex &index);

    void on_actionReport_Suspand_triggered();

    void on_pushButton_39_clicked();

    void on_pushButton_40_clicked();

    void on_pushButton_41_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_50_clicked();

    void on_cmb_RepUsoMesaTable_currentIndexChanged(int index);

    void on_cmb_RepUsoMesaTable_currentTextChanged(const QString &arg1);

    void on_cmb_cons_workorder_activated(int index);

    void on_cmb_cons_workorder_currentIndexChanged(int index);

    void on_cmb_cons_workorder_currentTextChanged(const QString &arg1);

    void on_cmb_cons_workorder_editTextChanged(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_date_cons_dateChanged(const QDate &date);

    void on_pushButton_55_clicked();

    void on_pushButton_58_clicked();

    void on_pushButton_53_clicked();

    void on_actionDownload_Orders_Maya_triggered();

    void on_pushButton_51_clicked();

    void on_pushButton_60_clicked();

private:
    Ui::frmPrincipal *ui;

protected:
    void timerEvent(QTimerEvent *event);
};
#endif // FRMPRINCIPAL_H
