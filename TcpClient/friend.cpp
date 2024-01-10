#include "friend.h"
#include<tcpclient.h>
#include<protocol.h>
#include<QInputDialog>
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


