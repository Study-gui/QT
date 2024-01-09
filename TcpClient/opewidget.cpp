#include "opewidget.h"

OpeWidget::OpeWidget(QWidget *parent)
    : QWidget{parent}
{
    m_pListW=new QListWidget(this);

    m_pListW->addItem("好友");
    m_pListW->addItem("图书");

    m_pFriend=new Friend;
    m_pBook=new Book;


     m_Psw=new QStackedWidget;

    m_Psw->addWidget(m_pFriend);
    m_Psw->addWidget(m_pBook);

    QHBoxLayout* pMain=new QHBoxLayout;
    pMain->addWidget(m_pListW);
    pMain->addWidget(m_Psw);


    setLayout(pMain);

    //把listWidget窗口与stackd窗口相连接
    connect(m_pListW,SIGNAL(currentRowChanged(int)),m_Psw,SLOT(setCurrentIndex(int)));
}

//设置为单例模式
OpeWidget &OpeWidget::getInstance()
{
    static OpeWidget instance;
    return instance;
}

Friend *OpeWidget::getFriend()
{
    return m_pFriend;
}
