#ifndef AXISWIDGET_H
#define AXISWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QDebug>
#include <algorithm>

namespace Ui {
class AxisWidget;
}

enum class AxisType
{
    XAxis,
    YAxis,
    dYAxis,
    dXAxis
};

class AxisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AxisWidget(QWidget *parent = nullptr);
    ~AxisWidget();

    void setName(QString name);
    void setClrBtnVisible(bool state);
    QVector<double> values() const;
    void setValues(const QVector<double> &values);

    AxisType t() const;
    void setT(const AxisType &t);

    double min() const;
    void setMin(double min);

    double max() const;
    void setMax(double max);

    double av() const;
    void setAv(double av);

    QString name();

    QColor color() const;
    void setColor(const QColor &color);

signals:
    void currentIndexChanged(int index);
public slots:
    void setModel(QStandardItemModel *model);
    void on_axis_cb_currentIndexChanged(int index);

private slots:


private:
    Ui::AxisWidget *ui;
    AxisType t_;
    QStandardItemModel *model_;
    QVector<double> values_;
    QColor color_;
    void typeProxy(AxisType type);
    double min_;
    double max_;
    double av_;
};

#endif // AXISWIDGET_H
