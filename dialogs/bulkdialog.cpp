#include "bulkdialog.h"
#include "ui_bulkdialog.h"

BulkDialog::BulkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BulkDialog)
{
    ui->setupUi(this);
    copyActionsUIToBtns();

    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowFlag(Qt::WindowMinMaxButtonsHint);

    activeAct_.resize(9);
    activeAct_.fill(false, 0, 8);
    actionsActivating(activeAct_);
}

BulkDialog::~BulkDialog()
{
    delete ui;
}

void BulkDialog::on_action_rule_1_triggered()
{
    QStringList aObjNames = fileA_.objNames();
    QStringList bObjNames = fileB_.objNames();

    QString cPath = genPath(fileA_.path(), fileB_.path(), "C.csv");
    QStandardItemModel *cModel = genEmptyModelWithHeaders();
    CSVFile fileC(cPath, cModel);
    emit fileCompleted(fileC);
    close();
}

void BulkDialog::on_action_rule_2_triggered()
{

}

void BulkDialog::on_action_rule_3_triggered()
{

}

void BulkDialog::on_action_rule_4_triggered()
{

}

void BulkDialog::on_action_rule_5_triggered()
{

}

void BulkDialog::on_action_rule_6_triggered()
{

}

void BulkDialog::on_action_rule_7_triggered()
{

}

void BulkDialog::on_action_rule_8_triggered()
{

}

void BulkDialog::on_action_rule_9_triggered()
{

}

void BulkDialog::copyActionsUIToBtns()
{
    for(int i = 1; i <= 9; i++)
    {
        QString actionName = ACT_NAME + QString::number(i);
        QString btnName = BTN_NAME + QString::number(i);
        QAction *act = findChild<QAction*>(actionName);
        QCommandLinkButton *cmdBtn = this->findChild<QCommandLinkButton*>(btnName);
        cmdBtn->setIcon(act->icon());
        cmdBtn->setText(act->text());
        cmdBtn->setDescription(act->toolTip());
    }
}

void BulkDialog::setFilesNotEmpty(bool filesNotEmpty)
{
    filesNotEmpty_ = filesNotEmpty;
    ui->filesValid_yes_rb->setChecked(filesNotEmpty);
    ui->filesValid_no_rb->setChecked(!ui->filesValid_yes_rb->isChecked());
}

bool BulkDialog::compareDescriptors(CSVFile aF, CSVFile bF)
{
    QStringList aDescrList = aF.descriptorsNames();
    QStringList bDescrList = bF.descriptorsNames();

    bool dE = (aDescrList == bDescrList);
    return dE;
}

QStringList BulkDialog::findDuplicatedObjectNames(CSVFile aF, CSVFile bF)
{
    QStringList duplicatesList;
    QStringList aObjectsNames = aF.objNames();
    QStringList bObjectsNames = bF.objNames();

    for(int i = 0; i < aObjectsNames.count(); i++)
    {
        for(int j = 0; j < bObjectsNames.count(); j++)
        {
            if( aObjectsNames.at(i) == bObjectsNames.at(j))
                duplicatesList << aObjectsNames.at(i);
        }
    }
    return duplicatesList;
}

bool BulkDialog::checkFIlesNotEmpty(CSVFile aF, CSVFile bF)
{
    bool notEmpty = !((aF.model()->rowCount() == 0) || (bF.model()->rowCount() == 0));
    return notEmpty;
}

void BulkDialog::actionsActivating(const QBitArray &activeAct)
{
    for(int i = 1; i <= 9; i++)
    {
        QString btnName = BTN_NAME + QString::number(i);
        QCommandLinkButton *cmdBtn = this->findChild<QCommandLinkButton*>(btnName);
        cmdBtn->setEnabled(activeAct.at(i-1));
    }
}

QString BulkDialog::genPath(QString pathA, QString pathB, QString nameC)
{
    QString nameA = pathA.split("/").last();
    QString dirA = pathA.left(pathA.length() - nameA.length());
    QString nameB = pathB.split("/").last();
    QString dirB = pathB.left(pathB.length() - nameB.length());
    QString pathC = "";
    if(dirA == dirB)
        pathC = dirA + nameC;
    else {
        pathC = QFileDialog::getSaveFileName(nullptr, "Выберите папку для сохранения файла");
    }
    return pathC;
}

QStandardItemModel *BulkDialog::genEmptyModelWithHeaders()
{
    QStandardItemModel *cModel = new QStandardItemModel;
    for(int j = 0; j < fileA_.model()->columnCount(); j++)
    {
        QStandardItem *hHItem = new QStandardItem(fileA_.model()->horizontalHeaderItem(j)->text());
        CSVFile::makeHeader(hHItem, Qt::Horizontal);
        cModel->setHorizontalHeaderItem(cModel->columnCount(), hHItem);
    }
    return cModel;
}

QStringList BulkDialog::duplicatedNames() const
{
    return duplicatedNames_;
}

void BulkDialog::setDuplicatedNames(const QStringList &duplicatedNames)
{
    duplicatedNames_ = duplicatedNames;
}

void BulkDialog::processFiles(const CSVFile &aF, const CSVFile &bF)
{
    setFileA(aF);
    setFileB(bF);
    setDescriptorsIsValid( compareDescriptors(aF, bF) );
    QStringList duplicatedObjectsNames = findDuplicatedObjectNames(aF, bF);
    setDuplicatedNames(duplicatedObjectsNames);
    ui->duplicatesCnt_sb->setValue(duplicatedObjectsNames.count());
    bool dup = (duplicatedObjectsNames.count() == 0);
    setDuplicatesFound(!dup);
    setFilesNotEmpty(checkFIlesNotEmpty(aF, bF));

    if(ui->descriptors_no_rb->isChecked() || ui->filesValid_no_rb->isChecked())
    {
        activeAct_.fill(false, 0, 9);
        actionsActivating(activeAct_);
    }
}

void BulkDialog::setDuplicatesFound(bool duplicatesFound)
{
    duplicatesFound_ = duplicatesFound;
    ui->duplicates_yes_rb->setChecked(duplicatesFound);
    ui->duplicates_no_rb->setChecked(!ui->duplicates_yes_rb->isChecked());

    activeAct_.fill(duplicatesFound, 0, 7);
    activeAct_.fill(!duplicatesFound, 7, 9);

    actionsActivating(activeAct_);
}

void BulkDialog::setDescriptorsIsValid(bool descriptorsIsValid)
{
    descriptorsIsValid_ = descriptorsIsValid;
    ui->descriptors_yes_rb->setChecked(descriptorsIsValid);
    ui->descriptors_no_rb->setChecked(!ui->descriptors_yes_rb->isChecked());
}

void BulkDialog::setFileB(const CSVFile &bF)
{
    fileB_ = bF;
}

void BulkDialog::setFileA(const CSVFile &aF)
{
    fileA_ = aF;
}
