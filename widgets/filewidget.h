#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QGroupBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QSpinBox>
#include <csvfile.h>

namespace Ui {
class FileWidget;
}

class FileWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit FileWidget(QWidget *parent = nullptr);
    ~FileWidget();

    CSVFile csv() const;
    void setCsv(const CSVFile &csv);

signals:
    void parsingDone(QStandardItemModel *model);

private slots:
    void on_file_btn_clicked();

private:
    Ui::FileWidget *ui;
    CSVFile csv_;
    QString requiredPath(QDir currentDir, const QString &redirect, const QStandardPaths::StandardLocation &loc);
    void setupSB(QSpinBox *sb, int max = 0);
};

#endif // FILEWIDGET_H
