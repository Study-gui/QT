#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QWidget>
#include<QPushButton>
class Mypushbutton : public QPushButton
{
    Q_OBJECT
public:
    Qt::GlobalColor color;
    explicit Mypushbutton(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *e);
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

signals:
};

#endif // MYPUSHBUTTON_H
