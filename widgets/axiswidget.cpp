#include "axiswidget.h"
#include "ui_axiswidget.h"

AxisWidget::AxisWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AxisWidget)
{
    ui->setupUi(this);
}

AxisWidget::~AxisWidget()
{
    delete ui;
}

void AxisWidget::setName(QString name)
{
    ui->axis_lbl->setText(name);
}

void AxisWidget::setClrBtnVisible(bool state)
{
    ui->clr_btn->setVisible(state);
}

void AxisWidget::on_axis_cb_currentIndexChanged(int index)
{
    QList<QStandardItem*> column = model_->takeColumn(index);
    QVector<double> valVec;
    for(QStandardItem* item : column)
        valVec << item->data(Qt::UserRole).toDouble();

    setValues(valVec);
}

void AxisWidget::typeProxy(AxisType type)
{
    QString axisName = "";
    switch (type)
    {
        case AxisType::XAxis:
        {
            axisName = "X: ";
            setClrBtnVisible(false);
            break;
        }
        case AxisType::YAxis:
        {
            axisName = "Y: ";
            setClrBtnVisible(false);
            break;
        }
        case AxisType::dXAxis:
        {
            axisName = "dx :";
            setClrBtnVisible(true);
            break;
        }
        case AxisType::dYAxis:
        {
            axisName = "dy :";
            setClrBtnVisible(true);
            break;
        }
    }
    setName(axisName);
}

double AxisWidget::av() const
{
    return av_;
}

void AxisWidget::setAv(double av)
{
    av_ = av;
    ui->av_le->setText(QString::number(av));
}

QColor AxisWidget::getColor()
{
    if(ui->clr_btn->isVisible())
        return ui->clr_btn->color();
    else {
        return QColor(Qt::white);
    }
}

double AxisWidget::max() const
{
    return max_;
}

void AxisWidget::setMax(double max)
{
    max_ = max;
    ui->max_le->setText(QString::number(max));
}

double AxisWidget::min() const
{
    return min_;
}

void AxisWidget::setMin(double min)
{
    min_ = min;
    ui->min_le->setText(QString::number(min));
}

AxisType AxisWidget::t() const
{
    return t_;
}

void AxisWidget::setT(const AxisType &t)
{
    t_ = t;
    typeProxy(t);
}

void AxisWidget::setModel(QStandardItemModel *model)
{
    model_ = model;
    QStringList modelHHeaders;
    for(int i = 0; i < model->columnCount(); i++)
        modelHHeaders << model->headerData(i, Qt::Horizontal).toString();
    ui->axis_cb->addItems(modelHHeaders);
}

QVector<double> AxisWidget::values() const
{
    return values_;
}

void AxisWidget::setValues(const QVector<double> &values)
{
    values_ = values;
    double min = *std::min_element(values.constBegin(), values.constEnd());
    setMin(min);
    double max = *std::max_element(values.constBegin(), values.constEnd());
    setMax(max);
    setAv((min + max) / 2);
}