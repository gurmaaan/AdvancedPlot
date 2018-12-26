#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectAll()
{
    connect(ui->fileA_widget, &FileWidget::parsingDone,
            ui->fileA_tv, &QTableView::setModel);
    connect(ui->fileB_widget, &FileWidget::parsingDone,
            ui->fileB_tv, &QTableView::setModel);
}
