#include "friend.h"
#include<tcpclient.h>
#include<protocol.h>
#include<QInputDialog>
#include<QMessageBox>
#include"privatechat.h"
Friend::Friend(QWidget *parent)
    : QWidget{parent}
{
     m_pShowMsgTE=new QTextEdit;  //文本消息框
     m_pFriendListWidget=new QListWidget;  //好友列表窗口
     m_pInputMsgLE=new QLineEdit;  //输入信息框

     m_pDelFriendPB=new  QPushButton("删除好友");   //删除好友按钮
     m_pFlushFriendPB=new QPushButton("刷新好友");  //刷新好友按钮
     m_pShowOnlineUsrPB=new  QPushButton("显示在线用户");   //显示在线用户按钮
     m_pSearchUsrPB=new QPushButton("查找用户");      //查找用户按钮
     m_pMsgSendPB=new QPushButton("信息发送");        //发送消息按钮
     m_PrivateChatPB=new QPushButton("私聊");     //私聊按钮

     //按钮进行左边垂直布局
     QVBoxLayout* pRightPBVBL= new QVBoxLayout;
     pRightPBVBL->addWidget(m_pDelFriendPB);
     pRightPBVBL->addWidget(m_pFlushFriendPB);
     pRightPBVBL->addWidget(m_pShowOnlineUsrPB);
     pRightPBVBL->addWidget(m_pSearchUsrPB);
     pRightPBVBL->addWidget(m_PrivateChatPB);

    //文本信息与按钮进行水平布局
     QHBoxLayout * pTopHBL= new QHBoxLayout;
     pTopHBL->addWidget(m_pShowMsgTE);
     pTopHBL->addWidget(m_pFriendListWidget);
     pTopHBL->addLayout(pRightPBVBL);


     //输入信息框和发送按钮进行水平布局
     QHBoxLayout *pMsgHBL=new QHBoxLayout;
     pMsgHBL->addWidget(m_pInputMsgLE);
     pMsgHBL->addWidget(m_pMsgSendPB);

     m_pShowOnline= new Online;
     //再把整体进行垂直布局
     QVBoxLayout* pMain=new QVBoxLayout;
     pMain->addLayout(pTopHBL);
     pMain->addLayout(pMsgHBL);
     pMain->addWidget(m_pShowOnline);
     m_pShowOnline->hide();

     //显示在线用户的型号与槽
     connect(m_pShowOnlineUsrPB,SIGNAL(clicked(bool)),this,SLOT(ShowOnline()));
     //显示查找用户的信号与槽
     connect(m_pSearchUsrPB,SIGNAL(clicked(bool)),this,SLOT(SearchUsr()));

     //刷新好友信号与槽
     connect(m_pFlushFriendPB,SIGNAL(clicked(bool)),this,SLOT(FlushFriend()));

     //删除好友信号与槽
     connect(m_pDelFriendPB,SIGNAL(clicked(bool)),this,SLOT(delFriend()));

     //私聊好友的信号与槽
     connect(m_PrivateChatPB,SIGNAL(clicked(bool)),this,SLOT(Privatechat()));

     //群聊的信号与槽
     connect(m_pMsgSendPB,SIGNAL(clicked(bool)),this,SLOT(groupChat()));
     //进行输出
     setLayout(pMain);
}

void Friend::ShowAllOnline(PDU *pdu)
{
    if(pdu==NULL)
    {
        return ;
    }
    m_pShowOnline->ShowUsr(pdu);

}

Online *Friend::getOnline()
{
    return m_pShowOnline;
}

void Friend::updateFriendList(PDU *pdu)
{
    if(pdu==NULL)
    {
        return ;
    }
    //再次刷新把上一次信息清除
    for(int i=0;i<m_pFriendListWidget->count();)
    {
        QListWidgetItem *pItem=m_pFriendListWidget->item(i);
        m_pFriendListWidget->removeItemWidget(pItem);
        delete pItem;
        pItem=NULL;
    }
    char caName[32]={'\0'};
    for(int i=0;i<pdu->uiMsgLen/32;i++)
    {
        strncpy(caName,(char*)pdu->caMsg+i*32,32);
        m_pFriendListWidget->addItem(caName);
    }
}

QListWidget *Friend::getFriendList()
{
    return m_pFriendListWidget;
}

void Friend::updateGroupMsg(PDU *pdu)
{
    if(pdu==NULL)
    {
        qDebug()<<"updateGroupMsg";
        return ;
    }
    char caName[32]={'\0'};
    strncpy(caName,pdu->caData,32);
    //qDebug()<<caName;
    QString Chat=QString("%1 say: %2").arg(caName).arg((char*)(pdu->caMsg));
    //qDebug()<<Chat;
    m_pShowMsgTE->append(Chat);

}

void Friend::ShowOnline()
{
    //如果是没有展开，就进行show
    if(m_pShowOnline->isHidden())
    {
        m_pShowOnline->show();
        PDU *pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;
        tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;

    }
    else{
        m_pShowOnline->hide();
    }
}

void Friend::SearchUsr()
{
    QString name= QInputDialog::getText(this,"搜索","用户名");

    if(!name.isEmpty())
    {
        //qDebug()<<name;
        m_strSearchUsrname=name;
        PDU* pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_SEARCH_USR_REQUEST;
        strcpy(pdu->caData,name.toStdString().c_str());
        tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;

    }





}

void Friend::FlushFriend()
{
    //自身用户名
    QString m_strname=tcpclient::getInstance().m_UsrName;
    //qDebug()<<tcpclient::getInstance().m_UsrName;
    //qDebug()<<m_strname;

        PDU* pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FRIEND_REGST_REQUEST;
        memcpy(pdu->caData,m_strname.toStdString().c_str(),32);
        tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;



}

void Friend::delFriend()
{
    //对是否点击进行判断
    if(NULL!=m_pFriendListWidget->currentItem())
    {
        //自己的用户名
        QString m_strName=tcpclient::getInstance().m_UsrName;
        //删除对象的用户名,将列表窗口中的信息转化为文本
        QString m_perName=m_pFriendListWidget->currentItem()->text();
        // qDebug()<<m_strName;
        // qDebug()<<m_perName;
        PDU* pdu=mkPDU(0);
        pdu->uiMsgType=ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
        strncpy(pdu->caData,m_strName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,m_perName.toStdString().c_str(),32);
        tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu==NULL;
    }



}

void Friend::Privatechat()
{
    if(NULL!=m_pFriendListWidget->currentItem())
    {
        //自己的用户名
        //QString m_strName=tcpclient::getInstance().m_UsrName;
        //私聊对象的用户名,将列表窗口中的信息转化为文本
        QString m_perName=m_pFriendListWidget->currentItem()->text();
        PrivateChat::getInstance().setChatName(m_perName);
        if(PrivateChat::getInstance().isHidden())
        {
            PrivateChat::getInstance().show();
        }
        // else{
        //     PrivateChat::getInstance().hide();
        // }
    }
    else{
        QMessageBox::information(this,"私聊","请选择私聊用户");
    }
}

void Friend::groupChat()
{
    QString ChatIn=m_pInputMsgLE->text();
    m_pInputMsgLE->clear();
    if(!ChatIn.isEmpty())
    {
        PDU* pdu=mkPDU(ChatIn.toUtf8().size()+1);
        strncpy((char*)pdu->caData,tcpclient::getInstance().m_UsrName.toStdString().c_str(),tcpclient::getInstance().m_UsrName.size());
        strncpy((char*)pdu->caMsg,ChatIn.toStdString().c_str(),ChatIn.toUtf8().size());
        pdu->uiMsgType=ENUM_MSG_TYPE_GROUP_CHATG_REQUEST;
        tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
    else{
        QMessageBox::information(this,"输入信息","请输入信息");
    }
}


