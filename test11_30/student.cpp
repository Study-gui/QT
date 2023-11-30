#include "student.h"
#include<QDebug>
Student::Student(QWidget *parent)
    : QWidget{parent}
{}

void Student::treat()
{
    qDebug()<<"学生请老师吃饭";
}

void Student::treat(QString what)
{
    qDebug()<<"学生请老师吃"<<what;
}
