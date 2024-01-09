#include "online.h"
#include "ui_online.h"
#include<QStringList>
Online::Online(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Online)
{
    ui->setupUi(this);
}

void Online::ShowUsr(PDU *pdu)
{
    char tmp[32];
    for(int i=0;i<pdu->uiMsgLen/32;i++)
    {
        memcpy(tmp,(char*)(pdu->caMsg)+i*32,32);
        ui->online_lw->addItem(QString::fromLocal8Bit(tmp));
        qDebug()<<tmp;
        qDebug()<<"ShowUsr";
    }

}

Online::~Online()
{
    delete ui;
}
