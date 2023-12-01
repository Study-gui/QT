#ifndef WIDGET_H
#define WIDGET_H
#include<mywidget.h>
#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    //MyWidget* mywidget;
    Widget(QWidget *parent = nullptr);
    ~Widget();
};
#endif // WIDGET_H
