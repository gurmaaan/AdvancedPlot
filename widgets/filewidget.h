#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QGroupBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QSpinBox>
#include <QGuiApplication>
#include <QMessageBox>
//
#include <csvfile.h>

namespace Ui {
class FileWidget;
}

/**
 * @brief      Class for file widget.
 */
class FileWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit FileWidget(QWidget *parent = nullptr);
    ~FileWidget();

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    CSVFile csv() const;
    void setCsv(const CSVFile &csv);

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    bool parsingState() const;
    void setParsingState(bool parsingState);

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    QString fileName();

public slots:
    void setBtnVisible(bool status);
    void setPath(QString filePath);
    void receiveCSV(CSVFile file);
    void processFile(QString filePath);

signals:
    void modelChanged(QStandardItemModel *model);
    void parsingDone(bool status);

private slots:
    void on_file_btn_clicked();

private:
    Ui::FileWidget *ui;
    CSVFile csv_;
    bool parsingState_;
    QString requiredPath(QDir currentDir, const QString &redirect, const QStandardPaths::StandardLocation &loc);
    void setupSB(QSpinBox *sb, int max = 0);
};

#endif // FILEWIDGET_H
