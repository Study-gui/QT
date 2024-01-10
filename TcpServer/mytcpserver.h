#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include "tcpserver.h"
#include<QTcpServer>
#include<QList>
#include<mytcpsocket.h>
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer();

    static MyTcpServer &getInstance();
    //转发信息的函数
    void reSend(const char* name,PDU* pdu);
public slots:
    //客户端下线槽函数
    void deleteSocket(MyTcpSocket* mysocket);
protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    QList<MyTcpSocket*> m_tcpSocketList;
};

#endif // MYTCPSERVER_H
