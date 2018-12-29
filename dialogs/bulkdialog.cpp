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
    QStandardItemModel *cModel = genEmptyModelWithHeaders();

    copyItems(aModel_, cModel, duplicatedNames_, "_A", false);
    copyItems(aModel_, cModel, duplicatedNames_, "_A", true);
    copyItems(bModel_, cModel, duplicatedNames_, "_B", true);
    copyItems(bModel_, cModel, duplicatedNames_, "_B", false);

    CSVFile fileC(cPath_, cModel);
    emit fileCompleted(fileC);
    close();
}

void BulkDialog::on_action_rule_2_triggered()
{
    QStandardItemModel *cModel = genEmptyModelWithHeaders();

    copyItems(aModel_, cModel, duplicatedNames_, "_A", false);
    copyItems(aModel_, cModel, duplicatedNames_, "_A", true);
    copyItems(bModel_, cModel, duplicatedNames_, "_B", false);

    CSVFile fileC(cPath_, cModel);
    emit fileCompleted(fileC);
    close();
}

void BulkDialog::on_action_rule_3_triggered()
{
    QStandardItemModel *cModel = genEmptyModelWithHeaders();

    copyItems(aModel_, cModel, duplicatedNames_, "_A", false);
    copyItems(bModel_, cModel, duplicatedNames_, "_B", false);

    CSVFile fileC(cPath_, cModel);
    emit fileCompleted(fileC);
    close();
}

void BulkDialog::on_action_rule_4_triggered()
{
    QStandardItemModel *cModel = genEmptyModelWithHeaders();

    copyItems(aModel_, cModel, duplicatedNames_, "_A", true);
    copyItems(bModel_, cModel, duplicatedNames_, "_B", true);

    CSVFile fileC(cPath_, cModel);
    emit fileCompleted(fileC);
    close();
}

void BulkDialog::on_action_rule_5_triggered()
{
    QStandardItemModel *cModel = genEmptyModelWithHeaders();

    copyItems(aModel_, cModel, duplicatedNames_, "_A", true);

    CSVFile fileC(cPath_, cModel);
    emit fileCompleted(fileC);
    close();
}

void BulkDialog::on_action_rule_6_triggered()
{
    QStandardItemModel *cModel = genEmptyModelWithHeaders();

    copyItems(aModel_, cModel, duplicatedNames_, "_A", false);

    CSVFile fileC(cPath_, cModel);
    emit fileCompleted(fileC);
    close();
}

void BulkDialog::on_action_rule_7_triggered()
{
    QStandardItemModel *cModel = genEmptyModelWithHeaders();

    copyItems(aModel_, cModel, duplicatedNames_, "_A", false);
    copyItems(bModel_, cModel, duplicatedNames_, "_B", true);

    CSVFile fileC(cPath_, cModel);
    emit fileCompleted(fileC);
    close();
}

void BulkDialog::on_action_rule_8_triggered()
{
    CSVFile fileC(fileA_);
    emit fileCompleted(fileC);
    close();
}

void BulkDialog::on_action_rule_9_triggered()
{
    QStandardItemModel *cModel = genEmptyModelWithHeaders();

    copyItems(aModel_, cModel, duplicatedNames_, "_A", false);
    copyItems(bModel_, cModel, duplicatedNames_, "_B", false);

    CSVFile fileC(cPath_, cModel);
    emit fileCompleted(fileC);
    close();
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
    QStringList aDescrList;
    aDescrList.clear();

    for(int i = 0; i < aF.model()->columnCount(); i++)
        aDescrList << aF.model()->headerData(i, Qt::Horizontal).toString();

    QStringList bDescrList;
    bDescrList.clear();

    for(int i = 0; i < bF.model()->columnCount(); i++)
        bDescrList << bF.model()->headerData(i, Qt::Horizontal).toString();

    aDescrList.sort();
    bDescrList.sort();

    for(int i = 0; i < aDescrList.count(); i++)
    {
        qDebug() << aDescrList.at(i) << " : " << bDescrList.at(i);
    }
    //FIXME кривое определение дескрипторов
    bool dE = (aF.model()->columnCount() == bF.model()->columnCount());
    qDebug() << dE << aF.model()->columnCount() << bF.model()->columnCount();
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

bool BulkDialog::oneOfDuplicated(QString test, QStringList duplicates)
{
    bool testIsDup = false;
    for(QString dup : duplicates)
    {
        if(test == dup)
        {
            testIsDup = true;
            break;
        }
    }
    return testIsDup;
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

void BulkDialog::setCPath(const QString &cPath)
{
    cPath_ = cPath;
}

void BulkDialog::setBModel(QStandardItemModel *bModel)
{
    bModel_ = bModel;
}

void BulkDialog::setAModel(QStandardItemModel *aModel)
{
    aModel_ = aModel;
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

void BulkDialog::copyItems(QStandardItemModel *sourceModel, QStandardItemModel *targetModel, QStringList duplicates, QString suffix, bool onlyDuplicates)
{
    //TODO доработать
    for (int i = 0; i < sourceModel->rowCount(); i++)
    {
        QString originalObjNameAtI = sourceModel->headerData(i, Qt::Vertical).toString();
        bool condition;
        if(onlyDuplicates)
        {
            condition = oneOfDuplicated(originalObjNameAtI, duplicates);
            originalObjNameAtI = originalObjNameAtI + suffix;
            qDebug() << onlyDuplicates << originalObjNameAtI << condition;
        }
        else
        {
            condition = !oneOfDuplicated(originalObjNameAtI, duplicates);
        }

        if(!condition)
            continue;
        else
        {
            QList<QStandardItem*> rowSourceAtI = sourceModel->takeRow(i);
            QList<QStandardItem*> rowCopyAtI;
            for(QStandardItem *aIt : rowSourceAtI)
            {
                QStandardItem *cIt = aIt->clone();
                rowCopyAtI << cIt;
            }
            targetModel->appendRow(rowCopyAtI);
            QStandardItem *vHItem = new QStandardItem(originalObjNameAtI);
            CSVFile::makeHeader(vHItem, Qt::Vertical);
            targetModel->setVerticalHeaderItem(targetModel->rowCount() - 1, vHItem);
        }
    }
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
    setAModel(aF.model());
    setFileB(bF);
    setBModel(bF.model());
    setCPath( genPath(aF.path(), bF.path(), "C.csv") );

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
