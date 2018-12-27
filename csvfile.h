#ifndef CSVFILE_H
#define CSVFILE_H

#include <QObject>
#include <QMap>
#include <QFileInfo>
#include <QChar>
#include <QDebug>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTextStream>
#include <QTextCodec>

#include "static.h"

class CSVFile : public QObject
{
    Q_OBJECT
public:
    explicit CSVFile(QObject *parent = nullptr);
    CSVFile(const CSVFile &csv, QObject *parent = nullptr);
    CSVFile(QString path);

    QStandardItemModel *model() const;
    void setModel(QStandardItemModel *model);

    QString path() const;
    void setPath(const QString &path);

    QStringList descriptorsNames() const;
    void setDescriptorsNames(const QStringList &descriptorsNames);

    QChar delim() const;
    void setDelim(const QChar &delim);

    QMap<QString, QVector<qreal> > objects() const;
    void setObjects(const QMap<QString, QVector<qreal> > &objects);

    CSVFile &operator=(const CSVFile &csv);

signals:

public slots:

private:
    QString path_;
    QStringList descriptorsNames_;
    QChar delim_;
    QMap< QString, QVector<qreal> > objects_;
    QStandardItemModel *model_;

    void loadFromFile(QString path);
    bool checkPath(QString path);
    bool extensionValid(QString path);
    bool fileExists(QString path);

    void makeHeader(QStandardItem *item, Qt::Orientation orient);
    void setAllTextData(QStandardItem* item);
    QChar detectDelim(QString fileContentStr);
};

#endif // CSVFILE_H
