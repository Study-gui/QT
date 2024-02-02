#ifndef OPEWIDGET_H
#define OPEWIDGET_H

#include <QWidget>
#include<QListWidget>
#include<friend.h>
#include<online.h>
#include<book.h>
#include<QStackedWidget>

class OpeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpeWidget(QWidget *parent = nullptr);
    //设置为单例模式
    static OpeWidget &getInstance();
    //创建返回friend对象
    Friend *getFriend();
    Book* getBook();
signals:

private:
    QListWidget* m_pListW;

    Friend* m_pFriend;
    //Online* m_pOnline;
    Book* m_pBook;
    QStackedWidget* m_Psw;

};

#endif // OPEWIDGET_H
