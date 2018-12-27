#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    aW_ = ui->fileA_widget;
    bW_ = ui->fileB_widget;
    cW_ = ui->fileC_widget;

    setupFileWidgets();
    connectAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::runCgen(bool status)
{
    bool allCompleted = status && aW_->parsingState() && bW_->parsingState();
    cW_->setEnabled(allCompleted);
    ui->save_btn->setEnabled(allCompleted);
    int curInd = 0;
    if( aW_->parsingState() && (!bW_->parsingState()) )
        curInd = 0;
    else if(bW_->parsingState() && (!aW_->parsingState()) )
        curInd = 1;
    else if(allCompleted)
        curInd = 2;
    ui->tabWidget->setCurrentIndex(curInd);

    if(allCompleted)
    {
        BulkDialog *bulk = new BulkDialog;
        bulk->show();
        bulk->processFiles(aW_->csv(), bW_->csv());
    }
}

void MainWindow::connectAll()
{
    connect(aW_, &FileWidget::modelChanged,
            ui->fileA_tv, &QTableView::setModel);
    connect(bW_, &FileWidget::modelChanged,
            ui->fileB_tv, &QTableView::setModel);

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

void MainWindow::on_save_btn_clicked()
{
//    BulkDialog *bulk = new BulkDialog(this);
//    bulk->show();
}
