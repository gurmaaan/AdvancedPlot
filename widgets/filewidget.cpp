#include "filewidget.h"
#include "ui_filewidget.h"

FileWidget::FileWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::FileWidget)
{
    ui->setupUi(this);
}

FileWidget::~FileWidget()
{
    delete ui;
}

void FileWidget::on_file_btn_clicked()
{

}
