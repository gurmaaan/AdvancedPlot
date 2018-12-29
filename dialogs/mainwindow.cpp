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

    connect(aW_, &FileWidget::modelChanged,
            ui->xAxis, &AxisWidget::setModel);
    connect(aW_, &FileWidget::modelChanged,
            ui->yAxis, &AxisWidget::setModel);
    connect(aW_, &FileWidget::modelChanged,
            ui->dxAxis, &AxisWidget::setModel);
    connect(aW_, &FileWidget::modelChanged,
            ui->dyAxis, &AxisWidget::setModel);

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
    ui->dyAxis->setT(AxisType::dYAxis);
}

void MainWindow::scrollAndSelect(int colNum)
{
    QAbstractItemModel *aM = ui->fileA_tv->model();
    ui->fileA_tv->scrollTo(aM->index(0,colNum));
    ui->fileA_tv->selectColumn(colNum);
}

void MainWindow::on_build_btn_clicked()
{
    ui->tabWidget->setCurrentIndex(3);

    plot->clearGraphs();

    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica", 9));
    plot->xAxis->setLabel(ui->xAxis->name());
    plot->yAxis->setLabel(ui->yAxis->name());

    plot->addGraph();
    plot->graph(0)->setPen(QPen(ui->pointClr->color()));
    plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    plot->graph(0)->setScatterStyle( QCPScatterStyle( QCPScatterStyle::ssDisc, ui->pointSize_sb->value() ) );
    plot->graph(0)->setName("Dependance " + ui->yAxis->name() + " of " + ui->xAxis->name() );

    QCPErrorBars *errorBars = new QCPErrorBars(plot->xAxis, plot->yAxis);
    errorBars->removeFromLegend();
    errorBars->setAntialiased(false);
    errorBars->setDataPlottable(plot->graph(0));
    errorBars->setPen(QPen(ui->dyAxis->getColor()));

    plot->graph(0)->setData(ui->xAxis->values(), ui->yAxis->values());
    errorBars->setData(ui->dyAxis->values());
    plot->graph(0)->rescaleAxes(true);

    plot->axisRect()->setupFullAxesBox();

    plot->replot();
}
