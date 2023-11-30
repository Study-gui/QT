#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<student.h>
#include<teacher.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    //创建学生和老师对象
    Student* pStudent;
    Teacher* pTeacher;
    //创建老师饿了的函数
    void classIsover();
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
