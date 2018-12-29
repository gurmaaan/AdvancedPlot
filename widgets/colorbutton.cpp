#include "colorbutton.h"
#include "ui_colorbutton.h"

ColorButton::ColorButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorButton)
{
    ui->setupUi(this);
    setColor(QColor(Qt::white));
}

ColorButton::~ColorButton()
{
    delete ui;
}

void ColorButton::on_toolButton_clicked()
{
    QColor clr = QColorDialog::getColor(Qt::white, this);
    if( clr.isValid() )
        setColor(clr);
    else {
        setColor(QColor(Qt::white));
    }
}

void ColorButton::genIcon(QColor clr)
{
    QPixmap icon(QSize(16,16));
    icon.fill(clr);
    ui->toolButton->setIcon(QIcon(icon));
}

QColor ColorButton::color() const
{
    return color_;
}

void ColorButton::setColor(const QColor &color)
{
    color_ = color;
    genIcon(color);
}

void ColorButton::setText(QString text)
{
    ui->label->setText(text);
}
