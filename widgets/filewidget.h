#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QGroupBox>

namespace Ui {
class FileWidget;
}

class FileWidget : public QGroupBox
{
    Q_OBJECT

public:
    explicit FileWidget(QWidget *parent = nullptr);
    ~FileWidget();

private slots:
    void on_file_btn_clicked();

private:
    Ui::FileWidget *ui;
};

#endif // FILEWIDGET_H
