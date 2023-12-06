#ifndef MYLABEL_H
#define MYLABEL_H

#include <QWidget>
#include<QLabel>
class Mylabel : public QLabel
{
    Q_OBJECT
public:
    explicit Mylabel(QWidget *parent = nullptr);
protected:
    //重写鼠标移动函数
    void mouseMoveEvent(QMouseEvent *ev);
    //重写鼠标处理函数
    void mousePressEvent(QMouseEvent *ev);

signals:
};

#endif // MYLABEL_H
