#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtMath>
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

    double oldAngle() const;
    void setOldAngle(double oldAngle);

public slots:
    void runCgen(bool status);
    void scrollAndSelect(int colNum = 0);
    void setAxisModel(QStandardItemModel *m);

private slots:
    void on_build_btn_clicked();
    void on_split_gb_clicked(bool checked);
    void on_angle_dial_sliderMoved(int position);
    void on_actionDebug_triggered();
    void setComboBoxes();

private:
    Ui::MainWindow *ui;
    BulkDialog *bulkDialog_;
    void connectAll();
    void setupWidgets();
    FileWidget *aW_;
    FileWidget *bW_;
    FileWidget *cW_;
    double oldAngle_;
    QCustomPlot *plot;
    double calcDelta(int newAngle, double y11);
    QString kStr(double x1, double x2, double y1, double y2);
    QString bStr(double x1, double x2, double y1, double y2);
};

#endif // MAINWINDOW_H
