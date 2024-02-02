#include "tcpclient.h"
#include "ui_tcpclient.h"
#include<QFile>
#include<QByteArray>
#include<QString>
#include<QDebug>
#include<QStringList>
#include<QMessageBox>
#include<QHostAddress>
#include"privatechat.h"
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
    //OpeWidget::getInstance().getBook()->m_bDownload=false;
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

QString tcpclient::getDir()
{
    return m_strCurPath;
}



void tcpclient::setCurPath(QString PathName)
{
    m_strCurPath=PathName;
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
    if(OpeWidget::getInstance().getBook()->m_bDownload==false)
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
            //获取路径
            m_strCurPath=QString("./%1").arg(m_UsrName);
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
        //接收到另一客户端发出添加好友
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
        //收到服务器添加好友的信息
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:
    {
        QMessageBox::information(this,"添加好友",pdu->caData);

        break;
    }
        //接收到另一客户端不同意添加好友
    case ENUM_MSG_TYPE_ADD_FRIEND_REEUSE_RESPOND:
    {
        char caPerName[32]={'\0'};
        strncpy(caPerName,pdu->caData+32,32);
        qDebug()<<caPerName;
        QMessageBox::information(this,"添加好友",QString("%1 disagree").arg(caPerName));
        break;
    }
        //接收到另一客户端同意添加好友
    case ENUM_MSG_TYPE_ADD_FRIEND_AGGREE_REQUEST:
    {
        char caPerName[32]={'\0'};
        strncpy(caPerName,pdu->caData+32,32);
        qDebug()<<caPerName;
        QMessageBox::information(this,"添加好友",QString("%1 yes").arg(caPerName));
        break;
    }
        //刷新好友回复
    case ENUM_MSG_TYPE_FLUSH_FRIEND_REGST_RESPOND:
    {
        // char caName[32]={'\0'};
        // for(int i=0;i<pdu->uiMsgLen/32;i++)
        // {
        //     strncpy(caName,(char*)pdu->caMsg+i*32,32);
        //     qDebug()<<caName;
        // }
        OpeWidget::getInstance().getFriend()->updateFriendList(pdu);

        break;
    }
        //被删除一方的客户端
    case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:
    {
        char m_strname[32]={'\0'};
        strncpy(m_strname,pdu->caData,32);
        QMessageBox::information(this,"删除好友",QString("\'%1\'要删除跟你为好友").arg(m_strname));
        break;
    }
        //删除一方的客户端
    case ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND:
    {
        QMessageBox::information(this,"删除好友","删除好友成功");
        break;
    }
        //显示对方说的话
    case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
    {
        //判断是否有窗口
        if(PrivateChat::getInstance().isHidden())
        {
            PrivateChat::getInstance().show();
        }
        char caChatName[32]={'\0'};
        strncpy(caChatName,pdu->caData,32);

        PrivateChat::getInstance().setChatName(caChatName);
        // qDebug()<<"==================";
        // qDebug()<<(char*)pdu->caMsg;
        PrivateChat::getInstance().updateMsg(pdu);
        break;
    }
        //显示自己说的话
    case ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND:
    {
        //判断是否有窗口
        if(PrivateChat::getInstance().isHidden())
        {

            PrivateChat::getInstance().show();
        }
        char caChatName[32]={'\0'};
        strncpy(caChatName,pdu->caData,32);
        PrivateChat::getInstance().setChatName(caChatName);
        // qDebug()<<"==================";
        // qDebug()<<(char*)pdu->caMsg;
         PrivateChat::getInstance().ShowChat(pdu);
        break;
    }
    case ENUM_MSG_TYPE_GROUP_CHATG_REQUEST:
    {
        OpeWidget::getInstance().getFriend()->updateGroupMsg(pdu);
        qDebug()<<(char*)pdu->caMsg;
        break;
    }
    case ENUM_MSG_TYPE_CREATE_DIR_RESPOND:
    {
        QMessageBox::information(this,"创建文件夹",QString("%1").arg(pdu->caData));

        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FILE_RESPOND:
    {
        OpeWidget::getInstance().getBook()->updateFileList(pdu);
        //成功进入下一级目录，就要把最新路径加到当前路径中
        QString strEntername=OpeWidget::getInstance().getBook()->enterDir();
        if(!strEntername.isEmpty())
        {
            m_strCurPath=m_strCurPath+"/"+strEntername;
        }
        //qDebug()<<m_strCurPath;

        break;
    }
    case ENUM_MSG_TYPE_DEL_DIR_RESPOND:
    {
        QMessageBox::information(this,"删除文件夹",QString("%1").arg(pdu->caData));
        break;
    }
    case ENUM_MSG_TYPE_RENAME_FILE_RESPOND:
    {
        QMessageBox::information(this,"重命名文件夹",QString("%1").arg(pdu->caData));
        break;
    }
    case ENUM_MSG_TYPE_ENTER_DIR_RESPOND:
    {
        QMessageBox::information(this,"进入文件夹",QString("%1").arg(pdu->caData));
        //进入文件夹失败把保存下一文件夹路径清空
        OpeWidget::getInstance().getBook()->clearEnterDir();
        //qDebug()<<pdu->caData;
        break;
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND:
    {
        QMessageBox::information(this,"上传文件",QString("%1").arg(pdu->caData));
        break;
    }
    case ENUM_MSG_TYPE_DIR_FILE_RESPOND:
    {
        QMessageBox::information(this,"删除文件",QString("%1").arg(pdu->caData));
        break;
    }
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND:
    {
        // qDebug()<<"=============";
        // qDebug()<<pdu->caData;
        qint64 filesize=0;
        char strName[32]={'\0'};
        sscanf(pdu->caData,"%s %lld",strName,&filesize);
        OpeWidget::getInstance().getBook()->m_iTotal=filesize;
        //OpeWidget::getInstance().getBook()->m_iRecved=0;
        if(strlen(strName)>0&& OpeWidget::getInstance().getBook()->m_iTotal>0)
        {
            OpeWidget::getInstance().getBook()->setDownLoadStatus(true);
            m_file.setFileName(OpeWidget::getInstance().getBook()->getSaveFilePath());
            if(!m_file.open(QIODeviceBase::WriteOnly))
            {
                QMessageBox::warning(this,"下载文件","获得保存地址失败");
            }
        }
        else if(OpeWidget::getInstance().getBook()->m_iTotal==0)
        {
             m_file.setFileName(OpeWidget::getInstance().getBook()->getSaveFilePath());
            if(!m_file.open(QIODeviceBase::WriteOnly))
            {
                QMessageBox::warning(this,"下载文件","获得保存地址失败");
            }
            else
            {
               QMessageBox::information(this,"下载文件","下载成功");
             m_file.close();
             OpeWidget::getInstance().getBook()->m_iRecved=0;
             OpeWidget::getInstance().getBook()->m_iTotal=0;
             OpeWidget::getInstance().getBook()->setDownLoadStatus(false);
            }
        }



        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_RESPOND:
    {
        //提示发送成功信息
        QMessageBox::information(this,"分享文件",QString("%1").arg(pdu->caData));
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_NOTE:
    {
        char* strfilePath=new char[pdu->uiMsgLen];
        strcpy(strfilePath,(char*)pdu->caMsg);
        //  aa/bb/cc  从后往前找'/' 找到返回指针，没有找到返回NULL
        char* pos=strrchr(strfilePath,'/');
        //qDebug()<<strfilePath;
        if(pos!=NULL)
        {
            pos++;
            QString strNote=QString("%1 share file : %2 \n Do you accept?").arg(pdu->caData).arg(pos);
            int ret=QMessageBox::question(this,"分享文件",strNote);
            if(ret==QMessageBox::Yes)
            {
                //qDebug()<<strNote;
                PDU* respdu=mkPDU(pdu->uiMsgLen);
                respdu->uiMsgType=ENUM_MSG_TYPE_SHARE_FILE_NOTE_RESPOND;
                QString strName=tcpclient::getInstance().m_UsrName;
                strcpy(respdu->caData,strName.toStdString().c_str());
                strncpy((char*)respdu->caMsg,(char*)pdu->caMsg,pdu->uiMsgLen);
                tcpclient::getInstance().getSocket().write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu=NULL;
            }
            delete []strfilePath;
            strfilePath=NULL;
        }
        break;
    }
    case ENUM_MSG_TYPE_MOVE_FILE_RESPOND:
    {
        QMessageBox::information(this,"移动文件",QString("%1").arg(pdu->caData));
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu=NULL;
    }
    else
    {

        //qDebug()<<"=================================";
        QByteArray buffer=m_TcpSocket.readAll();
        //m_file(OpeWidget::getInstance().getBook()->getSaveFilePath());
        //qint64 ret=0;
           m_file.write(buffer);
           OpeWidget::getInstance().getBook()->m_iRecved+=buffer.size();
           if(OpeWidget::getInstance().getBook()->m_iRecved== OpeWidget::getInstance().getBook()->m_iTotal)
           {
               m_file.close();
               OpeWidget::getInstance().getBook()->m_iRecved=0;
               OpeWidget::getInstance().getBook()->m_iTotal=0;
               OpeWidget::getInstance().getBook()->setDownLoadStatus(false);
               QMessageBox::information(this,"下载文件","下载成功");
           }
           else if( OpeWidget::getInstance().getBook()->m_iRecved> OpeWidget::getInstance().getBook()->m_iTotal)
           {
               m_file.close();
               OpeWidget::getInstance().getBook()->m_iRecved=0;
               OpeWidget::getInstance().getBook()->m_iTotal=0;
               OpeWidget::getInstance().getBook()->setDownLoadStatus(false);
               QMessageBox::information(this,"下载文件","下载失败");
           }
             //qDebug()<<"=================================";
    }
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
    //m_strCurPath=QString("./")+"/"+strName;
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
    tcpclient::getInstance().close();
}

