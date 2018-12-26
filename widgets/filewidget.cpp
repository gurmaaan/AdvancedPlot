#include "filewidget.h"
#include "ui_filewidget.h"

FileWidget::FileWidget(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::FileWidget)
{
    ui->setupUi(this);
    parsingState_ = false;
}

FileWidget::~FileWidget()
{
    delete ui;
}

void FileWidget::on_file_btn_clicked()
{
    QString openLocation = requiredPath(QDir::current(), DATA_LOCATION, QStandardPaths::DocumentsLocation);
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Выберите CSV файл", openLocation, FILE_TYPE);
    ui->file_le->setText(filePath);

    CSVFile csvFile(filePath);
    setCsv(csvFile);

    int rcnt = csvFile.model()->rowCount();
    int ccnt = csvFile.model()->columnCount();
    setupSB(ui->descrCnt_sb, ccnt);
    setupSB(ui->objctsCnt_sb, rcnt);

    emit modelChanged(csvFile.model());

    setParsingState(true);
}

bool FileWidget::parsingState() const
{
    return parsingState_;
}

void FileWidget::setParsingState(bool parsingState)
{
    parsingState_ = parsingState;
    emit parsingDone(parsingState);
}

QString FileWidget::requiredPath(QDir currentDir, const QString &redirect, const QStandardPaths::StandardLocation &loc)
{
#ifdef Q_OS_MAC
    currentDir.cdUp();
    currentDir.cdUp();
    currentDir.cdUp();
#endif

    currentDir.cdUp();
    currentDir.cd(redirect);
    QString defaultLocation = QStandardPaths::writableLocation(loc);
    QString path = "";
    if( currentDir.exists() )
    {
        path = currentDir.absolutePath();
    }
    else
    {
        path = defaultLocation;
        qDebug() << "Invalid location - " << currentDir.absolutePath() << " / " << redirect;
    }
    return path;
}

void FileWidget::setupSB(QSpinBox *sb, int max)
{
    sb->setRange(0, max);
    sb->setValue(max);
}

CSVFile FileWidget::csv() const
{
    return csv_;
}

void FileWidget::setCsv(const CSVFile &csv)
{
    csv_ = csv;
}

void FileWidget::setBtnVisible(bool status)
{
    ui->file_btn->setVisible(status);
}
