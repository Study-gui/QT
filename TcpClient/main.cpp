#include "tcpclient.h"
#include"sharefile.h"
#include <QApplication>
// #include<friend.h>
// #include<online.h>
// #include<opewidget.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // tcpclient w;
    // w.show();

    //单例模式
    tcpclient::getInstance().show();

    // ShareFile w;
    // w.show();
    // OpeWidget w;
    // w.show();

    // // Online w;
    // // w.show();

    // // Friend w;
    // // w.show();

    return a.exec();
}
