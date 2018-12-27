#ifndef BULKDIALOG_H
#define BULKDIALOG_H

#include <QDialog>
#include <QAction>
#include <QBitArray>

#include "csvfile.h"
#include "static.h"

namespace Ui {
class BulkDialog;
}

class BulkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BulkDialog(QWidget *parent = nullptr);
    ~BulkDialog();

    void setFileA(const CSVFile &aF);
    void setFileB(const CSVFile &bF);

    QStringList duplicatedNames() const;
    void setDuplicatedNames(const QStringList &duplicatedNames);

    void processFiles(const CSVFile &aF,const CSVFile &bF);

public slots:
    void setDescriptorsIsValid(bool descriptorsIsValid);
    void setDuplicatesFound(bool duplicatesFound);
    void setFilesNotEmpty(bool filesNotEmpty);

private slots:
    void on_action_rule_1_triggered();
    void on_action_rule_2_triggered();
    void on_action_rule_3_triggered();
    void on_action_rule_4_triggered();
    void on_action_rule_5_triggered();
    void on_action_rule_6_triggered();
    void on_action_rule_7_triggered();
    void on_action_rule_8_triggered();
    void on_action_rule_9_triggered();

private:
    Ui::BulkDialog *ui;
    void copyActionsUIToBtns();
    CSVFile fileA_;
    CSVFile fileB_;

    bool descriptorsIsValid_;
    bool duplicatesFound_;
    bool filesNotEmpty_;

    bool compareDescriptors(CSVFile aF, CSVFile bF);
    QStringList duplicatedNames_;
    QStringList findDuplicatedObjectNames(CSVFile aF, CSVFile bF);
    bool checkFIlesNotEmpty(CSVFile aF, CSVFile bF);

    QBitArray activeAct_;
    void actionsActivating(const QBitArray &activeAct);
};

#endif // BULKDIALOG_H
