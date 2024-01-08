#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include"protocol.h"
#include<opedb.h>
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = nullptr);
    //获取是由用户名函数
    QString getName();
signals:
    //客户端下线信号
    void Offline(MyTcpSocket* mysocket);

public slots:
    //客户端下线槽函数,修改数据库状态
    void  clientOffine();
public slots:
    //服务器接收数据槽函数
    void recvMsg();
private:
    //客户端用户名
    QString m_strName;
};

#endif // MYTCPSOCKET_H
