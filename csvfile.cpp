#include "csvfile.h"

CSVFile::CSVFile(QObject *parent) : QObject(parent)
{

}

CSVFile::CSVFile(const CSVFile &csv, QObject *parent) : QObject(parent)
{
    path_ = csv.path_;
    fRfCstr_ = csv.fRfCstr_;
    delim_ = csv.delim_;
    model_ = csv.model_;
}

CSVFile::CSVFile(QString pathToOpen)
{
    loadFromFile(pathToOpen);
}

CSVFile::CSVFile(QString pathToSave, QStandardItemModel *model)
{
    QFile file(pathToSave);
    if( file.open(QIODevice::WriteOnly) )
    {
        setPath(pathToSave);
        setFRfCstr("...");
        setDelim(';');
        setModel(model);
    }

}

QStandardItemModel *CSVFile::model() const
{
    return model_;
}

void CSVFile::setModel(QStandardItemModel *model)
{
    model_ = model;
}

void CSVFile::saveToFile(QString pathToSave)
{
    if(pathToSave == "")
    {
        QString userPathToSave = QFileDialog::getSaveFileName(nullptr, "Выберите папку для сохранения");
        if( checkPath(userPathToSave) )
            setPath(userPathToSave);
    }
    else
    {
        if( checkPath(pathToSave) )
            setPath(pathToSave);
    }
    QFile file(path());
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream textStream( &file );
        textStream << fileHeaderStr() << endl;
        for(QString objName : objNames())
            textStream << getObjStr(objName) << endl;
    }
}

QString CSVFile::path() const
{
    return path_;
}

void CSVFile::setPath(const QString &path)
{
    path_ = path;
}

QStringList CSVFile::descriptorsNames() const
{
    QStringList descrNames;
    for(int j = 0; j < model_->columnCount(); j++)
        descrNames.append( model_->horizontalHeaderItem(j)->text() );
    return descrNames;
}

QStringList CSVFile::objNames() const
{
    QStringList objNames;
    for(int i = 0; i < model_->rowCount(); i++)
        objNames.append( model_->verticalHeaderItem(i)->text() );
    return objNames;
}

QString CSVFile::delim() const
{
    return QString(delim_);
}

void CSVFile::setDelim(const QChar &delim)
{
    delim_ = delim;
}

CSVFile &CSVFile::operator=(const CSVFile &csv)
{
    if( this == &csv)
        return *this;

    if( checkPath(csv.path_) )
        path_ = csv.path_;

    delim_ = csv.delim_;
    model_ = csv.model_;

    return *this;
}

QString CSVFile::getFRfCstr() const
{
    return fRfCstr_;
}

void CSVFile::setFRfCstr(const QString &fRfcStr)
{
    fRfCstr_ = fRfcStr;
}

QString CSVFile::fileHeaderStr()
{
    QString hStr = fRfCstr_ + delim();
    for(QString h : descriptorsNames())
    {
        if(h == descriptorsNames().last())
            hStr = hStr + h;
        else {
            hStr = hStr + h + delim();
        }
    }
    return  hStr;
}

QString CSVFile::getObjStr(QString objName)
{
    QString row = "";
    row = objName + delim();

    QList<QStandardItem*> modelRow = model_->takeRow( objNames().indexOf(objName) );
    for(QStandardItem *item :  modelRow)
    {
       if( item == modelRow.last() )
           row = row + item->text();
       else
           row = row + item->text() + delim();
    }
    return row;
}

void CSVFile::loadFromFile(QString path)
{
    if( checkPath(path) )
    {
        setPath(path);

        QFile csvFile(path);
        csvFile.open(QFile::ReadOnly | QFile::Text);
        QTextStream in( &csvFile );
        QString fileContentStr = in.readAll();

        //FIXME добавить автоматическое определение разделителя файла
        //QChar delim = detectDelim(fileContentStr);
        setDelim(';');

        QStandardItemModel *csvModel = new QStandardItemModel;

        QStringList rowStrList = fileContentStr.split('\n');
        QStringList headerRowStrList = rowStrList.first().split(delim());
        setFRfCstr( headerRowStrList.first() );
        headerRowStrList.removeFirst();

        for(QString headerStr : headerRowStrList)
        {
            QStandardItem *hHItem = new QStandardItem(headerStr);
            makeHeader(hHItem, Qt::Horizontal);
            csvModel->setHorizontalHeaderItem(csvModel->columnCount(), hHItem);
        }
        rowStrList.removeFirst();

        QString objName = "";
        for(QString row : rowStrList)
        {
            QList<QStandardItem*> modelRow;
            QStringList curRowStrList = row.split(delim());

            objName = curRowStrList.first();
            if(objName != "")
            {
                curRowStrList.removeFirst();
                for(QString itemStr : curRowStrList)
                {
                    QStandardItem *item = new QStandardItem(itemStr);
                    setAllTextData(item);
                    item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
                    itemStr.replace( QChar(','), QChar('.') );
                    item->setData( itemStr.toDouble(), Qt::UserRole );
                    modelRow << item;
                }

                csvModel->appendRow(modelRow);

                QStandardItem *vHItem = new QStandardItem(objName);
                makeHeader(vHItem, Qt::Vertical);
                csvModel->setVerticalHeaderItem(csvModel->rowCount() - 1, vHItem);
            }
        }
        setModel(csvModel);
    }
}

bool CSVFile::checkPath(QString path)
{
    bool pathIsValid = fileExists(path) && extensionValid(path);
    return pathIsValid;
}

bool CSVFile::extensionValid(QString path)
{
    bool correctExtension = false;
    QString fileExt = path.split(".").last();
    fileExt = fileExt.toUpper();
    correctExtension = (fileExt == EXTENSION) ? true : false;
    return correctExtension;
}

bool CSVFile::fileExists(QString path)
{
    bool correctFile = false;
    QFileInfo testFile(path);
    correctFile = testFile.exists() && testFile.isFile();
    return correctFile;
}

void CSVFile::makeHeader(QStandardItem *item, Qt::Orientation orient)
{
    Qt::AlignmentFlag al = (orient == Qt::Vertical) ? Qt::AlignRight : Qt::AlignCenter;
    setAllTextData(item);
    QFont f = item->font();
    f.setBold(true);
    item->setData(f, Qt::FontRole);
    item->setData(al, Qt::TextAlignmentRole);
}

void CSVFile::setAllTextData(QStandardItem *item)
{
    QString value = item->text();
    item->setData(value, Qt::StatusTipRole);
    item->setData(value, Qt::WhatsThisRole);
}

QChar CSVFile::detectDelim(QString fileContentStr)
{
    //TODO : Норм алгоритм определения разделителя (построчный)
    int charFrequency[255];
    int asciiStart = QChar('a').toLatin1();
    for(auto c : fileContentStr)
    {
        charFrequency[c.toLatin1() - asciiStart]++;
    }
    int max = 0;
    int maxInd = 0;
    for(int i = 0; i < 255; i++)
    {
        if(charFrequency[i] > max)
        {
            max = charFrequency[i];
            maxInd = i;
        }
    }
    QChar delimC(maxInd);
    setDelim(delimC);
    return  delimC;
}
