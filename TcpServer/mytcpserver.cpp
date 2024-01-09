#include "mytcpserver.h"
#include<QDebug>
MyTcpServer::MyTcpServer() {}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    for(QList<MyTcpSocket*>::iterator it=m_tcpSocketList.begin();it!=m_tcpSocketList.end();it++)
    {
        if(*it==mysocket)
        {
            //先将链表中的节点new出来的空间进行释放，再将这个链表的节点进行删除
            // delete *it;
            (*it) -> deleteLater();
            *it=NULL;
            m_tcpSocketList.erase(it);
            //qDebug()<<"deletesocket";
            break;
        }
    }
    //qDebug()<<"delesocket";
    for(int i=0;i<m_tcpSocketList.size();i++)
    {

        qDebug()<<m_tcpSocketList.at(i)->getName();
    }
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    //将socket传输的数据进行存入
    qDebug()<<"new Client connected";
    MyTcpSocket *pTcpSocket=new MyTcpSocket;
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_tcpSocketList.append(pTcpSocket);
    //触发客户端下线型号与槽
    connect(pTcpSocket,SIGNAL(Offline(MyTcpSocket*)),this,SLOT(deleteSocket(MyTcpSocket*)));
}


