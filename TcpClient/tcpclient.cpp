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
    //socket接收成功会发出connected()槽函数，
    m_TcpSocket.connectToHost(QHostAddress(m_strIp),m_usPort);
}

tcpclient::~tcpclient()
{
    delete ui;
}

void tcpclient::showConnect()
{
    QMessageBox::information(this,"连接服务器","连接服务器成功");
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

}


void tcpclient::on_regist_pb_clicked()
{
    QString strName=ui->name_le->text();
    QString strPwd=ui->pwd_le->text();
    if(!strName.isEmpty()&& !strPwd.isEmpty())
    {
        PDU* pdu=mkPDU(0);
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

