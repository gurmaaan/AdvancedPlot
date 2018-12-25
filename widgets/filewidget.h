#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QGroupBox>
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

private slots:
    void on_file_btn_clicked();

private:
    Ui::FileWidget *ui;
    CSVFile csv_;
};

#endif // FILEWIDGET_H
