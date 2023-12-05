#include "widget.h"
#include<form.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    Form w1;
    w1.show();
    return a.exec();
}
