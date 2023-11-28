#ifndef WIDGET_H
#define WIDGET_H
#include<teacher.h>
#include<student.h>
#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Teacher* pTeacher;
    Student* pStudent;
    void classIsover();
    Widget(QWidget *parent = nullptr);
    ~Widget();

};
#endif // WIDGET_H
