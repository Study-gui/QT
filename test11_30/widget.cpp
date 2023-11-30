#include "widget.h"
#include "ui_widget.h"
#include<QPushButton>
#include<QString>
#include<QDebug>
void Widget::classIsover()
{
   emit pTeacher->hungry();
   emit  pTeacher->hungry("黄焖鸡");
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QPushButton* btn=new QPushButton(this);
    btn->setText("按钮一");
    //信号与槽 1 信号发起者 2 信号 3 信号接收者 4 槽：操作的动作

    //connect(btn,&QPushButton::clicked,this,&Widget::close);//实现点击关闭功能
    //案例一 ，下课了，老师饿了，学生请老师吃饭

    //实例化对象
    pTeacher=new Teacher;
    pStudent=new Student;

    //connect(pTeacher,&Teacher::hungry,pStudent,&Student::treat);


    //案例二，有重载函数的自定义信号与槽
    //方法一 利用函数指针消除 二义性
    void (Teacher::*teacher_string)(QString)=&Teacher::hungry;
    void (Student::*student_string)(QString)=&Student::treat;

    //connect(pTeacher,teacher_string,pStudent,student_string);

    //方法二利用static_cast消除二义性
    // connect(pTeacher,
    //         static_cast<void (Teacher::*)(QString)>(&Teacher::hungry),
    //         pStudent,
    //         static_cast<void (Student::*)(QString)>(&Student::treat));
    // disconnect(pTeacher,
    //            static_cast<void (Teacher::*)(QString)>(&Teacher::hungry),
    //            pStudent,
    //            static_cast<void (Student::*)(QString)>(&Student::treat));
    //this->classIsover();
    //connect(btn,&QPushButton::clicked,pTeacher,static_cast<void (Teacher::*)()>(&Teacher::hungry));
    int a=10;
    int b=20;
    //lambda表达式时匿名函数对象，[](){}
    [=](){
        qDebug()<<a<<b;
    }();
    connect(btn,&QPushButton::clicked,[=](){
        this->close();
        qDebug()<<a<<b;
    });

 this->classIsover();
}
Widget::~Widget()
{
    delete ui;
}
