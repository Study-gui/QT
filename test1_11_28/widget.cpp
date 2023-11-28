#include "widget.h"
#include<QPushButton>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //利用new创建一个QpushButton类来进行设置按钮
    // QPushButton* btn=new QPushButton(this);
    // btn->setText("按钮一");

    //利用信号和槽进行连接关系
    //1 信号发送者
    //2 信号
    //3 信号接收者
    //4 槽：信号的处理的动作
    //实现点击按钮关闭窗口
    // connect(btn,&QPushButton::clicked,this,&Widget::close);

    // ui->setupUi(this);
    //自定义信号和槽，请老师吃饭
    //信号发送者 老师
    //信号 老师饿了
    //信号接收者 学生
    //槽：请吃饭
    pTeacher=new Teacher(this);
    pStudent=new Student(this);
    //connect(pTeacher,&pTeacher::hungry,pStudent,&pStudent::treat);

    //this->classIsover();
    //释放老师饿了的信号
    //重载版本,用connect系统会有二义性出现
    //解决方法一：利用函数指针进行函数的重载赋值
    void(Teacher::*teacher_qString)(QString)=&pTeacher->hungry;

    void(Student::*student_qString)(QString)=&pStudent->treat;

    connect(pTeacher,teacher_qString,pStudent,student_qString);

    //pTeacher->hungry();
    this->classIsover();

}

void Widget::classIsover()
{
    //老师饿了
    emit pTeacher->hungry();
    //重载版本，老师要吃黄焖鸡
    emit pTeacher->hungry("黄焖鸡");
}
Widget::~Widget() {}
