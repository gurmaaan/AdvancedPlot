#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
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

private slots:

    void on_save_btn_clicked();

private:
    Ui::MainWindow *ui;
    void connectAll();
    void setupFileWidgets();

    FileWidget *aW_;
    FileWidget *bW_;
    FileWidget *cW_;
};

#endif // MAINWINDOW_H
