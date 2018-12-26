#include "csvfile.h"

CSVFile::CSVFile(QObject *parent) : QObject(parent)
{

}

CSVFile::CSVFile(const CSVFile &csv, QObject *parent) : QObject(parent)
{
    path_ = csv.path_;
    descriptorsNames_ = csv.descriptorsNames_;
    objects_ = csv.objects_;
    delim_ = csv.delim_;
    model_ = csv.model_;
}

CSVFile::CSVFile(QString path)
{
    loadFromFile(path);
}

QStandardItemModel *CSVFile::model() const
{
    return model_;
}

void CSVFile::setModel(QStandardItemModel *model)
{
    model_ = model;
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
    return descriptorsNames_;
}

void CSVFile::setDescriptorsNames(const QStringList &descriptorsNames)
{
    descriptorsNames_ = descriptorsNames;
}

QChar CSVFile::delim() const
{
    return delim_;
}

void CSVFile::setDelim(const QChar &delim)
{
    delim_ = delim;
}

QMap<QString, QVector<qreal> > CSVFile::objects() const
{
    return objects_;
}

void CSVFile::setObjects(const QMap<QString, QVector<qreal> > &objects)
{
    objects_ = objects;
}

CSVFile &CSVFile::operator=(const CSVFile &csv)
{
    if( this == &csv)
        return *this;

    if( checkPath(csv.path_) )
        path_ = csv.path_;
    descriptorsNames_ = csv.descriptorsNames_;
    objects_ = csv.objects_;
    delim_ = csv.delim_;
    model_ = csv.model_;

    return *this;
}

void CSVFile::loadFromFile(QString path)
{
    if( checkPath(path) )
    {
        setPath(path);

        QFile csvFile(path);
        csvFile.open(QFile::ReadOnly | QFile::Text);
        QTextStream in(&csvFile);
        QString fileContentStr = in.readAll();

        //FIXME добавить автоматическое определение разделителя файла
        //QChar delim = detectDelim(fileContentStr);
        QChar delim(';');
        setDelim(delim);

        QStandardItemModel *mod = new QStandardItemModel;

        QStringList rows = fileContentStr.split('\n');
        QStringList firstRowCols = rows.at(0).split(delim);
        firstRowCols.removeFirst();
        setDescriptorsNames(firstRowCols);

        for(QString headerStr : firstRowCols)
        {
            headerStr.insert(headerStr.count() / 2, '\n');
            QStandardItem *hHItem = new QStandardItem(headerStr);
            makeHeader(hHItem, Qt::Horizontal);
            mod->setHorizontalHeaderItem(mod->columnCount(), hHItem);
        }

        rows.removeFirst();
        QMap< QString, QVector<qreal> > objcts;
        QString objName;
        for(QString r : rows)
        {
            QList<QStandardItem*> objectAtR;
            QStringList curRow = r.split(delim);
            QVector<qreal> rVals;
            objName = curRow.first();
            curRow.removeFirst();
            for(QString d : curRow)
            {
                QStandardItem *item = new QStandardItem(d);
                setAllTextData(item);
                item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

                d.replace( QChar(','), QChar('.') );

                rVals << d.toDouble();
                objectAtR << item;
            }
            objcts[objName] = rVals;
            mod->appendRow(objectAtR);

            QStandardItem *vHItem = new QStandardItem(objName);
            makeHeader(vHItem, Qt::Vertical);
            mod->setVerticalHeaderItem(mod->rowCount() - 1, vHItem);
        }
        setObjects(objcts);
        setModel(mod);
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
    correctFile = ( testFile.exists() && testFile.isFile() ) ? true : false;
    return correctFile;
}

void CSVFile::makeHeader(QStandardItem *item, Qt::Orientation orient)
{
    Qt::AlignmentFlag al;
    setAllTextData(item);
    QFont f = item->font();
    f.setBold(true);
    item->setData(f, Qt::FontRole);
    al = (orient == Qt::Vertical) ? Qt::AlignRight : Qt::AlignCenter;
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
