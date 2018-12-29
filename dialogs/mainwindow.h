#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QComboBox>
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
    //void runCgen(bool status);
    void setupPlotDock(QStandardItemModel *model);
    void scrollAndSelect(int colNum = 0);

private slots:
    void on_build_btn_clicked();
    void on_x_cb_currentIndexChanged(int index);
    void on_y_cb_currentIndexChanged(int index);
    void on_dx_cb_currentIndexChanged(int index);
    void on_dy_cb_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    void connectAll();
    void setupFileWidgets();
    FileWidget *aW_;
//    FileWidget *bW_;
//    FileWidget *cW_;
    QCustomPlot *plot;
};

#endif // MAINWINDOW_H
