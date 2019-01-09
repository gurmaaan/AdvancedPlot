#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bulkDialog_ = new BulkDialog;
    aW_ = ui->fileA_widget;
    bW_ = ui->fileB_widget;
    cW_ = ui->fileC_widget;

    plot_ = ui->plot_view;
    setupWidgets();
    connectAll();
    showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::runCgen(bool status)
{
    bool allCompleted = status && aW_->parsingState() && bW_->parsingState();

    cW_->setEnabled(allCompleted);
    ui->plot_dock->setEnabled(allCompleted);

    int curInd = 0;
    if( aW_->parsingState() && (!bW_->parsingState()) )
    {
        curInd = 0;
        ui->tabWidget->setTabText( curInd, aW_->fileName() );
    }
    else if(bW_->parsingState() && (!aW_->parsingState()) )
    {
        curInd = 1;
        ui->tabWidget->setTabText( curInd, bW_->fileName() );
    }
    else if(allCompleted)
    {
        curInd = 2;
        //BUG
//        ui->tabWidget->setTabText( curInd, cW_->fileName() );
    }
    ui->tabWidget->setCurrentIndex(curInd);

    if(allCompleted)
    {
        bulkDialog_->show();
        bulkDialog_->processFiles(aW_->csv(), bW_->csv());
    }
}

void MainWindow::connectAll()
{
    connect(aW_, &FileWidget::modelChanged,
            ui->fileA_tv, &QTableView::setModel);
    connect(bW_, &FileWidget::modelChanged,
            ui->fileB_tv, &QTableView::setModel);
    connect(cW_, &FileWidget::modelChanged,
            ui->fileC_tv, &QTableView::setModel);

    connect(cW_, &FileWidget::modelChanged,
            this, &MainWindow::setAxisModel);

    connect(ui->xAxis, &AxisWidget::currentIndexChanged,
            this, &MainWindow::scrollAndSelect);
    connect(ui->yAxis, &AxisWidget::currentIndexChanged,
            this, &MainWindow::scrollAndSelect);
    connect(ui->dxAxis, &AxisWidget::currentIndexChanged,
            this, &MainWindow::scrollAndSelect);
    connect(ui->dyAxis, &AxisWidget::currentIndexChanged,
            this, &MainWindow::scrollAndSelect);

    connect(bulkDialog_, &BulkDialog::fileCompleted,
            cW_, &FileWidget::receiveCSV);

    connect(aW_, &FileWidget::parsingDone,
            this, &MainWindow::runCgen);
    connect(bW_, &FileWidget::parsingDone,
            this, &MainWindow::runCgen);
}

void MainWindow::setupWidgets()
{
    aW_->setTitle("Файл А");
    bW_->setTitle("Файл В");
    cW_->setTitle("Файл С");
    cW_->setEnabled(false);
    cW_->setBtnVisible(false);

    //NOTE : включение / отключение дебага
    ui->debug_btn->setVisible(true);

    ui->xAxis->setT(AxisType::XAxis);
    ui->xAxis->setVisibleOnPlotBtnVisible(false);
    ui->yAxis->setT(AxisType::YAxis);
    ui->yAxis->setVisibleOnPlotBtnVisible(false);

    ui->dxAxis->setT(AxisType::dXAxis);
    ui->dxAxis->setColor(QColor(Qt::black));
    ui->dyAxis->setT(AxisType::dYAxis);
    ui->dyAxis->setColor(QColor(Qt::black));

    ui->pointsAClr->setColor(QColor(Qt::red));
    ui->pointsAClr->setText("Цвет точек файла А:");
    ui->pointsBClr->setColor(QColor(Qt::darkGreen));
    ui->pointsBClr->setText("Цвет точек файла В:");
    ui->splitClr->setColor(QColor(Qt::blue));
}

QVector<double> MainWindow::getPlotData(QStandardItemModel *model, int colIndex, QString suffix)
{
    QVector<double> vals;
    for(int i = 0; i < model->rowCount(); i++)
    {
        QString objName = model->headerData(i, Qt::Vertical, Qt::DisplayRole).toString();
        if(objName.right(1) == suffix)
        {
            QModelIndex index = model->index(i, colIndex);
            vals << index.data(Qt::UserRole).toDouble();
        }
    }
    return vals;
}

double MainWindow::calcDelta(int newAngle, double y11)
{
    //Треугольник A(0;0), B(x11;y11), C(x21;y21). Ищем x21 = x11 - BC. Возвращаем BC.
    double ctg1 = 1.0 / qTan( qDegreesToRadians( static_cast<double>(oldAngle()) ) );
    double ctg2 = 1.0 / qTan( qDegreesToRadians( static_cast<double>(newAngle  ) ) );
    double delta = y11*(ctg1 - ctg2);
    return delta;
}

double MainWindow::oldAngle() const
{
    return oldAngle_;
}

void MainWindow::setOldAngle(double oldAngle)
{
    oldAngle_ = oldAngle;
}

QString MainWindow::kStr(AxisWidget *xA, AxisWidget *yA)
{
    double x1,x2,y1,y2;
    x1 = xA->max();
    x2 = xA->min();
    y1 = yA->min();
    y2 = yA->max();

    return kStr(x1,x2,y1,y2);
}

QString MainWindow::kStr(double x1, double x2, double y1, double y2)
{
    double ax, by;
    ax = x2 - x1;
    by = y2 - y1;
    QString kString = QString::number(ax/by);
    return kString;
}

QString MainWindow::bStr(AxisWidget *xA, AxisWidget *yA)
{
    double x1,x2,y1,y2;
    x1 = xA->max();
    x2 = xA->min();
    y1 = yA->min();
    y2 = yA->max();

    return bStr(x1,x2,y1,y2);
}

QString MainWindow::bStr(double x1, double x2, double y1, double y2)
{
    double ax, by;
    ax = x2 - x1;
    by = y2 - y1;
    double blyat = ( y2 * (ax/by) ) - ( x2 * (ax/by) );
    QString bString = QString::number(blyat);
    return bString;
}

void MainWindow::setLineEq(AxisWidget *xA, AxisWidget *yA)
{
    ui->lineEq_k->setText(kStr(xA, yA));
    ui->lineEq_b->setText(bStr(xA, yA));
}

void MainWindow::setLineEq(double x1, double x2, double y1, double y2)
{
    ui->lineEq_k->setText(kStr(x1, x2, y1, y2));
    ui->lineEq_b->setText(bStr(x1, x2, y1, y2));
}

void MainWindow::addError(QCustomPlot *plotVie, AxisWidget *axis)
{
    QCPErrorBars *errorBar = new QCPErrorBars(plotVie->xAxis, plotVie->yAxis);
    QCPErrorBars::ErrorType et = ( axis == ui->dxAxis) ? QCPErrorBars::ErrorType::etKeyError
                                                       : QCPErrorBars::ErrorType::etValueError;
    errorBar->setErrorType(et);
    errorBar->removeFromLegend();
    errorBar->setAntialiased(false);
    errorBar->setDataPlottable( plotVie->graph(plotVie->graphCount() - 1) );
    errorBar->setPen(QPen(axis->color()));
    errorBar->setData(axis->values());
}

void MainWindow::addPoints(QCustomPlot *plotVie, QVector<double> datax, QVector<double> datay, QColor clr, int size, QString name)
{
    plotVie->addGraph();
    int gn = plotVie->graphCount() - 1;
    plotVie->graph(gn)->setPen(QPen(clr));
    plotVie->graph(gn)->setLineStyle(QCPGraph::lsNone);
    plotVie->graph(gn)->setScatterStyle( QCPScatterStyle(QCPScatterStyle::ssDisc, size) );
    plotVie->graph(gn)->setData(datax, datay);
    plotVie->graph(gn)->setName(name);
}

void MainWindow::setupAxis(QCustomPlot *plotVie, AxisWidget *axis)
{
    QCPAxis *ax = (axis == ui->xAxis) ? plotVie->xAxis
                                      : plotVie->yAxis;
    ax->setLabel(axis->name());
    ax->setRange( axis->min(), axis->max());
}

void MainWindow::setupDelta(QSlider *slider, AxisWidget *axis)
{
    int minV = static_cast<int>(axis->min() * 100.0);
    int maxV = static_cast<int>(axis->max() * 100.0);
    int curV = static_cast<int>(axis->av()  * 100.0);

    slider->setRange(minV, maxV);
    slider->setValue(curV);
}

void MainWindow::scrollAndSelect(int colNum)
{
    QAbstractItemModel *cM = ui->fileC_tv->model();
    ui->fileC_tv->scrollTo(cM->index(0,colNum));
    ui->fileC_tv->selectColumn(colNum);
}

void MainWindow::setAxisModel(QStandardItemModel *m)
{
    ui->xAxis->setModel(m);
    ui->yAxis->setModel(m);
    ui->dxAxis->setModel(m);
    ui->dyAxis->setModel(m);

    QVector<double> xAVals = getPlotData(m, 0, "A");
    QVector<double> xBVals = getPlotData(m, 0, "B");

    ui->pointsAAbsCnt_sb->setMaximum(m->rowCount());
    ui->pointsBAbsCnt_sb->setMaximum(m->rowCount());
    ui->pointsAAbsCnt_sb->setValue(xAVals.count());
    ui->pointsBAbsCnt_sb->setValue(xBVals.count());

    double total = static_cast<double>(m->rowCount());
    double aC = static_cast<double>(xAVals.count());
    double bC = static_cast<double>(xBVals.count());
    ui->pointsACntPer_sb->setValue((aC / total) * 100.0);
    ui->pointsBCntPer_sb->setValue((bC / total) * 100.0);
}

void MainWindow::on_build_btn_clicked()
{
    //Активирование таба с графиком + установка в false линию разделения
    ui->tabWidget->setCurrentIndex(3);
    ui->split_gb->setChecked(false);

    //Очистка графика
    plot_->clearGraphs();

    //Настройка легенды и осей
    plot_->legend->setVisible(true);
    plot_->legend->setFont(QFont("Helvetica", 8));
    setupAxis(plot_, ui->xAxis);
    setupAxis(plot_, ui->yAxis);

    //Сосотавление уравнения прямой по умолчанию
    setLineEq(ui->xAxis, ui->yAxis);

    //Настройка угла прямой разделения
    double tg = ui->yAxis->max() / ui->xAxis->min() ;
    int alphaAngle = static_cast<int>( qRadiansToDegrees( qAtan(tg) ) ) ;
    setOldAngle(qRadiansToDegrees( qAtan(tg) ));
    ui->angle_dial->setValue(alphaAngle);
    ui->angle_sb->setValue(alphaAngle);

    //Настройка смещений
    setupDelta(ui->hDelta_H_sldr, ui->xAxis);
    setupDelta(ui->vDelta_H_sldr, ui->yAxis);

    //Добавление основных точек
    QStandardItemModel *cModel = cW_->csv().model();
    QVector<double> xAdata = getPlotData(cModel, ui->xAxis->curentIndex(), "A");
    QVector<double> yAdata = getPlotData(cModel, ui->yAxis->curentIndex(), "A");
    QVector<double> xBdata = getPlotData(cModel, ui->xAxis->curentIndex(), "B");
    QVector<double> yBdata = getPlotData(cModel, ui->yAxis->curentIndex(), "B");

    QString aName = DEPENDANCE + ui->yAxis->name() + DEPOF + ui->xAxis->name() + " для файла А";
    QString bName = DEPENDANCE + ui->yAxis->name() + DEPOF + ui->xAxis->name() + " для файла B";
    int pSize = ui->pointSize_sb->value();

    if( (xAdata.count() != 0) && (yAdata.count() != 0))
        addPoints(plot_, xAdata, yAdata, ui->pointsAClr->color(), pSize, aName);
    if( (xBdata.count() != 0) && (yBdata.count() != 0))
        addPoints(plot_, xBdata, yBdata, ui->pointsBClr->color(), pSize, bName);

    plot_->graph(0)->rescaleAxes(true);

    //Добавление ошибок
    if(ui->dxAxis->isVisibleOnPlot())
        addError(plot_, ui->dxAxis);

    if(ui->dyAxis->isVisibleOnPlot())
        addError(plot_, ui->dyAxis);

    //Обновление графика
    plot_->axisRect()->setupFullAxesBox();
    plot_->replot();
}

void MainWindow::on_split_gb_clicked(bool checked)
{
    // Прямая через 1(xmin, ymax) - 2(xmax, ymin)
    if(checked)
    {
        plot_->addGraph();

        double xmax, xmin, ymax, ymin;
        xmax = ui->xAxis->max();
        ymin = ui->yAxis->min();
        xmin = ui->xAxis->min();
        ymax = ui->yAxis->max();

        QVector<double> xValues, yValues;
        xValues << xmin << xmax;
        yValues << ymax << ymin;

        int grNum = (plot_->graphCount() > 1) ? plot_->graphCount() - 1 : 0;

        plot_->graph(grNum)->setPen(QPen(ui->splitClr->color(), ui->splitSize_sb->value()));
        plot_->graph(grNum)->setLineStyle(QCPGraph::lsLine);
        plot_->graph(grNum)->setData(xValues, yValues);
        plot_->graph(grNum)->setName(SPLITNAME);
    } else
    {
        plot_->removeGraph(plot_->graphCount() - 1);
    }
    plot_->replot();
}

void MainWindow::on_angle_dial_sliderMoved(int position)
{
    // Прямая через 1(xmin, ymax) - 2(xmax, ymin)
    int grNum = plot_->graphCount() - 1;
    plot_->removeGraph(grNum);

    double xmax, xmin, ymax, ymin;

    xmin = ui->xAxis->min() + calcDelta(position, ui->yAxis->max());
    ymax = ui->yAxis->max();
    ymin = ui->yAxis->min();
    xmax = ui->xAxis->max() - calcDelta(position, ui->yAxis->max());

    QVector<double> xValues, yValues;
    xValues << xmin << xmax;
    yValues << ymax << ymin;

    setLineEq(xmax, xmin, ymin, ymax);

    plot_->addGraph();
    plot_->graph(grNum)->setPen(QPen(ui->splitClr->color(), ui->splitSize_sb->value()));
    plot_->graph(grNum)->setLineStyle(QCPGraph::lsLine);
    plot_->graph(grNum)->setData(xValues, yValues);
    plot_->graph(grNum)->setName(SPLITNAME);

    plot_->replot();
}

void MainWindow::on_actionDebug_triggered()
{
    aW_->processFile(A_FILE_PATH);
    bW_->processFile(B_FILE_PATH);
    QTimer::singleShot(2500, this, SLOT( setComboBoxes() ));
}

void MainWindow::setComboBoxes()
{
    ui->xAxis->setCurInd(1);
    ui->yAxis->setCurInd(32);
    ui->dxAxis->setCurInd(0);
    ui->dyAxis->setCurInd(31);

    ui->pointSize_sb->setValue(3);
}

void MainWindow::on_hDelta_H_sldr_sliderMoved(int position)
{
    int grNum = plot_->graphCount() - 1;
    plot_->removeGraph(grNum);

    double xmax, xmin, ymax, ymin;

    xmin = ui->xAxis->min() + calcDelta(ui->angle_dial->value(), ui->yAxis->max());
    xmin = xmin + static_cast<double>(position / 100);
    ymax = ui->yAxis->max();
    ymin = ui->yAxis->min();
    xmax = ui->xAxis->max() - calcDelta(ui->angle_dial->value() , ui->yAxis->max());
    xmax = xmax + static_cast<double>(position / 100);

    QVector<double> xValues, yValues;
    xValues << xmin << xmax;
    yValues << ymax << ymin;

    setLineEq(xmax, xmin, ymin, ymax);

    plot_->addGraph();
    plot_->graph(grNum)->setPen(QPen(ui->splitClr->color(), ui->splitSize_sb->value()));
    plot_->graph(grNum)->setLineStyle(QCPGraph::lsLine);
    plot_->graph(grNum)->setData(xValues, yValues);
    plot_->graph(grNum)->setName(SPLITNAME);

    plot_->replot();
}

void MainWindow::on_vDelta_H_sldr_sliderMoved(int position)
{
    int grNum = plot_->graphCount() - 1;
    plot_->removeGraph(grNum);

    double xmax, xmin, ymax, ymin;

    xmin = ui->xAxis->min() + calcDelta(ui->angle_dial->value(), ui->yAxis->max());
    ymax = ui->yAxis->max();
    ymax = ymax + static_cast<double>(position / 100);
    ymin = ui->yAxis->min();
    ymin = ymin + static_cast<double>(position / 100);
    xmax = ui->xAxis->max() - calcDelta(ui->angle_dial->value(), ui->yAxis->max());

    QVector<double> xValues, yValues;
    xValues << xmin << xmax;
    yValues << ymax << ymin;

    setLineEq(xmax, xmin, ymin, ymax);

    plot_->addGraph();
    plot_->graph(grNum)->setPen(QPen(ui->splitClr->color(), ui->splitSize_sb->value()));
    plot_->graph(grNum)->setLineStyle(QCPGraph::lsLine);
    plot_->graph(grNum)->setData(xValues, yValues);
    plot_->graph(grNum)->setName(SPLITNAME);

    plot_->replot();
}
