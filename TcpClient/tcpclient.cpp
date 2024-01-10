#include "tcpclient.h"
#include "ui_tcpclient.h"
#include<QFile>
#include<QByteArray>
#include<QString>
#include<QDebug>
#include<QStringList>
#include<QMessageBox>
#include<QHostAddress>

void tcpclient::loadConfig()
{
    QFile file(":/Client.config");
    if(file.open(QIODeviceBase::ReadOnly))
    {
        QByteArray baData=file.readAll();
        QString strData=QString(baData);
        file.close();
        //把换行\n\r进行替换
        //qDebug()<<strData;
        strData.replace("\r\n"," ");
        //qDebug()<<strData;
        //对数据进行剪切,遇到空格进行换行
       QStringList list= strData.split(" ");
       //  for(int i=0;i<list.size();i++)
       // {
       //     qDebug()<<list[i];
       // }
       //存入ip号和端口号
       m_strIp=list.at(0);
       m_usPort=list.at(1).toShort();
       qDebug()<<"Ip:"<<m_strIp<<"Port:"<<m_usPort;
    }
    else{
        QMessageBox::critical(this,"打开文件","失败");
    }
}

tcpclient::tcpclient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::tcpclient)
{
    ui->setupUi(this);
    this->loadConfig();
    //
    connect(&m_TcpSocket,SIGNAL(connected()),this,SLOT(showConnect()));
    connect(&m_TcpSocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));

    //socket连接成功会发出connected()槽函数，接收成功会发出readyRead()槽函数

    m_TcpSocket.connectToHost(QHostAddress(m_strIp),m_usPort);
}

tcpclient &tcpclient::getInstance()
{
    static tcpclient instance;
    return instance;

}

QTcpSocket &tcpclient::getSocket()
{
    return m_TcpSocket;
}

tcpclient::~tcpclient()
{
    delete ui;
}

void tcpclient::showConnect()
{
    QMessageBox::information(this,"连接服务器","连接服务器成功");
}

void tcpclient::recvMsg()
{
    //当前读到的数据大小(字节)
    qDebug()<<m_TcpSocket.bytesAvailable();
    uint uiPDULen=0;
    m_TcpSocket.read((char*)&uiPDULen,sizeof(uint));
    uint uiMsgLen=uiPDULen-sizeof(PDU);
    PDU* pdu=mkPDU(uiMsgLen);
    //跳过占uiPDULen的4个字节开始读
    m_TcpSocket.read((char*)pdu+sizeof(uint),uiPDULen-sizeof(uint));
    switch (pdu->uiMsgType) {
    case ENUM_MSG_TYPE_REGST_RESPOND:
    {
        if(0==strcmp(pdu->caData,REGIST_OK))
        {
            QMessageBox::information(this,"注册",REGIST_OK);
        }
        else if(0==strcmp(pdu->caData,REGIST_FAILED))
        {
            QMessageBox::critical(this,"注册",REGIST_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND:
    {
        if(0==strcmp(pdu->caData,LOGIN_OK))
        {
            QMessageBox::information(this,"登录",LOGIN_OK);
            OpeWidget::getInstance().show();
            this->hide();
        }
        else if(0==strcmp(pdu->caData,LOGIN_FAILED))
        {
            QMessageBox::critical(this,"登录",LOGIN_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_ALL_ONLINE_RESPOND:
    {
        OpeWidget::getInstance().getFriend()->ShowAllOnline(pdu);
        //qDebug()<<(char*)(pdu->caMsg);
        break;
    }
    case ENUM_MSG_TYPE_SEARCH_USR_RESPOND:
    {
        qDebug()<<(char*)(pdu->caData);
        qDebug()<<"查找用户函数";
        if(0==strcmp(SEARCH_USR_ONLINE,pdu->caData))
        {
            QMessageBox::information(this,"搜索",QString("%1: online").arg(OpeWidget::getInstance().getFriend()->m_strSearchUsrname));

        }
        else if(0==strcmp(SEARCH_USR_OFFLINE,pdu->caData))
        {
             QMessageBox::information(this,"搜索",QString("%1: offline").arg(OpeWidget::getInstance().getFriend()->m_strSearchUsrname));
        }
        else if(0==strcmp(SEARCH_USR_NO,pdu->caData))
        {
             QMessageBox::critical(this,"搜索",QString("%1: not exist").arg(OpeWidget::getInstance().getFriend()->m_strSearchUsrname));
        }
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:
    {
        char caPerName[32]={'\0'};
        char caUsrName[32]={'\0'};
        strncpy(caUsrName,pdu->caData,32);
        strncpy(caPerName,pdu->caData+32,32);
        // qDebug()<<caPerName;
        // qDebug()<<caUsrName;
        int ret=QMessageBox::information(this,"添加好友",QString("%1 want to add you friend").arg(caPerName),QMessageBox::Yes,QMessageBox::No);
        PDU* respdu=mkPDU(0);
        strncpy(respdu->caData,caPerName,32);
        strncpy(respdu->caData+32,caUsrName,32);
        if(ret==QMessageBox::Yes)
        {
            respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_AGGREE_REQUEST;

        }
        else{
            respdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_REEUSE_RESPOND;
        }
        this->getInstance().m_TcpSocket.write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu=NULL;
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:
    {
        QMessageBox::information(this,"添加好友",pdu->caData);

        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REEUSE_RESPOND:
    {
        char caPerName[32]={'\0'};
        strncpy(caPerName,pdu->caData+32,32);
        qDebug()<<caPerName;
        QMessageBox::information(this,"添加好友",QString("%1 disagree").arg(caPerName));
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGGREE_REQUEST:
    {
        char caPerName[32]={'\0'};
        strncpy(caPerName,pdu->caData+32,32);
        qDebug()<<caPerName;
        QMessageBox::information(this,"添加好友",QString("%1 yes").arg(caPerName));
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu=NULL;
}

// void tcpclient::on_send_pb_clicked()
// {
//     //点击按钮，获得输入框内的数据
//     QString strMsg=ui->lineEdit->text();
//     if(!strMsg.isEmpty())
//     {
//         PDU *pdu=mkPDU(strMsg.size());
//         //消息类型
//         pdu->uiMsgType=8888;
//         //把文本消息封装到发送数据结构体当中
//         memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.size());
//         m_TcpSocket.write((char*)pdu,pdu->uiPDULen);
//         free(pdu);
//         pdu=NULL;
//     }
//     else{
//         QMessageBox::warning(this,"信息发送","发送消息不能为空");
//     }
// }


void tcpclient::on_login_pb_clicked()
{
    //从文本框获取信息
    QString strName=ui->name_le->text();
    m_UsrName=strName;
    QString strPwd=ui->pwd_le->text();
    if(!strName.isEmpty()&& !strPwd.isEmpty())
    {
        PDU* pdu=mkPDU(0);
        //发送消息类型为注册请求
        pdu->uiMsgType=ENUM_MSG_TYPE_LOGIN_REQUEST;
        //前32位存放用户名，后32位存放密码
        memcpy(pdu->caData,strName.toStdString().c_str(),32);
        memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
        m_TcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
    else{
        QMessageBox::warning(this,"登录","登录用户名或密码为空");
    }
}


void tcpclient::on_regist_pb_clicked()
{
    //从文本框获取信息
    QString strName=ui->name_le->text();
    QString strPwd=ui->pwd_le->text();
    if(!strName.isEmpty()&& !strPwd.isEmpty())
    {
        PDU* pdu=mkPDU(0);
        //发送消息类型为注册请求
        pdu->uiMsgType=ENUM_MSG_TYPE_REGIST_REQUEST;
        //前32位存放用户名，后32位存放密码
        memcpy(pdu->caData,strName.toStdString().c_str(),32);
        memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32);
        m_TcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
    else{
        QMessageBox::warning(this,"注册","注册用户名或密码为空");
    }
}


void tcpclient::on_cancel_pb_clicked()
{

}

