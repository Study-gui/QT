#include "mypushbutton.h"
#include<QPainter>
#include<QBrush>
#include<QMouseEvent>
Mypushbutton::Mypushbutton(QWidget *parent)
    : QPushButton{parent}
{
    //默认红色
    color=Qt::red;
}

void Mypushbutton::paintEvent(QPaintEvent *e)
{
    //创建画家
    QPainter painter(this);

    //创建画刷
    QBrush brush(color,Qt::SolidPattern);
    painter.setBrush(brush);

    //创建椭圆
    painter.drawEllipse(0,0,this->width()-1,this->height()-1);

    //画出按钮
    painter.drawText(0,0,this->width()-1,this->height()-1,Qt::AlignVCenter|Qt::AlignHCenter,this->text());

}

void Mypushbutton::enterEvent(QEnterEvent *event)
{
    //鼠标进来变成绿色
    color=Qt::green;
    this->update();
}

void Mypushbutton::leaveEvent(QEvent *event)
{
    //鼠标离开变成红色
    color=Qt::red;
    this->update();
}

void Mypushbutton::mousePressEvent(QMouseEvent *e)
{
    //鼠标点击变成蓝色
    if(e->button()!=Qt::LeftButton)
    {
        return ;
    }
    color=Qt::blue;
    this->update();
    //信号依赖于事件，mypushbutton有这个事件,手动发送QpushButton的事件，系统会产生信号与驱动槽
    QPushButton::mousePressEvent(e);
}

void Mypushbutton::mouseReleaseEvent(QMouseEvent *e)
{
    //鼠标松开变成绿色
    color=Qt::green;
    this->update();

    QPushButton::mouseReleaseEvent(e);

}
