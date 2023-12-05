#include "form.h"
#include "ui_form.h"
#include<QDebug>
Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::on_toolButton_triggered(QAction *arg1)
{
    qDebug()<<"杨职业是sb";
}


void Form::on_toolButton_clicked()
{
     qDebug()<<"杨职业是sb";
}

