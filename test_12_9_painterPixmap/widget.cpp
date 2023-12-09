#include "widget.h"
#include "ui_widget.h"
#include<QPainter>
#include<QPixmap>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    mpox=0;
}
void Widget::paintEvent(QPaintEvent *event)
{
    //创建画家
    QPainter painter(this);
   // QPixmap hhh()
    //QPixmap pixmap(":/new/prefix1/C:/Users/2814554384/Pictures/timg.jpg");
    QPixmap pixmap(":/new/prefix1/C:/Users/2814554384/Pictures/Saved Pictures/屏幕截图 2023-07-11 115202.png");

    painter.drawPixmap(mpox,0,pixmap);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    mpox+=10;

    //更新绘图事件
    this->update();
}

