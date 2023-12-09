#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
#include<QPainter>
#include<QPen>
#include<QBrush>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    //设置画家
    QPainter painter(this);
    //设置画家偏移，使画家画的图形进行整体移动
    painter.translate(200,0);
    //设置画笔
    QPen  pen;
    pen.setColor(QColor(Qt::blue));
    //设置笔宽
    pen.setWidth(3);
    //设置画笔风格
    pen.setStyle(Qt::DashLine);

    //画家设置画笔，应用这只画笔
    painter.setPen(pen);

    //设置画刷
    QBrush brush;
    brush.setColor(QColor(Qt::yellow));
    //默认情况下，画刷不提供画，还得提供风格
    brush.setStyle(Qt::FDiagPattern);
    //画家设置画刷,只给封闭图形填充
    painter.setBrush(brush);

    //画出图形,画出直线
    painter.drawLine(0,0,200,200);

    //画一个长方形
    painter.drawRect(50,50,50,100);

    //在画一个圆
    painter.drawEllipse(100,100,50,50);

    //添加文字
    painter.drawText(300,300,"你好");






}
