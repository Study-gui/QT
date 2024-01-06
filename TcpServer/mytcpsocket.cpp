#include "mytcpsocket.h"
#include<QDebug>
MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket{parent}
{
    //socket接收成功会自动发送readyRead()函数
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));
}

void MyTcpSocket::recvMsg()
{
    //当前读到的数据大小(字节)
    qDebug()<<this->bytesAvailable();
    uint uiPDULen=0;
    this->read((char*)&uiPDULen,sizeof(uint));
    uint uiMsgLen=uiPDULen-sizeof(PDU);
    PDU* pdu=mkPDU(uiMsgLen);
    //跳过占uiPDULen的4个字节开始读
    this->read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    qDebug()<<pdu->uiMsgType<<(char*)(pdu->caMsg);
}
