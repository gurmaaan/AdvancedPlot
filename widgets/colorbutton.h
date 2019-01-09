#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QWidget>
#include <QColorDialog>

namespace Ui {
class ColorButton;
}

class ColorButton : public QWidget
{
    Q_OBJECT

public:
    explicit ColorButton(QWidget *parent = nullptr);
    ~ColorButton();

    QColor color() const;

public slots:
    void setText(QString text);
    void setColor(const QColor &color);

private slots:
    void on_toolButton_clicked();

private:
    Ui::ColorButton *ui;
    QColor color_;
    void genIcon(QColor clr);
};

#endif // COLORBUTTON_H
