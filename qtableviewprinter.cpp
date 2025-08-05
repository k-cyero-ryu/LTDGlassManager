#include "qtableviewprinter.h"
#include <QAbstractItemModel>
#include <QPainter>
#include <QDebug>
#include <QDateTime>

//--------Contenido principal-------------
int QTableViewPrinter::headerHeight = 200;
int QTableViewPrinter::leftBlank = 40;
int QTableViewPrinter::rightBlank = 40;

QTableViewPrinter::QTableViewPrinter(QPainter* painter, QPrinter* printer) : painter(painter),printer(printer) {
    //Establecer valor predeterminado
    topMargin = 3;
    bottomMargin = 3;
    leftMargin = 3;
    rightMargin = 3;
    bottomHeight = 40;
    maxRowHeight = 1000;
    mheaderRowHeight = 0;
    pen = painter->pen();
    headersFont = QFont("Times",10,QFont::Bold);
    contentFont = QFont("Times",9);
    headerColor = painter->pen().color();
    contentColor = painter->pen().color();
    pageFooter = NULL;
    pageHeader = NULL;
    pageTitle = NULL;
    error = "No hay error";
    mheaderFlag = true;
    textLineFlag=Qt::TextFlag::TextSingleLine;
    textAlign=Qt::AlignmentFlag::AlignCenter;
    titleFlag=TitleFlag::FIRST_PAGE;
}

bool QTableViewPrinter::printTable(QTableView *tableView, const QStringList &totalFields, const QStringList &visualFields)
{
    //inicialización
    mTableView = tableView;
    mHeaderTitles = totalFields;
    model = tableView->model();
    //Comprobación de errores
    if(!printer->isValid()) {
        error = "Puntero de impresión incorrecto!";
        return false;
    }
    if(!painter->isActive()) {
        error = "Puntero incorrecto!";
        return false;
    }
    if(!setPrintColumnList(visualFields))
        return false;
    //Empieza a dibujar
    if(!paintTable(0,0,true))
        return false;
    return true;
}

QString QTableViewPrinter::lastError() {
    return error;
}

void QTableViewPrinter::setCellMargin(int left, int right, int top, int bottom) {
    topMargin = top;
    bottomMargin = bottom;
    leftMargin = left;
    rightMargin = right;
}

void QTableViewPrinter::setPageMargin(int left, int right, int top, int bottom) {
    headerHeight = top;
    bottomHeight = bottom;
    leftBlank = left;
    rightBlank = right;
}

void QTableViewPrinter::setPageHeader(PageHeader *pageheader)
{
    pageHeader=pageheader;
}

void QTableViewPrinter::setPagerFooter(PageFooter *pagefooter)
{
    pageFooter = pagefooter;
}

void QTableViewPrinter::setPagerTitle(PageTitle *pagetitle)
{
    pageTitle = pagetitle;
}

void QTableViewPrinter::setPen(QPen p) {
    pen = p;
}

void QTableViewPrinter::setHeadersFont(QFont f) {
    headersFont = f;
}

void QTableViewPrinter::setContentFont(QFont f) {
    contentFont = f;
}

void PageHeader::setPageHeaderFont(QFont font)
{
    pageHeaderFont=font;
}

void PageFooter::setViewFlag(bool zbr, bool ym, bool zbsj)
{
    mzbr = zbr;
    mym = ym;
    mzbsj = zbsj;
}

void QTableViewPrinter::setTextWordWrap(Qt::TextFlag Wrap)
{
    textLineFlag=Wrap;
}

void QTableViewPrinter::setHeaderColor(QColor color) {
    headerColor = color;
}

void QTableViewPrinter::setContentColor(QColor color) {
    contentColor = color;
}

void QTableViewPrinter::setMaxRowHeight(int height) {
    maxRowHeight = height;
}

void QTableViewPrinter::setTextAlign(Qt::AlignmentFlag align)
{
    textAlign=align;
}

void QTableViewPrinter::setTitleFlag(const TitleFlag type)
{
    titleFlag = type;
}

void QTableViewPrinter::setHeaderFlag(bool flag)
{
    mheaderFlag = flag;
}

void QTableViewPrinter::position(const QString &str)
{
    qDebug()<<str<<" x:"<<painter->transform().dx()<<" y:"<<painter->transform().dy();
}

bool QTableViewPrinter::setPrintColumnList(const QStringList &list)
{
    if(list.size()==0 || mHeaderTitles.size()==0 || model->columnCount()==0)
        return false;
    mColumnList.clear();
    //Asigne un valor al número de línea primero
    if(!list.contains("No.")){
        PrintColumn *item= new PrintColumn();
        item->name="No.";
        item->modelColumn=-1;
        item->columnWidth=50;
        mColumnList.push_back(item);
    }
    for(int i=0;i<model->columnCount();++i){
        if(list.contains(mHeaderTitles.at(i))){
            PrintColumn *item= new PrintColumn();
            item->name= mHeaderTitles.at(i);
            item->modelColumn = i;
            item->columnWidth = mTableView->columnWidth(i)*1.2*contentFont.pointSize()/12+leftMargin+rightMargin;
            if(item->name.contains("|")){//Encabezado complejo
                QString temp=item->name;
                QStringList tempList=temp.split("|");
                mergeColumn* merge;
                for(int j=0;j<tempList.size();++j){
                    if(_mergeColumnHash[j].size()>0){
                        if(_mergeColumnHash[j].last()->text!=tempList.at(j)){
                            merge = new mergeColumn();
                            merge->text = tempList.at(j);
                            merge->columnList.push_back(QString::number(i));
                            _mergeColumnHash[j].push_back(merge);
                        }else{//Columna a fusionar
                            if(j==0){
                                merge = _mergeColumnHash[j].last();
                                merge->columnList.push_back(QString::number(i));
                            }else{
                                if(_mergeColumnHash[j-1].last()->columnList.contains(QString::number(i)) && _mergeColumnHash[j-1].last()->columnList.contains(_mergeColumnHash[j].last()->columnList.last())){
                                    merge = _mergeColumnHash[j].last();
                                    merge->columnList.push_back(QString::number(i));
                                }else{
                                    merge = new mergeColumn();
                                    merge->text = tempList.at(j);
                                    merge->columnList.push_back(QString::number(i));
                                    _mergeColumnHash[j].push_back(merge);
                                }
                            }
                        }
                    }else{
                        merge = new mergeColumn();
                        merge->text = tempList.at(j);
                        merge->columnList.push_back(QString::number(i));
                        _mergeColumnHash[j].push_back(merge);
                    }
                }
            }
            mColumnList.push_back(item);
        }
    }
    return true;
}

bool QTableViewPrinter::paintTable(int row, int column, bool headFlag)
{
    //Obtenga el número total de filas y columnas
    int rowCount = model->rowCount();
    int columnCount=mColumnList.size();
    //Ancho de interfaz, altura
    int viewWidth = painter->viewport().width();
    int viewHeight = painter->viewport().height();
    //Ancho de la tabla, altura
    int tableWidth = viewWidth - leftBlank - rightBlank;
    int tableHeight = viewHeight - headerHeight - bottomHeight;
    //La última fila dibujada, una columna
    int lastRow =0 ,lastColumn = 0;
    //El ancho total real de la página actual y el ancho de cada columna
    int realTableWidth = 0;
    bool ischild=false;
    QList<int> columnWidth;
    for(int i=column;i<columnCount;++i){
        //Este calculo es una estimación
        int width=mColumnList.at(i)->columnWidth;
        if(realTableWidth+width > tableWidth){
            lastColumn = i;
            break;
        }
        columnWidth.push_back(width);
        realTableWidth += width;
    }
    //El número total de columnas en la página de dibujo actual
    int curColumnCount = columnWidth.size();
    //Si dibujar el encabezado de la tabla
    bool curheadflag=headFlag;
    //Establecer origen
    painter->translate(-painter->transform().dx(),-painter->transform().dy()+headerHeight);//(0,60)
    painter->save();//(0,60)

    //Dibujar encabezado
    if(pageHeader){
        painter->save();
        pageHeader->startDrawHeader();
        painter->restore();
    }
    //Dibujar Título
    if(pageTitle){
        painter->save();
        pageTitle->startDrawTitle();
        painter->restore();
    }
    //Dibujar pie de página
    if(pageFooter) {
        painter->save();
        painter->translate(-painter->transform().dx(), -painter->transform().dy());
        pageFooter->startDrawFooter();
        painter->restore();
    }

    //Coloque el vértice izquierdo de la tabla.
    painter->translate(-painter->transform().dx() + leftBlank, 0);
    painter->save();//(40,60)

    painter->setPen(pen);
    //Línea superior horizontal
    painter->drawLine(0, 0, realTableWidth, 0);

    //Primero use un pintor de prueba para obtener la altura máxima de la fila
    QPainter testSize;
    QImage* image = new QImage(10, 10, QImage::Format_RGB32);
    image->setDotsPerMeterX(printer->logicalDpiX() * 100 / 2.54); // 2.54 cm = 1 inch
    image->setDotsPerMeterY(printer->logicalDpiY() * 100 / 2.54);
    testSize.begin(image);

    //Obtener altura de fila
    int i;
    if(curheadflag)
        i = row-1;
    else
        i = row;
    testSize.setFont(headersFont);
    painter->setFont(headersFont);
    int currentPageRowCount=0;
    QHash<int, int> headrowheight;
    for(i; i<rowCount; ++i){
        int maxHeight = 0;
        for(int j=0;j<columnCount;++j){
            if(i==row){
                testSize.setFont(contentFont);
                painter->setFont(contentFont);
            }

            QString str;
            if(i<row){
                str = mColumnList.at(j)->name;
            }else{
                if(mColumnList.at(j)->modelColumn==-1){//行号
                    str = QString::number(i+1);
                }else
                    str = model->data(model->index(i,mColumnList.at(j)->modelColumn), Qt::DisplayRole).toString();
            }

            if(str.contains("|")){
                int height=0;
                QStringList list=str.split("|");
                for(int k=0;k<list.size();++k){
                    QRect rect(0, 0,  mColumnList.at(j)->columnWidth- rightMargin - leftMargin, maxRowHeight);
                    QRect realRect;
                    testSize.drawText(rect, textAlign | textLineFlag, list.at(k), &realRect);
                    if(headrowheight[k]<realRect.height())
                        headrowheight[k]=realRect.height();
                    height += realRect.height()+bottomMargin;
                }
                if(height > maxHeight && mColumnList.at(j)->columnWidth != 0){
                    height > maxRowHeight ? maxHeight = maxRowHeight : maxHeight = height;
                }
            }else{
                QRect rect(0, 0,  mColumnList.at(j)->columnWidth- rightMargin - leftMargin, maxRowHeight);
                QRect realRect;
                testSize.drawText(rect, textAlign | textLineFlag, str, &realRect);
                if (realRect.height() > maxHeight && mColumnList.at(j)->columnWidth != 0) {
                    realRect.height() > maxRowHeight ? maxHeight = maxRowHeight : maxHeight = realRect.height();
                }
            }
        }
        //Altura de la cabeza de la mesa**
        if(mheaderRowHeight==0){
            mheaderRowHeight = maxHeight;
        }
        //Determinar si agregar una página
        double curHeight=painter->transform().dy() + maxHeight + topMargin + bottomMargin;
        double topHeight=painter->viewport().height() -bottomHeight;
        if(curHeight > topHeight){
            lastRow = i;
            //-------Dibuja una línea vertical en la página actual----------
            int y = painter->transform().dy();
            painter->restore();//(40,60)
            painter->save();//(40,60)
            for(int j = column; j < column+curColumnCount; j++) {
                if(j==column){
                    painter->translate(0, -painter->transform().dy()+headerHeight);
                }else{
                    painter->translate(0,-painter->transform().dy()+headerHeight+mheaderRowHeight+topMargin+bottomMargin);
                }
                painter->drawLine(0, 0, 0, - painter->transform().dy() + y);
                painter->translate(mColumnList.at(j)->columnWidth, 0);
            }
            painter->translate(0, -painter->transform().dy()+headerHeight);
            painter->drawLine(0, 0, 0, - painter->transform().dy() + y);
            painter->restore();//(40,60)
            //------Terminar de dibujar la línea vertical de la página actual---------
            //Nueva subpágina
            printer->newPage();
            paintTable(lastRow,column,mheaderFlag);
            painter->save();//(40,60)
            ischild=true;
            break;
        }
        //-------------Comience a dibujar datos tabulares----------------------
        painter->save(); //(40,60)
        i < row ? painter->setPen(QPen(headerColor)) : painter->setPen(QPen(contentColor));
        curheadflag ? painter->setFont(headersFont) : painter->setFont(contentFont);
        int firstc = mColumnList.at(column)->modelColumn;
        int lastc = mColumnList.at(column+curColumnCount-1)->modelColumn;
        //Trazar datos
        for(int j = column; j < column+curColumnCount; j++) {
            QString str;
            if(curheadflag) {
                str = mColumnList.at(j)->name;
            } else {
                if(mColumnList.at(j)->modelColumn==-1){
                    str = QString::number(i+1);
                }else
                    str = model->data(model->index(i,mColumnList.at(j)->modelColumn), Qt::DisplayRole).toString();
            }

            if(str.contains("|")){
                QStringList templist = str.split("|");
                int realHeight=0;
                for(int a=0;a<templist.size();++a){
                    QString temp = templist.at(a);
                    //Obtener altura de fila
                    realHeight = headrowheight[a];
                    //Determine si necesita fusionar filas
                    if(templist.size()<headrowheight.size() && a==templist.size()-1){
                        for(int b=a+1;b<headrowheight.size();++b){
                            realHeight += (headrowheight[b]+topMargin+bottomMargin);
                        }
                    }
                    //Obtenga el valor del contenedor y descubra si la columna actual existe ------
                    //Columna actual
                    int curc = mColumnList.at(j)->modelColumn;
                    int width=0;
                    //Encuentra la columna actual, calcula el ancho de la columna
                    for(int b=0;b<_mergeColumnHash[a].size();++b){
                        //Toma la columna del contenedor
                        mergeColumn* item = _mergeColumnHash[a].at(b);
                        //Determine si la columna actual existe en la columna del contenedor
                        if(item->columnList.contains(QString::number(curc))){
                            //Ciclo de la columna del contenedor, encontrar la posición de la columna, obtener el ancho de la columna
                            if(item->columnList.at(0).toInt()==curc || curc==firstc){
                                int d=0;
                                for(int c=0;c<item->columnList.size();++c){
                                    //El valor de la primera columna de la columna del contenedor debe ser igual a la columna actual o la columna del contenedor o el valor de la primera columna de la página actual está en el medio de la columna del contenedor
                                    if(item->columnList.at(c).toInt()<=lastc && item->columnList.at(c).toInt()>=curc){
                                        width += mColumnList.at(j+d)->columnWidth;
                                        d++;
                                    }
                                }
                            }
                            break;
                        }
                    }
                    //Determinar el ancho de columna
                    if(width==0){//Se ha dibujado el título de la columna actual, no es necesario dibujar
                        if(a==templist.size()-1){//换列
                            painter->translate(mColumnList.at(j)->columnWidth, -painter->transform().dy()+headerHeight);
                        }else{//Sin salto de línea
                            painter->translate(0, realHeight+bottomMargin+topMargin);
                        }
                    }else{//Listo para dibujar
                        QRect rec(leftMargin, topMargin, width - rightMargin - leftMargin, realHeight);
                        painter->drawText(rec, textAlign | textLineFlag, temp);
                        //Dibujar lineas horizontales
                        if(a!=templist.size()-1){//No es necesario dibujar la última línea.
                            painter->drawLine(0,realHeight+bottomMargin+topMargin,width,realHeight+bottomMargin+topMargin);
                        }
                        //Dibujar linea vertical
                        painter->drawLine(width,0,width,realHeight+bottomMargin+topMargin);

                        if(a==templist.size()-1){//Cambiar columna
                            painter->translate(mColumnList.at(j)->columnWidth, -painter->transform().dy()+headerHeight);
                        }else{//Envolver
                            painter->translate(0, realHeight+bottomMargin+topMargin);
                        }
                    }
                }
            }else{
                QRect rec(leftMargin, topMargin, mColumnList.at(j)->columnWidth - rightMargin - leftMargin, maxHeight);
                painter->drawText(rec, textAlign | textLineFlag, str);
                //Dibujar linea vertical
                painter->drawLine(mColumnList.at(j)->columnWidth,0,mColumnList.at(j)->columnWidth,maxHeight+bottomMargin+topMargin);
                painter->translate(mColumnList.at(j)->columnWidth, 0);
            }
        }
        if(curheadflag){
            curheadflag = false;
        }
        painter->restore(); //(40,60)
        //--------------Terminar de dibujar datos tabulares-----------------------
        //Dibujar línea actual línea horizontal
        painter->drawLine(0, maxHeight + topMargin + bottomMargin, realTableWidth,maxHeight + topMargin + bottomMargin);
        //Actualizar coordenadas
        painter->translate(0, maxHeight + topMargin + bottomMargin);
        currentPageRowCount++;
    }
    //-----------Dibujar linea vertical--------------------
    int y = painter->transform().dy();
    painter->restore();//(40,60)
    if(!ischild){ //La línea vertical se ha dibujado en la subpágina, no es necesario volver a dibujar
        for(int i = column; i < column+curColumnCount; i++) {
            if(i==column){
                painter->translate(0, -painter->transform().dy()+headerHeight);
            }else{
                painter->translate(0,-painter->transform().dy()+headerHeight+mheaderRowHeight+topMargin+bottomMargin);
            }
            painter->drawLine(0, 0, 0,- painter->transform().dy() + y);
            painter->translate(mColumnList.at(i)->columnWidth, 0);
        }
        painter->translate(0, -painter->transform().dy()+headerHeight);
        painter->drawLine(0, 0, 0, - painter->transform().dy() + y);
    }
    //-----------Se dibuja la línea vertical----------------
    testSize.end();
    delete image;

    //Restaurar origen(0,60)
    painter->restore();

    //Paginación (solo los datos en la primera página permiten la paginación)
    if(column+curColumnCount<columnCount && row==0){
        printer->newPage();
        paintTable(row,lastColumn,mheaderFlag);
    }
    return true;
}
//--------Fin del contenido principal-------------

//--------Clase de encabezado----------------
PageHeader::PageHeader(QPainter *painter)
{
    mpainter = painter;
    pageHeaderFont = QFont("Microsoft YaHei",9);
}

void PageHeader::startDrawHeader()
{

}
//--------Fin de la clase de encabezado-------------


//--------Categoría de pie de página-------------
int PageFooter::pageNumber=1;

PageFooter::PageFooter(QPainter *painter)
{
    mpainter = painter;
    creater = "No";
    pageFooterFont = QFont("Times",9);
    mzbr = true;
    mzbsj = true;
    mym = true;
}

void PageFooter::setPageFooterFont(QFont font)
{
    pageFooterFont=font;
}

void PageFooter::resetPageNumber()
{
    pageNumber=1;
}

void PageFooter::startDrawFooter()
{
    //Guardar fuente actual
    QFont font=mpainter->font();
    //Establecer fuente de pie de página
    mpainter->setFont(pageFooterFont);
    //Dibuja al reloj
    if(mzbr){
        mpainter->translate(-mpainter->transform().dx()+40, -mpainter->transform().dy()+mpainter->viewport().height() - 20);
        mpainter->drawText(0, 0, QString("%1").arg(creater));
    }
    //Dibujar número de página
    if(mym){
        mpainter->translate(-mpainter->transform().dx()+mpainter->viewport().width()/2-15, -mpainter->transform().dy()+mpainter->viewport().height() - 20);
        mpainter->drawText(0, 0, QString("Pag. %1 ").arg(pageNumber));
        pageNumber += 1;
    }
    //Dibujar fecha de tabulación
    if(mzbsj){
        mpainter->translate(-mpainter->transform().dx()+mpainter->viewport().width()-170, -mpainter->transform().dy()+mpainter->viewport().height() - 20);
        mpainter->drawText(0, 0, QString("Date: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")));
    }
    //Restaurar la fuente anterior
    mpainter->setFont(font);
}

void PageFooter::setCreater(const QString &name)
{
    creater=name;
}

//--------Fin de la clase de pie de página-------------

//--------clase título-------------
PageTitle::PageTitle(QPainter *painter, QPrinter* printer)
{
    mpainter=painter;
    mprinter = printer;
    QFont font = QFont("Times",25);
    QFont font2 = QFont("Times",20);
    QFont sidefont = QFont("Times",15);
    font.setBold(true);
    titleFont=font;
    title2Font=font2;
    sidetitleFont = sidefont;
}

void PageTitle::setTitleFont(QFont font)
{
    titleFont=font;
}

void PageTitle::setSideTitleFont(QFont font)
{
    sidetitleFont=font;
}

void PageTitle::setPageTitle(const QString &title)
{
    mtitle=title;
}

void PageTitle::setSideTitle(const QStringList &sidetitle)
{
    msideTitles = sidetitle;
}

void PageTitle::startDrawTitle()
{
    //de vuelta al principio
    mpainter->translate(-mpainter->transform().dx(),-mpainter->transform().dy());
    //Establecer fuente
    QFont font=mpainter->font();
    mpainter->setFont(titleFont);
    //Obtener el ancho del título
    QPainter testSize;
    QImage* image = new QImage(10, 10, QImage::Format_RGB32);
    image->setDotsPerMeterX(mprinter->logicalDpiX() * 100 / 2.54); // 2.54 cm = 1 inch
    image->setDotsPerMeterY(mprinter->logicalDpiY() * 100 / 2.54);
    testSize.begin(image);
    testSize.setFont(titleFont);
    QRect rect(0, 0,  1000, 1000);
    QRect realRect;
    testSize.drawText(rect, Qt::AlignmentFlag::AlignCenter | Qt::TextFlag::TextSingleLine, mtitle, &realRect);
    int left= (mpainter->viewport().width()-realRect.width())*0.5;
    //Título del sorteo
    //mpainter->drawText(left,QTableViewPrinter::headerHeight/2+realRect.height()/2,mtitle);
    mpainter->drawText(left,QTableViewPrinter::headerHeight-(QTableViewPrinter::headerHeight - realRect.height()),mtitle);

    //Titulo del Reporte
    mpainter->setFont(title2Font);
    mpainter->drawText(left,QTableViewPrinter::headerHeight-(QTableViewPrinter::headerHeight - (realRect.height()*1.8)),mtitle2);//line of code added

    //añadir imagen del logo
    QImage img("://Data/Icon.png");
    QImage img2 = img.scaled(100, 100);
    mpainter->drawImage(50,25,img2,0,0,100,100);


    //Dibuje el subtítulo (uniformemente sobre la tabla, debajo del título)
    if(msideTitles.size()>0){
        mpainter->setFont(sidetitleFont);
        testSize.setFont(sidetitleFont);
        QList<int> widthList;
        int totalwidth=0;
        int height =0;
        for(int i=0;i<msideTitles.size();++i){
            testSize.drawText(rect, Qt::AlignmentFlag::AlignCenter | Qt::TextFlag::TextWordWrap, msideTitles.at(i), &realRect);
            widthList.push_back(realRect.width());
            totalwidth+=realRect.width();
            if(i==0)
                height = realRect.height();
        }
        int midWidth = 0;
        if(msideTitles.size()>1)
            midWidth=(mpainter->viewport().width()-(QTableViewPrinter::leftBlank+QTableViewPrinter::rightBlank+totalwidth))/(msideTitles.size()-1);
        int tempWidth=left;
        int sideTiY=150;
        for(int i=0;i<msideTitles.size();++i){
            /*for(int j=0;j<i;++j){
                tempWidth+=widthList.at(j)+midWidth;
                tempWidth+=0;
            }*/
            //mpainter->drawText(QTableViewPrinter::leftBlank+tempWidth,QTableViewPrinter::headerHeight-height/4,msideTitles.at(i));
            mpainter->drawText(tempWidth,sideTiY,msideTitles.at(i));
            sideTiY-=height;
        }
    }
    mpainter->setFont(font);
}

QFont PageTitle::getTitle2Font() const
{
    return title2Font;
}

void PageTitle::setTitle2Font(const QFont &value)
{
    title2Font = value;
}

QString PageTitle::getMtitle2() const
{
    return mtitle2;
}

void PageTitle::setMtitle2(const QString &value)
{
    mtitle2 = value;
}
//--------Fin de la clase de título-------------
