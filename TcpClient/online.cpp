#include "online.h"
#include "ui_online.h"
#include<QStringList>
#include<tcpclient.h>
#include<protocol.h>
Online::Online(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Online)
{
    ui->setupUi(this);
}

void Online::ShowUsr(PDU *pdu)
{
    //再次进来清楚上次的信息
    for(int i=0;i<ui->online_lw->count();)
    {
        QListWidgetItem * pItem=ui->online_lw->item(i);
        ui->online_lw->removeItemWidget(pItem);
        delete pItem;
        pItem=NULL;
    }

    char tmp[32];
    for(int i=0;i<pdu->uiMsgLen/32;i++)
    {
        memcpy(tmp,(char*)(pdu->caMsg)+i*32,32);
        //ui->online_lw->addItem(QString::fromLocal8Bit(tmp));
        ui->online_lw->addItem(tmp);
        //qDebug()<<tmp;
        //qDebug()<<"ShowUsr";
    }

}

Online::~Online()
{
    delete ui;
}
//添加好友
void Online::on_addfriend_pb_clicked()
{
    QListWidgetItem * pItem=ui->online_lw->currentItem();
    QString strPerUsrName=pItem->text();
    QString strLoginUsrName=tcpclient::getInstance().m_UsrName;
    if(strPerUsrName==strLoginUsrName)
    {
        return;
    }
    PDU* pdu=mkPDU(0);
    pdu->uiMsgType=ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    strncpy(pdu->caData,strPerUsrName.toStdString().c_str(),32);
    strncpy(pdu->caData+32,strLoginUsrName.toStdString().c_str(),32);
    tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=NULL;

}

