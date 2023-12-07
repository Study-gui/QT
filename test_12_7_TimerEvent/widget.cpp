#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

}

Widget::~Widget()
{
    this->timerId1=0;
    this->timerId2=0;
    delete ui;
}

void Widget::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==this->timerId1)
    {
        static int num1=1;
        ui->lcdNumber->display(num1++);
    }
    if(event->timerId()==this->timerId2)
    {
        static int num2=1;
        ui->lcdNumber_2->display(num2++);
    }
}

void Widget::on_pushButtonstart_clicked()
{
    this->timerId1=startTimer(1000);

}


void Widget::on_pushButton_stop_clicked()
{
    killTimer(this->timerId1);
     this->timerId1=0;
}


void Widget::on_pushButtonstart_2_clicked()
{
    this->timerId2=startTimer(100);

}


void Widget::on_pushButton_stop_2_clicked()
{
    killTimer(this->timerId2);
    this->timerId2=0;
}

