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

    path_ = csv.path_;
    descriptorsNames_ = csv.descriptorsNames_;
    objects_ = csv.objects_;
    delim_ = csv.delim_;
    model_ = csv.model_;

    return *this;
}
