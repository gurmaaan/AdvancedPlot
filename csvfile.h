#ifndef CSVFILE_H
#define CSVFILE_H

#include <QObject>
#include <QMap>
#include <QFileInfo>
#include <QChar>
#include <QFileDialog>
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
    CSVFile(QString pathToOpen);
    CSVFile(QString pathToSave, QStandardItemModel *model);

    QString getObjStr(QString objName);

    QStandardItemModel *model() const;
    void setModel(QStandardItemModel *model);

    void saveToFile(QString pathToSave = "");
    QString path() const;
    void setPath(const QString &path);

    QStringList descriptorsNames() const;
    QStringList objNames() const;

    QString delim() const;
    void setDelim(const QChar &delim);

    CSVFile &operator=(const CSVFile &csv);

    QString getFRfCstr() const;
    void setFRfCstr(const QString &frfcstr);

    static void makeHeader(QStandardItem *item, Qt::Orientation orient);
    static void setAllTextData(QStandardItem* item);

signals:

public slots:

private:
    QString path_;
    QChar delim_;
    QStandardItemModel *model_;
    QString fRfCstr_;

    QString fileHeaderStr();

    void loadFromFile(QString path);
    bool checkPath(QString path);
    bool extensionValid(QString path);
    bool fileExists(QString path);

    QChar detectDelim(QString fileContentStr);
};

#endif // CSVFILE_H
