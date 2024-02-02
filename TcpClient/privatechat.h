#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <QWidget>
#include<QString>
#include<protocol.h>
#include<tcpclient.h>

namespace Ui {
class PrivateChat;
}

class PrivateChat : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChat(QWidget *parent = nullptr);
    ~PrivateChat();
    //赋值双方用户名的函数
    void setChatName(QString m_Name);
    //聊天用户名
    QString m_Usrname;
    //聊天对象用户名
    QString m_strChatname;
    //单例模式
    static PrivateChat& getInstance();

    //显示私聊信息
    void updateMsg(PDU* pdu);
    //显示自己的信息
    void ShowChat(PDU* pdu);

private slots:
    void on_sendMsg_pb_clicked();

private:
    Ui::PrivateChat *ui;
};

#endif // PRIVATECHAT_H
