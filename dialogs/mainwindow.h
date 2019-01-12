#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtMath>
#include <QComboBox>
//
#include "csvfile.h"
#include "static.h"
#include <widgets/qcustomplot.h>
#include "widgets/filewidget.h"
#include "widgets/axiswidget.h"
#include "dialogs/bulkdialog.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief      Class for main window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
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
    void on_hDelta_H_sldr_sliderMoved(int position);
    void on_vDelta_H_sldr_sliderMoved(int position);
    //
    // @brief      { function_description }
    //
    void mouseWheel();
    void mousePress();

private:
    Ui::MainWindow *ui;
    BulkDialog *bulkDialog_;
    FileWidget *aW_;
    FileWidget *bW_;
    FileWidget *cW_;
    double oldAngle_;
    QCustomPlot *plot_;

    /**
     * @brief      Connects all.
     */
    void connectAll();
    void setupWidgets();

    /**
     * @brief      Gets the plot data.
     *
     * @param      model     The model
     * @param[in]  colIndex  The col index
     * @param[in]  suffix    The suffix
     *
     * @return     The plot data.
     */
    QVector<double> getPlotData(QStandardItemModel *model, int colIndex, QString suffix);
    double calcDelta(int newAngle, double y11);

    /**
     * @brief      { function_description }
     *
     * @param      xA    The x a
     * @param      yA    The y a
     *
     * @return     { description_of_the_return_value }
     */
    QString kStr(AxisWidget *xA, AxisWidget *yA);
    QString kStr(double x1, double x2, double y1, double y2);
    QString bStr(AxisWidget *xA, AxisWidget *yA);
    QString bStr(double x1, double x2, double y1, double y2);
    void setLineEq(AxisWidget *xA, AxisWidget *yA);
    void setLineEq(double x1, double x2, double y1, double y2);

    /**
     * @brief      Adds an error.
     *
     * @param      plotVie    The plot vie
     * @param      axis       The axis
     * @param[in]  errorData  The error data
     */
    void addError(QCustomPlot *plotVie, AxisWidget *axis, QVector<double> errorData);
    void addPoints(QCustomPlot *plotVie, QVector<double> datax, QVector<double> datay, QColor clr, int size, QString name);
    void setupAxis(QCustomPlot *plotVie, AxisWidget *axis);
    void setupDelta(QSlider *slider, AxisWidget *axis);
};

#endif // MAINWINDOW_H
