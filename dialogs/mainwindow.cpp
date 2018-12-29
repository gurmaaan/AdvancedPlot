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
    setupFileWidgets();
    connectAll();
    showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPlotDock(QStandardItemModel *model)
{
    QStringList comboBoxItems;
    for(int i = 0; i < model->columnCount(); i++)
        comboBoxItems.append(model->horizontalHeaderItem(i)->statusTip());

    ui->x_cb->addItems(comboBoxItems);
    ui->y_cb->addItems(comboBoxItems);
    ui->dx_cb->addItems(comboBoxItems);
    ui->dy_cb->addItems(comboBoxItems);
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

    connect(bulkDialog_, &BulkDialog::fileCompleted,
            cW_, &FileWidget::receiveCSV);

    connect(aW_, &FileWidget::parsingDone,
            this, &MainWindow::runCgen);
    connect(bW_, &FileWidget::parsingDone,
            this, &MainWindow::runCgen);
}

void MainWindow::setupFileWidgets()
{
    aW_->setTitle("Файл А");
    bW_->setTitle("Файл В");
    cW_->setTitle("Файл С");
    cW_->setEnabled(false);
    cW_->setBtnVisible(false);
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
    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica", 9));
    plot->xAxis->setLabel("Xui");
    plot->xAxis->setRange(-228,228);
    plot->replot();
}

void MainWindow::on_x_cb_currentIndexChanged(int index)
{
    scrollAndSelect(index);
}

void MainWindow::on_y_cb_currentIndexChanged(int index)
{
    scrollAndSelect(index);
}

void MainWindow::on_dx_cb_currentIndexChanged(int index)
{
    scrollAndSelect(index);
}

void MainWindow::on_dy_cb_currentIndexChanged(int index)
{
    scrollAndSelect(index);
}
