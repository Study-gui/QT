#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include<QListWidget>
#include<QLineEdit>
#include<QPushButton>
#include<QTextEdit>
//#include<QLayout>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<online.h>
#include<protocol.h>
class Friend : public QWidget
{
    Q_OBJECT
public:
    explicit Friend(QWidget *parent = nullptr);
    //查看好友函数
    void ShowAllOnline(PDU* pdu);
    //创建返回Online对象函数
    Online *getOnline();
private:
    QTextEdit* m_pShowMsgTE;  //文本消息框
    QListWidget* m_pFriendListWidget;  //好友列表窗口
    QLineEdit* m_pInputMsgLE;  //输入信息框

    QPushButton *m_pDelFriendPB;   //删除好友按钮
    QPushButton *m_pFlushFriendPB;  //刷新好友按钮
    QPushButton *m_pShowOnlineUsrPB;   //显示在线用户按钮
    QPushButton *m_pSearchUsrPB;      //查找用户按钮
    QPushButton *m_pMsgSendPB;        //发送消息按钮
    QPushButton *m_PrivateChatPB;     //私聊按钮

    Online* m_pShowOnline;

signals:
public slots:

    //触发显示好友信号的槽
    void ShowOnline();

};

#endif // FRIEND_H
