#include "widget.h"
#include<QPushButton>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    this->setWindowTitle("窗口一");
    this->resize(400,400);
    QPushButton* btn1=new QPushButton("按钮一",this);
    //错误做法，这个是一个窗口，不会有切换的页面
    // QPushButton* btn2=new QPushButton("按钮二",this);
    // btn2->move(400,400);
    // connect(btn1,&QPushButton::clicked,[=](){
    //     btn1->hide();
    //     btn2->show();

    // });
    // connect(btn2,&QPushButton::click,[=](){
    //      btn2->hide();
    //     btn1->show();

    // });

    //mywidget->show();
    MyWidget* mywidget=new MyWidget;
    connect(btn1,&QPushButton::clicked,[=](){
        //点击窗口一，显示窗口二
        mywidget->show();
        //隐藏窗口
        //mywidget->hide();
        connect(mywidget,&MyWidget::btnclick,[=](){
            //收到按钮二的自定义信号，就隐藏窗口二
            this->show();
            mywidget->hide();
        });
        this->hide();
    });



}

Widget::~Widget() {}
