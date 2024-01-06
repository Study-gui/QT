#include "mytcpserver.h"
#include<QDebug>
MyTcpServer::MyTcpServer() {}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    //将socket传输的数据进行存入
    qDebug()<<"new Client connected";
    MyTcpSocket *pTcpSocket=new MyTcpSocket;
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_tcpSocketList.append(pTcpSocket);
}


