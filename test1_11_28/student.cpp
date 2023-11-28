#include "student.h"

Student::Student(QWidget *parent)
    : QWidget{parent}
{}
void treat()
{
    qDebug()<<"学生请老师吃饭";
}

//重载版本
void treat(QString what)
{
    qDebug()<<"学生请老师吃"<<what;
}
