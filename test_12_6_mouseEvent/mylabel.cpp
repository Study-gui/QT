#include "mylabel.h"
#include<QMouseEvent>
#include<QString>
Mylabel::Mylabel(QWidget *parent) : QLabel{parent}
{
   //鼠标不会一开始，窗口不会随着鼠标移动
   //要进行点击，才能跟随，
    //所以要进行初始化
    this->setMouseTracking(true);
}

void Mylabel::mouseMoveEvent(QMouseEvent *ev)
{

    //获取鼠标x坐标
    int x=ev->x();
    //获取鼠标y坐标
    int y=ev->y();
    //获取鼠标按键
    Qt::MouseButtons btns=ev->buttons();
    QString strbtn="";
    //因为leftButton为00001，right为00010，所以要用&操作来判断是否同时按下按键
    if(btns & Qt::LeftButton)
    {
        strbtn+="LeftButton: ";
    }
    if(btns & Qt::RightButton)
    {
        strbtn+="RightButton: ";
    }
    if(btns & Qt::MiddleButton)
    {
        strbtn+="MiddleButton: ";
    }
    //利用html格式,h1表示加粗，center表示居中
    QString str=QString("<h1><center>[Move][%1,%2][%3]</center></h1>").arg(x).arg(y).arg(strbtn);
    this->setText(str);
}

void Mylabel::mousePressEvent(QMouseEvent *ev)
{
    //获取鼠标x坐标
    int x=ev->x();
    //获取鼠标y坐标
    int y=ev->y();
    //获取鼠标按键
    Qt::MouseButtons btns=ev->buttons();
    QString strbtn="";
    if(btns & Qt::LeftButton)
    {
        strbtn+="LeftButton: ";
    }
    if(btns & Qt::RightButton)
    {
        strbtn+="RightButton: ";
    }
    if(btns & Qt::MiddleButton)
    {
        strbtn+="MiddleButton: ";
    }
    //利用html格式,h1表示加粗，center表示居中
    QString str=QString("<h1><center>[Press][%1,%2][%3]</center></h1>").arg(x).arg(y).arg(strbtn);
    this->setText(str);
}

