#include "widget.h"
#include "ui_widget.h"
#include<QMessageBox>
#include<QInputDialog>
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

void Widget::on_pushButton_clicked()
{
    QMessageBox::information(this,"值",QString::number(this->ui->widget1->getvalue()));
}


void Widget::on_pushButton_2_clicked()
{
    // int value;
    // QInputDialog()>>value;
    ui->widget1->setvalue(50);
}

