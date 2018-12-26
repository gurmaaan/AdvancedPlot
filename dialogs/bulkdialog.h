#ifndef BULKDIALOG_H
#define BULKDIALOG_H

#include <QDialog>

namespace Ui {
class BulkDialog;
}

class BulkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BulkDialog(QWidget *parent = nullptr);
    ~BulkDialog();

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
};

#endif // BULKDIALOG_H
