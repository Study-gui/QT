#include "widget.h"
#include<QPushButton>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //QPushButton* btn=new QPushButton(this);
    //btn->setText("按钮一");
    //利用信号与槽
    //connect(btn,&QPushButton::clicked,this,&Widget::close);
    //信号与槽 案列一
    pTeacher=new Teacher(this);
    pStudent=new Student(this);

   // connect(pTeacher,&Teacher::hungry,pStudent,&Student::treat);
    //重载版本,利用函数指针进行取消系统二义性
    void (Teacher::*teacher_qstring)(QString)=&Teacher::hungry;
    void (Student::*student_qstring)(QString)=&Student::treat;

    connect(pTeacher,teacher_qstring,pStudent,student_qstring);
    //利用static_cast解决/*
       // connect(
        //pTeacher,
        //static_cast<void (Teacher::*)(QString)>(&Teacher::hungry),
        //pStudent,
        //static_cast<void (Student::*)(QString)>(&Student::treat));*/

     this->classIsover();
}
void Widget::classIsover()
{
    emit pTeacher->hungry();
    //重载版本
   emit pTeacher->hungry("黄焖鸡");

}

Widget::~Widget() {}
