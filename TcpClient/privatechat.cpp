#include "privatechat.h"
#include "ui_privatechat.h"
#include<friend.h>
#include<QInputDialog>
#include<QMessageBox>
PrivateChat::PrivateChat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrivateChat)
{
    ui->setupUi(this);
}

PrivateChat::~PrivateChat()
{
    delete ui;
}

void PrivateChat::setChatName(QString m_Name)
{
    m_strChatname=m_Name;
    m_Usrname=tcpclient::getInstance().m_UsrName;
}

PrivateChat &PrivateChat::getInstance()
{
    static PrivateChat instance;
    return instance;
}
//显示对方说的话
void PrivateChat::updateMsg(PDU *pdu)
{
    if(pdu==NULL)
    {
        qDebug()<<"updateMsg";
        return ;
    }
    char caChatName[32]={'\0'};
    strncpy(caChatName,pdu->caData,32);
    // qDebug()<<caChatName;
    // qDebug()<<(char*)pdu->caMsg;
    QString Chat=QString("%1 say: %2").arg(caChatName).arg((char*)(pdu->caMsg));
    ui->showMsg_te->append(Chat);

}
//显示自己说的话
void PrivateChat::ShowChat(PDU *pdu)
{
    if(pdu==NULL)
    {
        qDebug()<<"ShowMsg";
        return ;
    }
    char caChatName[32]={'\0'};
    strncpy(caChatName,pdu->caData+32,32);
    // qDebug()<<caChatName;
    // qDebug()<<(char*)pdu->caMsg;
    QString Chat=QString("%1 say: %2").arg(caChatName).arg((char*)(pdu->caMsg));
    ui->showMsg_te->append(Chat);
}

void PrivateChat::on_sendMsg_pb_clicked()
{
    //Friend::Privatechat();
    QString ChatMsg=ui->inputMsg_le->text();
    ui->inputMsg_le->clear();
    //qDebug()<<ChatMsg;
    if(ChatMsg.isEmpty())
    {
        QMessageBox::information(this,"私聊","信息为空");
    }
    else{

        PDU *pdu=mkPDU(ChatMsg.toUtf8().size()+1);
        pdu->uiMsgType=ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST;
        //前32位放自己的名字，后32位放私聊对象的名字
        strncpy((char*)pdu->caData,m_Usrname.toStdString().c_str(),m_Usrname.size());
        strncpy((char*)pdu->caData+32,m_strChatname.toStdString().c_str(),m_strChatname.size());
        // qDebug()<<m_Usrname;
        // qDebug()<<m_strChatname;
        // qDebug()<<(char*)pdu->caData;
        // qDebug()<<(char*)pdu->caData+32;
        memcpy((char*)pdu->caMsg,ChatMsg.toStdString().c_str(),ChatMsg.toUtf8().size());
        //qDebug()<<(char*)pdu->caMsg;
        tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
}

