#include "widget.h"

#include <QApplication>
#include<QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    //默认构造
    QPushButton btn1;
    btn1.setText("按钮一");
    btn1.setParent(&w);
    //直接构造
    QPushButton btn2("按钮二",&w);
    //移动位置
    btn2.move(100,100);
    //重新设置窗口大小
    btn2.resize(400,400);
    //按钮二与按钮三建立父子关系
    QPushButton btn3("按钮三",&btn2);
    //按钮三相对于按钮二移动位置
    btn3.move(100,200);

    //父亲窗口相对于屏幕移动位置
    w.move(400,100);
    w.setWindowTitle("hello world");
    w.show();
    return a.exec();
}
