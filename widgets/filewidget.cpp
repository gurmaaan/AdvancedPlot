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
    if(QFileInfo(filePath).isReadable())
    {
        setPath(filePath);

        CSVFile csvFile(filePath);
        setCsv(csvFile);

        setupSB(ui->descrCnt_sb, csvFile.model()->columnCount());
        setupSB(ui->objctsCnt_sb, csvFile.model()->rowCount());
        ui->fCfR_le->setText(csvFile.getFRfCstr());

        emit modelChanged(csvFile.model());
        setParsingState(true);
    }
    else {
        QMessageBox::critical(nullptr, QGuiApplication::applicationDisplayName(), QString("Не могу открыть файл %1. Пожалуйста попробуйте еще раз.").arg(filePath));
    }
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

QString FileWidget::fileName()
{
    return ui->file_le->text().split("/").last();
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

void FileWidget::setPath(QString filePath)
{
    ui->file_le->setText(filePath);
}

void FileWidget::receiveCSV(CSVFile file)
{
    setCsv(file);
    ui->file_le->setText(file.path());
    ui->fCfR_le->setText(file.getFRfCstr());
    setupSB(ui->descrCnt_sb, file.model()->columnCount());
    setupSB(ui->objctsCnt_sb, file.model()->rowCount());

    emit modelChanged(file.model());
    setParsingState(true);
}

void FileWidget::processFile(QString filePath)
{
    setPath(filePath);

    CSVFile csvFile(filePath);
    setCsv(csvFile);

    setupSB(ui->descrCnt_sb, csvFile.model()->columnCount());
    setupSB(ui->objctsCnt_sb, csvFile.model()->rowCount());
    ui->fCfR_le->setText(csvFile.getFRfCstr());

    emit modelChanged(csvFile.model());
    setParsingState(true);
}
