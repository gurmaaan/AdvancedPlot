#ifndef BULKDIALOG_H
#define BULKDIALOG_H

#include <QDialog>
#include <QAction>
#include <QBitArray>
#include <QDir>
#include <QDebug>
#include <QIODevice>
#include <QFileDialog>

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

    void setAModel(QStandardItemModel *aModel);
    void setBModel(QStandardItemModel *bModel);

    void setCPath(const QString &cPath);

public slots:
    void setDescriptorsIsValid(bool descriptorsIsValid);
    void setDuplicatesFound(bool duplicatesFound);
    void setFilesNotEmpty(bool filesNotEmpty);

signals:
    void fileCompleted(CSVFile file);

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
    bool checkFIlesNotEmpty(CSVFile aF, CSVFile bF);

    QStringList duplicatedNames_;
    QStringList findDuplicatedObjectNames(CSVFile aF, CSVFile bF);
    bool oneOfDuplicated(QString test, QStringList duplicates);

    QBitArray activeAct_;
    void actionsActivating(const QBitArray &activeAct);
    QString genPath(QString pathA, QString pathB, QString nameC);

    QStandardItemModel *aModel_;
    QStandardItemModel *bModel_;
    QString cPath_;

    QStandardItemModel *genEmptyModelWithHeaders();
    void copyItems(QStandardItemModel *sourceModel, QStandardItemModel *targetModel, QStringList duplicates,QString suffix, bool onlyDuplicates);
};

#endif // BULKDIALOG_H
