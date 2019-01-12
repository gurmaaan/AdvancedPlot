#ifndef AXISWIDGET_H
#define AXISWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QDebug>
#include <algorithm>

namespace Ui {
class AxisWidget;
}

/**
 * @brief      Class for axis type.
 */
enum class AxisType
{
    XAxis,
    YAxis,
    dYAxis,
    dXAxis
};

/**
 * @brief      Class for axis widget.
 */
class AxisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AxisWidget(QWidget *parent = nullptr);
    ~AxisWidget();

    /**
     * @brief      Sets the name.
     *
     * @param[in]  name  The name
     */
    void setName(QString name);

    /**
     * @brief      Sets the clr button visible.
     *
     * @param[in]  state  The state
     */
    void setClrBtnVisible(bool state);
    void setVisibleOnPlotBtnVisible(bool state);
    bool isVisibleOnPlot();

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    QVector<double> values() const;
    void setValues(const QVector<double> &values);

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    AxisType t() const;
    void setT(const AxisType &t);

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    double min() const;
    void setMin(double min);

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    double max() const;
    void setMax(double max);

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    double av() const;
    void setAv(double av);

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    QString name();
    int curentIndex();

    /**
     * @brief      { function_description }
     *
     * @return     { description_of_the_return_value }
     */
    QColor color() const;
    void setColor(const QColor &color);

    /**
     * @brief      Sets the current ind.
     *
     * @param[in]  index  The index
     */
    void setCurInd(int index);

signals:
    void currentIndexChanged(int index);
    void visibleChanged(bool state);

public slots:
    void setModel(QStandardItemModel *model);
    void on_axis_cb_currentIndexChanged(int index);

private slots:

    /**
     * @brief      { function_description }
     *
     * @param[in]  checked  The checked
     */
    void on_axis_gb_clicked(bool checked);

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
