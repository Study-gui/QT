#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QVector>
#include<QString>
#include<student.h>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QMessageBox>
#include<opensql.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:

    //显示数据
    void showstudet();
    //存入数据集合
    QVector<Student> p;
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QSqlDatabase m_mysql;

    //操作数据库函数
    void init();
private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
