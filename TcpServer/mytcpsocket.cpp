#include "mytcpsocket.h"
#include<QDebug>
MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket{parent}
{
    //socket接收成功会自动发送信号readyRead()函数
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));

    //socket断开连接会自动发送信号 disconnected()函数
    connect(this,SIGNAL(disconnected()),this,SLOT(clientOffine()));
}

QString MyTcpSocket::getName()
{
    return m_strName;
}

void MyTcpSocket::clientOffine()
{
  //根据客户端用户名，来修改在线状态,将m_strName转换为char*
    OpeDB::getInstance().handleOffline(m_strName.toStdString().c_str());
    //槽函数接口
    emit Offline(this);
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
    switch (pdu->uiMsgType) {
    case ENUM_MSG_TYPE_REGIST_REQUEST:
    {
        char caname[128]={'\0'};
        char capwd[128]={'\0'};
        strncpy(caname,pdu->caData,32);
        strncpy(capwd,pdu->caData+32,32);
        bool ret=OpeDB::getInstance().handleRegist(caname,capwd);
        //接收注册信息的respdu，传输到客户端
        PDU* respdu=mkPDU(0);
        if(ret)
        {
            strcpy(respdu->caData,REGIST_OK);
        }
        else
        {
            strcpy(respdu->caData,REGIST_FAILED);

        }
        //发送信息类型为注册回复
        respdu->uiMsgType=ENUM_MSG_TYPE_REGST_RESPOND;
        write((char*)respdu,respdu->uiPDULen);
        free(pdu);
        pdu=NULL;
        free(respdu);
        respdu=NULL;
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST:
    {
        char caname[128]={'\0'};
        char capwd[128]={'\0'};
        strncpy(caname,pdu->caData,32);
        strncpy(capwd,pdu->caData+32,32);
        bool ret=OpeDB::getInstance().handleLogin(caname,capwd);
        //接收注册信息的respdu，传输到客户端
        PDU* respdu=mkPDU(0);
        if(ret)
        {
            strcpy(respdu->caData,LOGIN_OK);
            m_strName=caname;
        }
        else
        {
            strcpy(respdu->caData,LOGIN_FAILED);

        }
        //发送信息类型为登录回复
        respdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_RESPOND;
        write((char*)respdu,respdu->uiPDULen);
        free(pdu);
        pdu=NULL;
        free(respdu);
        respdu=NULL;
        break;
    }
    default:
        break;
    }

    //qDebug()<<pdu->uiMsgType<<(char*)(pdu->caMsg);
}
