#ifndef QTABLEVIEWPRINTER_H
#define QTABLEVIEWPRINTER_H

#include <QPen>
#include <QFont>
#include <QPrinter>
#include <QTableView>

class QPainter;
class QAbstractItemModel;

enum TitleFlag{
    FIRST_PAGE=0X1,
    EVERY_PAGE=0X2,
};

struct PrintColumn{
    QString name;
    int modelColumn;
    int columnWidth;
};

struct mergeColumn{
    QString text;
    QStringList columnList;
};

typedef QList<PrintColumn*> PrintColumnList;
typedef QHash<int,QList<mergeColumn*>> MergeColumnHash;


//título
class PageTitle
{
public:
    PageTitle(QPainter *painter, QPrinter* printer);
    virtual ~PageTitle(){}
    void setPageTitle(const QString& title);
    void setTitleFont(QFont font);
    void setSideTitleFont(QFont font);
    void setSideTitle(const QStringList& sidetitle);
    QString getMtitle2() const;
    void setMtitle2(const QString &value);
    QFont getTitle2Font() const;
    void setTitle2Font(const QFont &value);


public:
    virtual void startDrawTitle();




private:
    QPainter* mpainter;
    QPrinter* mprinter;
    QFont titleFont;
    QFont title2Font;
    QFont sidetitleFont;
    QString mtitle;
    QString mtitle2;
    QStringList msideTitles;
};

//Header
class PageHeader
{
public:
    PageHeader(QPainter *painter);
    virtual ~PageHeader(){}
    void setPageHeaderFont(QFont font);
public:
    virtual void startDrawHeader();
private:
    QPainter* mpainter;
    QFont pageHeaderFont;
};

//Pie de página
class PageFooter
{
public:
    PageFooter(QPainter *painter);
    ~PageFooter(){}
    virtual void startDrawFooter();
    void setCreater(const QString& name);
    void setPageFooterFont(QFont font);
    void resetPageNumber();
    void setViewFlag(bool zbr=true, bool ym=true, bool zbsj=true);
private:
    QPainter* mpainter;
    static int pageNumber;
    QString creater;
    QFont pageFooterFont;
    bool mzbr,mym, mzbsj;
};

//Contenido principal
class QTableViewPrinter
{
public:
    QTableViewPrinter(QPainter *painter, QPrinter *printer);
    bool printTable(QTableView* tableView, const QStringList &totalFields, const QStringList &visualFields);
    QString lastError();
    static int headerHeight;
    static int leftBlank;
    static int rightBlank;

    //Establecer el espacio entre celdas
    void setCellMargin(int left = 10, int right = 5, int top = 5, int bottom = 5);
    //Establecer márgenes de tabla
    void setPageMargin(int left = 50, int right = 20, int top = 20, int bottom = 20);
    //Establecer pincel de dibujo de página
    void setPen(QPen pen);
    //Establecer fuente de encabezado de tabla
    void setHeadersFont(QFont font);
    //Establecer fuente de contenido de tabla
    void setContentFont(QFont font);
    //Establecer el color de fuente del encabezado de la tabla
    void setHeaderColor(QColor color);
    //Establecer el color del contenido de la tabla
    void setContentColor(QColor color);
    //Establezca el método de relleno de texto (que excede parte para envolver o no envolver)
    void setTextWordWrap(Qt::TextFlag Wrap);
    //Establecer la altura máxima de la fila
    void setMaxRowHeight(int height);
    //Establecer alineación de texto
    void setTextAlign(Qt::AlignmentFlag align);
    //Establezca el modo de visualización del título (mostrar solo en la primera página o mostrar el título en cada página)
    void setTitleFlag(const TitleFlag type);
    //Establezca el modo de visualización del encabezado de la tabla (se muestra solo en la primera página o se muestra en cada página)
    void setHeaderFlag(bool flag);

    //Obtener instancia de título
    void setPagerTitle(PageTitle *pagetitle);
    //Obtenga la instancia de encabezado
    void setPageHeader(PageHeader *pageheader);
    //Obtener ejemplo de pie de página
    void setPagerFooter(PageFooter *pagefooter);
private:
    QPainter *painter;
    QPrinter *printer;
    PageTitle *pageTitle;
    PageHeader *pageHeader;
    PageFooter *pageFooter;
    QTableView *mTableView;
    QAbstractItemModel *model;

    int topMargin;
    int bottomMargin;
    int leftMargin;
    int rightMargin;
    int bottomHeight;
    int maxRowHeight;
    int mheaderRowHeight;

    QPen pen;
    QFont headersFont;
    QFont contentFont;
    QColor headerColor;
    QColor contentColor;
    QStringList mHeaderTitles;
    Qt::TextFlag textLineFlag;
    Qt::AlignmentFlag textAlign;
    TitleFlag titleFlag;
    PrintColumnList mColumnList;
    MergeColumnHash _mergeColumnHash;
    bool mheaderFlag;
    QString error;

private:
    //Obtener la posición actual de la pluma (para probar)
    void position(const QString& str);
    //Analizar datos de encabezado de tabla (distinguir columnas ocultas, columnas que necesitan fusionarse y calcular anchos de columna)
    bool setPrintColumnList(const QStringList &list);
    //Método de dibujo del cuerpo
    bool paintTable(int row=0, int column=0, bool headFlag=false);
};
#endif // QTABLEVIEWPRINTER_H
