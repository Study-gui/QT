#include "mytcpsocket.h"
#include<QDebug>
#include<mytcpserver.h>
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
    case ENUM_MSG_TYPE_ALL_ONLINE_REQUEST:
    {
        QStringList ret=OpeDB::getInstance().handleAllOnline();
        for(int i=0;i<ret.size();i++)
        {
            // qDebug()<<"====================";
            // qDebug()<<ret.at(i);
            // qDebug()<<"====================";
        }
        uint  uiMsgLen=ret.size()*32;
        PDU* respdu=mkPDU(uiMsgLen);
        respdu->uiMsgType=ENUM_MSG_TYPE_ALL_ONLINE_RESPOND;
        for(uint i=0;i<ret.size();i++)
        {
            memcpy((char*)(respdu->caMsg)+i*32,ret.at(i).toStdString().c_str(),ret[i].size());

        }
        write((char*)respdu,respdu->uiPDULen);
        free(pdu);
        pdu=NULL;
        free(respdu);
        respdu=NULL;
        break;
    }
    case ENUM_MSG_TYPE_SEARCH_USR_REQUEST:
    {
        int ret=OpeDB::getInstance().handleSearchUsr(pdu->caData);
        PDU* respdu=mkPDU(0);
        respdu->uiMsgType=ENUM_MSG_TYPE_SEARCH_USR_RESPOND;
        if(1==ret)
        {
            strcpy(respdu->caData,SEARCH_USR_ONLINE);

        }
        else if(0==ret)
        {
            strcpy(respdu->caData,SEARCH_USR_OFFLINE);
        }
        else if(-1==ret)
        {
            strcpy(respdu->caData,SEARCH_USR_NO);
        }
        write((char*)respdu,respdu->uiPDULen);
        free(pdu);
        pdu=NULL;
        free(respdu);
        respdu=NULL;
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
    {
        //0表示不在线，1表示在线，-1表示没有找到，2表示已经是好友了 -2表示出现错误
        char caPerName[32]={'\0'};
        char caUsrName[32]={'\0'};
        strncpy(caPerName,pdu->caData,32);
        strncpy(caUsrName,pdu->caData+32,32);
        int ret=OpeDB::getInstance().headleAddFriend(caPerName,caUsrName);
        PDU* respdu=mkPDU(0);

        if(0==ret)
        {
            respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu->caData,ADD_FRIEND_OFFLINE);

        }
        else if(1==ret)
        {
            //找到好友对象的客户端
            MyTcpServer::getInstance().reSend(caPerName,pdu);
        }
        else if(2==ret)
        {
            respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu->caData,EEXISTED_FRIEEND);

        }
        else if(-1==ret)
        {
            respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            strcpy(respdu->caData,UNKNOW_ERROR);
        }
        write((char*)respdu,respdu->uiPDULen);
        free(pdu);
        pdu=NULL;
        free(respdu);
        respdu=NULL;

        break;
    }
        //添加好友同意的信号
    case ENUM_MSG_TYPE_ADD_FRIEND_AGGREE_REQUEST:
    {
        char caPerName[32]={'\0'};
        char caUsrName[32]={'\0'};
        strncpy(caPerName,pdu->caData,32);
        strncpy(caUsrName,pdu->caData+32,32);
        OpeDB::getInstance().headleAddFriendAgree(caPerName,caUsrName);
        MyTcpServer::getInstance().reSend(caPerName,pdu);
        break;
    }
        //不同意的信号
    case ENUM_MSG_TYPE_ADD_FRIEND_REEUSE_RESPOND:
    {
        //找到好友对象的客户端
        char caPerName[32]={'\0'};
        strncpy(caPerName,pdu->caData,32);
        qDebug()<<caPerName;
        MyTcpServer::getInstance().reSend(caPerName,pdu);
        break;
    }
    default:
        break;
    }

    //qDebug()<<pdu->uiMsgType<<(char*)(pdu->caMsg);
}
