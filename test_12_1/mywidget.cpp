#include "mywidget.h"
#include<QPushButton>

MyWidget::MyWidget(QWidget *parent)
    : QWidget{parent}
{
     QPushButton* btn2=new QPushButton("按钮二",this);
     this->resize(600,600);
     this->setWindowTitle("窗口二");
     //Widget* widget1=new Widget;
     connect(btn2,&QPushButton::clicked,[=](){
         //点击按钮二，释放自定义信号
         // widget1->show();
         // this->hide();
         this->btnclick();

     });

}
