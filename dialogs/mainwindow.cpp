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

    plot = ui->plot_view;
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

    ui->xAxis->setT(AxisType::XAxis);
    ui->yAxis->setT(AxisType::YAxis);
    ui->dxAxis->setT(AxisType::dXAxis);
    ui->dxAxis->setColor(QColor(Qt::black));
    ui->dyAxis->setT(AxisType::dYAxis);
    ui->dyAxis->setColor(QColor(Qt::black));
    ui->pointClr->setColor(QColor(Qt::red));
    ui->splitClr->setColor(QColor(Qt::blue));
}

double MainWindow::calcDelta(int newAngle, double y11)
{
    //Треугольник A(0;0), B(x11;y11), C(x21;y21). Ищем x21 = x11 - BC. Возвращаем BC.
    double ctg1 = 1.0 / qTan( qDegreesToRadians( static_cast<double>(oldAngle()) ) );
    double ctg2 = 1.0 / qTan( qDegreesToRadians( static_cast<double>(newAngle  ) ) );
    qDebug() << ctg1 << ctg2;
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

QString MainWindow::kStr(double x1, double x2, double y1, double y2)
{
    double ax, by;
    ax = x2 - x1;
    by = y2 - y1;
    QString kString = QString::number(ax/by);
    return kString;
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
}

void MainWindow::on_build_btn_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
    ui->split_gb->setChecked(false);
    plot->clearGraphs();

    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica", 12));
    plot->xAxis->setLabel(ui->xAxis->name());
    plot->yAxis->setLabel(ui->yAxis->name());
    plot->xAxis->setRange( ui->xAxis->min(), ui->xAxis->max());
    plot->yAxis->setRange( ui->yAxis->min(), ui->yAxis->max());

    ui->lineEq_k->setText(kStr(ui->xAxis->max(), ui->xAxis->min(), ui->yAxis->min(), ui->yAxis->max()));
    ui->lineEq_b->setText(bStr(ui->xAxis->max(), ui->xAxis->min(), ui->yAxis->min(), ui->yAxis->max()));
    qreal alpha = ui->yAxis->max() / ui->xAxis->min();
    int alphaAngle = static_cast<int>(qRadiansToDegrees(qAtan(alpha))) ;
    setOldAngle(qRadiansToDegrees(qAtan(alpha)));
    ui->angle_dial->setValue(alphaAngle);
    ui->angle_sb->setValue(alphaAngle);

    plot->addGraph();
    int grNum = plot->graphCount() - 1;
    plot->graph(grNum)->setPen(QPen(ui->pointClr->color()));
    plot->graph(grNum)->setLineStyle(QCPGraph::lsNone);
    plot->graph(grNum)->setScatterStyle( QCPScatterStyle( QCPScatterStyle::ssDisc, ui->pointSize_sb->value() ) );
    plot->graph(grNum)->setData(ui->xAxis->values(), ui->yAxis->values());
    plot->graph(grNum)->setName(DEPENDANCE + ui->yAxis->name() + DEPOF + ui->xAxis->name() );

    QCPErrorBars *yErrorBar = new QCPErrorBars(plot->xAxis, plot->yAxis);
    QCPErrorBars *xErrorBar = new QCPErrorBars(plot->xAxis, plot->yAxis);

    yErrorBar->setErrorType(QCPErrorBars::ErrorType::etValueError);
    xErrorBar->setErrorType(QCPErrorBars::ErrorType::etKeyError);

    yErrorBar->removeFromLegend();
    xErrorBar->removeFromLegend();

    yErrorBar->setAntialiased(false);
    xErrorBar->setAntialiased(false);

    yErrorBar->setDataPlottable(plot->graph(grNum));
    xErrorBar->setDataPlottable(plot->graph(grNum));

    yErrorBar->setPen(QPen(ui->dyAxis->color()));
    xErrorBar->setPen(QPen(ui->dxAxis->color()));

    yErrorBar->setData(ui->dyAxis->values());
    xErrorBar->setData(ui->dxAxis->values());

    plot->graph(grNum)->rescaleAxes(true);

    plot->axisRect()->setupFullAxesBox();

    plot->replot();
}

void MainWindow::on_split_gb_clicked(bool checked)
{
    // Прямая через 1(xmin, ymax) - 2(xmax, ymin)
    if(checked)
    {
        plot->addGraph();

        double xmax, xmin, ymax, ymin;
        xmax = ui->xAxis->max();
        ymin = ui->yAxis->min();
        xmin = ui->xAxis->min();
        ymax = ui->yAxis->max();

        QVector<double> xValues, yValues;
        xValues << xmin << xmax;
        yValues << ymax << ymin;

        int grNum = (plot->graphCount() > 1) ? plot->graphCount() - 1 : 0;

        plot->graph(grNum)->setPen(QPen(ui->splitClr->color(), ui->splitSize_sb->value()));
        plot->graph(grNum)->setLineStyle(QCPGraph::lsLine);
        plot->graph(grNum)->setData(xValues, yValues);
        plot->graph(grNum)->setName(SPLITNAME);
    } else
    {
        plot->removeGraph(plot->graphCount() - 1);
    }
    plot->replot();
}

void MainWindow::on_angle_dial_sliderMoved(int position)
{
    // Прямая через 1(xmin, ymax) - 2(xmax, ymin)
    int grNum = plot->graphCount() - 1;
    plot->removeGraph(grNum);

    double xmax, xmin, ymax, ymin;

    xmin = ui->xAxis->min() + calcDelta(position, ui->yAxis->max());
    ymax = ui->yAxis->max();
    ymin = ui->yAxis->min();
    xmax = ui->xAxis->max() - calcDelta(position, ui->yAxis->max());

    QVector<double> xValues, yValues;
    xValues << xmin << xmax;
    yValues << ymax << ymin;

    ui->lineEq_k->setText(kStr(xmin, xmax, ymax, ymin));
    ui->lineEq_b->setText(bStr(xmin, xmax, ymax, ymin));

    plot->addGraph();
    plot->graph(grNum)->setPen(QPen(ui->splitClr->color(), ui->splitSize_sb->value()));
    plot->graph(grNum)->setLineStyle(QCPGraph::lsLine);
    plot->graph(grNum)->setData(xValues, yValues);
    plot->graph(grNum)->setName(SPLITNAME);

    plot->replot();
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
}

void MainWindow::on_hDelta_H_sldr_sliderMoved(int position)
{

}

void MainWindow::on_vDelta_H_sldr_sliderMoved(int position)
{

}
