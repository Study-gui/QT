#ifndef MYWIDGET_H
#define MYWIDGET_H
#include<widget.h>
#include <QWidget>

class MyWidget : public QWidget
{
    Q_OBJECT
public:

    explicit MyWidget(QWidget *parent = nullptr);

signals:
    void btnclick();
};

#endif // MYWIDGET_H
