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

void MainWindow::scrollAndSelect(int colNum)
{
    QAbstractItemModel *cM = ui->fileC_tv->model();
    ui->fileA_tv->scrollTo(cM->index(0,colNum));
    ui->fileA_tv->selectColumn(colNum);
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

    plot->clearGraphs();

    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica", 9));
    plot->xAxis->setLabel(ui->xAxis->name());
    plot->yAxis->setLabel(ui->yAxis->name());
    plot->xAxis->setRange( ui->xAxis->min(), ui->xAxis->max());
    plot->yAxis->setRange( ui->yAxis->min(), ui->yAxis->max());


    plot->addGraph();
    plot->graph(0)->setPen(QPen(ui->pointClr->color()));
    plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    plot->graph(0)->setScatterStyle( QCPScatterStyle( QCPScatterStyle::ssDisc, ui->pointSize_sb->value() ) );
    plot->graph(0)->setData(ui->xAxis->values(), ui->yAxis->values());
    plot->graph(0)->setName(DEPENDANCE + ui->yAxis->name() + DEPOF + ui->xAxis->name() );

    QCPErrorBars *yErrorBar = new QCPErrorBars(plot->xAxis, plot->yAxis);
    QCPErrorBars *xErrorBar = new QCPErrorBars(plot->xAxis, plot->yAxis);

    yErrorBar->setErrorType(QCPErrorBars::ErrorType::etValueError);
    xErrorBar->setErrorType(QCPErrorBars::ErrorType::etKeyError);

    yErrorBar->removeFromLegend();
    xErrorBar->removeFromLegend();

    yErrorBar->setAntialiased(false);
    xErrorBar->setAntialiased(false);

    yErrorBar->setDataPlottable(plot->graph(0));
    xErrorBar->setDataPlottable(plot->graph(0));

    yErrorBar->setPen(QPen(ui->dyAxis->color()));
    xErrorBar->setPen(QPen(ui->dxAxis->color()));

    yErrorBar->setData(ui->dyAxis->values());
    xErrorBar->setData(ui->dxAxis->values());

    plot->graph(0)->rescaleAxes(true);

    plot->axisRect()->setupFullAxesBox();

    plot->replot();
}

void MainWindow::on_split_gb_clicked(bool checked)
{
    if(checked)
    {
        plot->addGraph();
        QVector<double> xValues;
        xValues << ui->xAxis->min() << ui->xAxis->max();
        QVector<double> yValues;
        yValues << ui->yAxis->max() << ui->yAxis->min();
        plot->graph(1)->setPen(QPen(ui->splitClr->color(), ui->splitSize_sb->value()));
        plot->graph(1)->setLineStyle(QCPGraph::lsLine);
        plot->graph(1)->setData(xValues, yValues);
        plot->graph(1)->setName(SPLITNAME);
    } else
    {
        plot->removeGraph(1);
    }
    plot->replot();
}
