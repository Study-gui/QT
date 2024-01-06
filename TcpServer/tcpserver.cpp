#include "tcpserver.h"
#include "ui_tcpserver.h"
#include<QFile>
#include<QByteArray>
#include<QString>
#include<QDebug>
#include<QStringList>
#include<QMessageBox>
#include<QHostAddress>
void Tcpserver::loadConfig()
{
    QFile file(":/Server.config");
    if(file.open(QIODeviceBase::ReadOnly))
    {
        QByteArray baData=file.readAll();
        QString strData=QString(baData);
        file.close();
        //把换行进行替换
        //qDebug()<<strData;
        strData.replace("\r\n"," ");
        //qDebug()<<strData;
        //对数据进行剪切
        QStringList list= strData.split(" ");
        //  for(int i=0;i<list.size();i++)
        // {
        //     qDebug()<<list[i];
        // }
        m_strIp=list.at(0);
        m_usPort=list.at(1).toShort();
        qDebug()<<"Ip:"<<m_strIp<<"Port:"<<m_usPort;
    }
    else{
        QMessageBox::information(this,"打开文件","失败");
    }
}

Tcpserver::Tcpserver(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tcpserver)
{
    ui->setupUi(this);
    loadConfig();
    MyTcpServer::getInstance().listen(QHostAddress(m_strIp),m_usPort);
}

Tcpserver::~Tcpserver()
{
    delete ui;
}
