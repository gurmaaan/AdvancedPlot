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

/**
 * @brief      Class for csv file.
 */
class CSVFile : public QObject
{
    Q_OBJECT
public:
    explicit CSVFile(QObject *parent = nullptr);
    CSVFile(const CSVFile &csv, QObject *parent = nullptr);
    CSVFile(QString pathToOpen);
    CSVFile(QString pathToSave, QStandardItemModel *model);

    /**
     * @brief      Получение строки файла по имени объекта
     *
     * @param[in]  objName  Имя объекта
     *
     * @return     Целую строку файла на позиции с именем объекта objName
     */
    QString getObjStr(QString objName);

    /**
     * @brief      Получение модели файла
     *
     * @return     Возвращает значения содержащиеся в файле в виде QStandardItemModel
     */
    QStandardItemModel *model() const;
    void setModel(QStandardItemModel *model);

    /**
     * @brief      Сохранение файла
     *
     * @param[in]  Полный путь к файлу после сохранения
     */
    void saveToFile(QString pathToSave = "");

    /**
     * @brief      Установка пути к файлу объектов
     *
     * @return     Путь к файлу объектов
     */
    QString path() const;
    void setPath(const QString &path);

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    QStringList descriptorsNames() const;
    QStringList objNames() const;

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    QString delim() const;
    void setDelim(const QChar &delim);

    /**
     * @brief      { operator_description }
     *
     * @param[in]  csv   The csv
     *
     * @return     { description_of_the_return_value }
     */
    CSVFile &operator=(const CSVFile &csv);

    /**
     * @brief      Gets the f rf cstr.
     *
     * @return     The f rf cstr.
     */
    QString getFRfCstr() const;
    void setFRfCstr(const QString &frfcstr);

    /**
     * @brief      Makes a header.
     *
     * @param      item    The item
     * @param[in]  orient  The orient
     */
    static void makeHeader(QStandardItem *item, Qt::Orientation orient);
    static void setAllTextData(QStandardItem* item);

signals:

public slots:

private:
    QString path_;
    QChar delim_;
    QStandardItemModel *model_;
    QString fRfCstr_;

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    QString fileHeaderStr();

    /**
     * @brief      Loads a from file.
     *
     * @param[in]  path  The path
     */
    void loadFromFile(QString path);
    bool checkPath(QString path);
    bool extensionValid(QString path);
    bool fileExists(QString path);

    /**
     * @brief      { function_description }
     *
     * @param[in]  fileContentStr  The file content string
     *
     * @return     { description_of_the_return_value }
     */
    QChar detectDelim(QString fileContentStr);
};

#endif // CSVFILE_H
