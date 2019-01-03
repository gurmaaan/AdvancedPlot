#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QComboBox>
//
#include "csvfile.h"
#include <widgets/qcustomplot.h>
#include "widgets/filewidget.h"
#include "dialogs/bulkdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void runCgen(bool status);
    void scrollAndSelect(int colNum = 0);
    void setAxisModel(QStandardItemModel *m);

private slots:
    void on_build_btn_clicked();
private:
    Ui::MainWindow *ui;
    BulkDialog *bulkDialog_;
    void connectAll();
    void setupWidgets();
    FileWidget *aW_;
    FileWidget *bW_;
    FileWidget *cW_;
    QCustomPlot *plot;
};

#endif // MAINWINDOW_H
