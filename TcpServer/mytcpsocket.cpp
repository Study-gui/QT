#include "mytcpsocket.h"
#include<QDebug>
#include<mytcpserver.h>
#include<QDir>
#include<QMessageBox>
MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket{parent}
{
    //初始化定时器
    m_timer=new QTimer;

    //初始化置为假
    m_bUploaddt=false;
    //socket接收成功会自动发送信号readyRead()函数
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));

    //socket断开连接会自动发送信号 disconnected()函数
    connect(this,SIGNAL(disconnected()),this,SLOT(clientOffine()));

    //关联定时器与发送客户端信号与槽
    connect(m_timer,SIGNAL(timeout()),this,SLOT(sendFileTOClient()));
}

QString MyTcpSocket::getName()
{
    return m_strName;
}

void MyTcpSocket::CopyDir(QString strSrcDir, QString strDestDir)
{
    QDir dir;
    dir.mkdir(strDestDir);
    dir.setPath(strSrcDir);
    QFileInfoList filelistInfo= dir.entryInfoList();
    QString strsrcPath;
    QString strDestPath;
    for(int i=0;i<filelistInfo.size();i++)
    {
        if(filelistInfo[i].isFile())
        {
            strsrcPath=strSrcDir+'/'+filelistInfo[i].fileName();
            strDestPath=strDestDir+'/'+filelistInfo[i].fileName();
            QFile::copy(strsrcPath,strDestPath);
        }
        else if(filelistInfo[i].isDir())
        {
            if(filelistInfo[i].fileName()==QString(".")||filelistInfo[i].fileName()==QString(".."))
            {
                continue;
            }
            strsrcPath=strSrcDir+'/'+filelistInfo[i].fileName();
            strDestPath=strDestDir+'/'+filelistInfo[i].fileName();
            CopyDir(strsrcPath,strDestPath);
        }
    }
}

void MyTcpSocket::clientOffine()
{
  //根据客户端用户名，来修改在线状态,将m_strName转换为char*
    OpeDB::getInstance().handleOffline(m_strName.toStdString().c_str());
    //调用自定义下线信号接口
    emit Offline(this);
    //qDebug()<<"========================";
}

void MyTcpSocket::recvMsg()
{
    if(m_bUploaddt==false)
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
                //注册成功创建根目录
                QDir dir;
                QString mstrDir=QString("./%1").arg(caname);
                dir.mkdir(mstrDir);
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
            // for(int i=0;i<ret.size();i++)
            // {
            //     // qDebug()<<"====================";
            //     // qDebug()<<ret.at(i);
            //     // qDebug()<<"====================";
            // }
            uint  uiMsgLen=ret.size()*32;
            PDU* respdu=mkPDU(uiMsgLen);
            respdu->uiMsgType=ENUM_MSG_TYPE_ALL_ONLINE_RESPOND;
            for(uint i=0;i<ret.size();i++)
            {
                memcpy((char*)(respdu->caMsg)+i*32,ret.at(i).toStdString().c_str(),ret[i].toUtf8().size());
                // qDebug()<<ret[i];
                // qDebug()<<(char*)respdu->caMsg;
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
        //刷新好友请求
        case ENUM_MSG_TYPE_FLUSH_FRIEND_REGST_REQUEST:
        {
            char caPerName[32]={'\0'};
            strncpy(caPerName,pdu->caData,32);
            //qDebug()<<caPerName;
            QStringList ret=OpeDB::getInstance().headleFlushFriend(caPerName);
            uint res=ret.size()*32;
            PDU* respdu=mkPDU(res);
            for(int i=0;i<ret.size();i++)
            {
                strncpy((char*)respdu->caMsg+i*32,ret.at(i).toStdString().c_str(),ret.at(i).toUtf8().size());
                //qDebug()<<ret.at(i);
            }
            respdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FRIEND_REGST_RESPOND;
            write((char*)respdu,respdu->uiPDULen);
            free(pdu);
            pdu=NULL;
            free(respdu);
            respdu=NULL;
            break;
        }
        case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:
        {
            char caUsrname[32]={'\0'};
            char caPername[32]={'\0'};
            strncpy(caUsrname,pdu->caData,32);
            strncpy(caPername,pdu->caData+32,32);
            // qDebug()<<caUsrname;
            // qDebug()<<caPername;
            OpeDB::getInstance().headleDelFriend(caUsrname,caPername);
            //返回删除客户端
            PDU* respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;
            write((char*)respdu,respdu->uiPDULen);
            //被删除的客户端
            MyTcpServer::getInstance().reSend(caPername,pdu);
            free(pdu);
            pdu=NULL;
            free(respdu);
            respdu=NULL;
            break;
        }
        case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST:
        {
            char caUsrname[32]={'\0'};
            char caPername[32]={'\0'};
            strncpy(caUsrname,pdu->caData,32);
            strncpy(caPername,pdu->caData+32,32);
            // qDebug()<<caUsrname;
            // qDebug()<<caPername;
            //返回自己说的话
            uint size=pdu->uiPDULen-sizeof(PDU);
            PDU* respdu=mkPDU(size);
            respdu->uiMsgType=ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND;
            memcpy((char*)respdu->caMsg,(char*)pdu->caMsg,size);
            // qDebug()<<(char*)pdu->caMsg;
            // qDebug()<<(char*)respdu->caMsg;
            //将用户名进行调换
            strncpy(respdu->caData,pdu->caData+32,32);
            strncpy(respdu->caData+32,pdu->caData,32);
            //发送自己要发送的信息
            write((char*)respdu,respdu->uiPDULen);
            //找到要私聊的socket
            MyTcpServer::getInstance().reSend(caPername,pdu);
            free(respdu);
            respdu=NULL;
            free(pdu);
            pdu=NULL;
            break;
        }
        case ENUM_MSG_TYPE_GROUP_CHATG_REQUEST:
        {
            char caPerName[32]={'\0'};
            strncpy(caPerName,pdu->caData,32);
            QStringList groupChatfriend=OpeDB::getInstance().headleFlushFriend(caPerName);
            for(int i=0;i<groupChatfriend.size();i++)
            {
                MyTcpServer::getInstance().reSend(groupChatfriend.at(i).toStdString().c_str(),pdu);
            }
            qDebug()<<(char*)pdu->caMsg;
            write((char*)pdu,pdu->uiPDULen);
            free(pdu);
            pdu=NULL;
            break;
        }
        //创建文件夹
        case ENUM_MSG_TYPE_CREATE_DIR_REQUEST:
        {
            QDir dir;
            QString strCurPath=QString("%1").arg((char*)pdu->caMsg);
            //看当前路径是否存在
            bool ret=dir.exists(strCurPath);
            PDU* respdu=mkPDU(0);
            if(ret)//存在为真,当前目录存在
            {
                respdu->uiMsgType=ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
                char strPath[32]={'\0'};
                strncpy(strPath,pdu->caData+32,32);
                // qDebug()<<strPath;
                // qDebug()<<strCurPath;
                QString strNewPath=strCurPath+"/"+strPath;
                //看当前文件是否存在
                ret=dir.exists(strNewPath);
                if(ret)//存在
                {
                    strcpy(respdu->caData,FILE_NAME_EXIST);
                }
                else//不存在
                {
                    dir.mkdir(strNewPath);
                    strcpy(respdu->caData,CREAT_DIR_OK);
                    //qDebug()<<strNewPath;
                }

            }
            else//不存在为假，当前目录不存在
            {
                respdu->uiMsgType=ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
                strcpy(respdu->caData,DIR_NO_EXIST);

            }
            write((char*)respdu,respdu->uiPDULen);
            free(pdu);
            pdu=NULL;
            free(respdu);
            respdu=NULL;
            break;
        }
        case ENUM_MSG_TYPE_FLUSH_FILE_REQUEST:
        {
            //获取文件地址
            char* strPath=new char[pdu->uiMsgLen];
            memcpy(strPath,(char*)pdu->caMsg,pdu->uiMsgLen);
            // qDebug()<<strPath;
            // qDebug()<<(char*)pdu->caMsg;
            QDir dir(strPath);
            QFileInfoList fileInfoList= dir.entryInfoList();

            PDU* respdu=mkPDU(sizeof(FileInfo)*fileInfoList.size());
            FileInfo* fileinfo=NULL;
            for(int i=0;i<fileInfoList.size();i++)
            {
                fileinfo=(FileInfo*)(respdu->caMsg)+i;
                //拷贝文件夹名字
                strncpy(fileinfo->caFileName,fileInfoList[i].fileName().toStdString().c_str(),fileInfoList[i].fileName().toUtf8().size());
                if(fileInfoList[i].isDir())
                {
                    //为文件夹
                    fileinfo->iFileType=0;
                }
                else if(fileInfoList[i].isFile())
                {
                    //为常规文件
                    fileinfo->iFileType=1;
                }
                qDebug()<<fileinfo->caFileName;
            }
            respdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FILE_RESPOND;
            write((char*)respdu,respdu->uiPDULen);
            free(respdu);
            respdu=NULL;
            free(pdu);
            pdu=NULL;
            delete []strPath;
            strPath=NULL;
            break;
        }
        case ENUM_MSG_TYPE_DEL_DIR_REQUEST:
        {
            //文件名
            char caFilename[32]={'\0'};
            strncpy(caFilename,pdu->caData,32);
            //qDebug()<<caFilename;
            //文件地址
            char* caPath=new char[pdu->uiMsgLen];
            strncpy(caPath,(char*)pdu->caMsg,pdu->uiMsgLen);
            QString DirPath=QString("%1/%2").arg(caPath).arg(caFilename);
            //qDebug()<<DirPath;
            delete []caPath;
            caPath=NULL;
            QFileInfo fileInfo(DirPath);
            //判断是否删除成功
            bool ret=false;
            PDU* respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_DEL_DIR_RESPOND;
            if(fileInfo.isDir())
            {
                //为文件夹
                QDir dir;
                dir.setPath(DirPath);
                ret=dir.removeRecursively();

            }
            else if(fileInfo.isFile())
            {
                //为常规文件,不进行删除

            }
            if(ret)
            {
                //删除
                strcpy(respdu->caData,DEL_DIR_OK);

            }
            else
            {
                //没有删除
                strcpy(respdu->caData,DEL_DIR_FAILURED);
            }
            write((char*)respdu,respdu->uiPDULen);
            free(respdu);
            respdu=NULL;
            free(pdu);
            pdu=NULL;
            break;
        }
        case ENUM_MSG_TYPE_RENAME_FILE_REQUEST:
        {
            char strOldName[32]={'\0'};
            char strNewName[32]={'\0'};
            strncpy(strOldName,pdu->caData,32);
            strncpy(strNewName,pdu->caData+32,32);
            char* strPath=new char[pdu->uiMsgLen];
            strncpy(strPath,(char*)pdu->caMsg,pdu->uiMsgLen);
            QString DirOldPath=QString("%1/%2").arg(strPath).arg(strOldName);
            QString DirNewPath=QString("%1/%2").arg(strPath).arg(strNewName);
            delete []strPath;
            strPath=NULL;
            QDir dir;
            bool ret=dir.rename(DirOldPath,DirNewPath);
            PDU* respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_RENAME_FILE_RESPOND;
            if(ret)
            {
                //重命名成功
                strcpy(respdu->caData,RENAME_FILE_OK);
            }
            else
            {
                //重命名失败
                strcpy(pdu->caData,RENAME_FILE_FAILURED);
            }
            write((char*)respdu,respdu->uiPDULen);
            free(respdu);
            respdu=NULL;
            free(pdu);
            pdu=NULL;
            break;
        }
        case ENUM_MSG_TYPE_ENTER_DIR_REQUEST:
        {
            char strName[32]={'\0'};
            strncpy(strName,pdu->caData,64);
            char* strPath=new char[pdu->uiMsgLen];
            strncpy(strPath,(char*)pdu->caMsg,pdu->uiMsgLen);
            QString DirPath=QString("%1/%2").arg(strPath).arg(strName);
            delete []strPath;
            strPath=NULL;
            QFileInfo fileInfo(DirPath);


            if(fileInfo.isDir())
            {
                //为文件夹
                QDir dir(DirPath);
                QFileInfoList fileInfoList= dir.entryInfoList();

                PDU* respdu=mkPDU(sizeof(FileInfo)*fileInfoList.size());
                FileInfo* fileinfo=NULL;
                for(int i=0;i<fileInfoList.size();i++)
                {
                    fileinfo=(FileInfo*)(respdu->caMsg)+i;
                    //拷贝文件夹名字
                    strncpy(fileinfo->caFileName,fileInfoList[i].fileName().toStdString().c_str(),fileInfoList[i].fileName().toUtf8().size());
                    if(fileInfoList[i].isDir())
                    {
                        //为文件夹
                        fileinfo->iFileType=0;
                    }
                    else if(fileInfoList[i].isFile())
                    {
                        //为常规文件
                        fileinfo->iFileType=1;
                    }
                    //qDebug()<<fileinfo->caFileName;
                }
                respdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FILE_RESPOND;
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu=NULL;
            }
            else if(fileInfo.isFile())
            {
                PDU* respdu=mkPDU(0);
                respdu->uiMsgType=ENUM_MSG_TYPE_ENTER_DIR_RESPOND;
                strcpy(respdu->caData,ENTER_DIR_FAILURED);
                //qDebug()<<pdu->caData;
                write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu=NULL;
            }

            free(pdu);
            pdu=NULL;
            break;

        }
        case ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST:
        {
            char strFileName[32]={'\0'};
            qint64 filesize=0;
            //从pdu->cadata中读取
            sscanf(pdu->caData,"%s %lld",strFileName,&filesize);
            char* strFilePath=new char[pdu->uiMsgLen];
            strncpy(strFilePath,(char*)pdu->caMsg,pdu->uiMsgLen);
            // qDebug()<<strFileName<<"========"<<filesize;
            // qDebug()<<strFilePath;
            //拼接要上传到文件夹的文件路径
            QString strPath=QString("%1/%2").arg(strFilePath).arg(strFileName);
            // qDebug()<<strPath;
            delete []strFilePath;
            strFilePath=NULL;
            //当前文件夹里创建上传文件
            m_file.setFileName(strPath);
            //以只写的形式打开文件，没有会自动创建文件
            if(m_file.open(QIODevice::WriteOnly))
            {
                m_bUploaddt=true;
                m_iTotal=filesize;
                m_iRecved=0;
            }
            PDU* respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND;
            //判断是否为传送为空文件，如果是空文件的话，只需创建无需写入，不然会将上一个上传文件pdu的信息写入文件们就会报错
            if(m_iTotal==0)
            {
                m_file.close();
                //置为假
                m_bUploaddt=false;
                strcpy(respdu->caData,UPLOAD_FILE_OK);
                write((char*)respdu,respdu->uiPDULen);
            }
            free(respdu);
            respdu=NULL;
            free(pdu);
            pdu=NULL;
            break;
        }
        case ENUM_MSG_TYPE_DIR_FILE_REQUEST:
        {
            //文件名
            char caFilename[32]={'\0'};
            strncpy(caFilename,pdu->caData,32);
            //qDebug()<<caFilename;
            //文件地址
            char* caPath=new char[pdu->uiMsgLen];
            strncpy(caPath,(char*)pdu->caMsg,pdu->uiMsgLen);
            QString DirPath=QString("%1/%2").arg(caPath).arg(caFilename);
            //qDebug()<<DirPath;
            delete []caPath;
            caPath=NULL;
            QFileInfo fileInfo(DirPath);
            //判断是否删除成功
            bool ret=false;
            PDU* respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_DIR_FILE_RESPOND;
            if(fileInfo.isDir())
            {
                //为文件夹


            }
            else if(fileInfo.isFile())
            {
                //为常规文件,进行删除
                QDir dir;
                //dir.setPath(DirPath);
                ret=dir.remove(DirPath);


            }
            if(ret)
            {
                //删除
                strcpy(respdu->caData,DEL_FILE_OK);

            }
            else
            {
                //没有删除
                strcpy(respdu->caData,DEL_FILE_FAILURED);
            }
            write((char*)respdu,respdu->uiPDULen);
            free(respdu);
            respdu=NULL;
            free(pdu);
            pdu=NULL;
            break;

            break;
        }
        case ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST:
        {

            char strFileName[32]={'\0'};
            //从pdu->cadata中读取
            //sscanf(pdu->caData,"%s %lld",strFileName,&filesize);
            strncpy(strFileName,pdu->caData,64);
            char* strFilePath=new char[pdu->uiMsgLen];
            strncpy(strFilePath,(char*)pdu->caMsg,pdu->uiMsgLen);
            QString strPath=QString("%1/%2").arg(strFilePath).arg(strFileName);
            // qDebug()<<pdu->caData;
            // qDebug()<<pdu->caMsg;
            // qDebug()<<strFileName;
            // qDebug()<<strFilePath;
            // qDebug()<<strPath;
            delete []strFilePath;
            strFilePath=NULL;
            //qDebug()<<strPath;
            //要下载的文件名和文件大小发送给客户端
            QFileInfo file(strPath);
            qint64 filesize=file.size();
            qDebug()<<filesize;
            PDU* respdu=mkPDU(0);
            sprintf(respdu->caData,"%s %lld",strFileName,filesize);
            respdu->uiMsgType=ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND;
            write((char*)respdu,respdu->uiPDULen);
            free(respdu);
            respdu=NULL;
            free(pdu);
            pdu=NULL;
            //放置要发送的文件名字
            m_file.setFileName(strPath);
            if(!m_file.open(QIODeviceBase::ReadOnly))
            {
                //QMessageBox::warning(Tcpserver,"下载文件","获取下载文件路径失败");
                qDebug()<<"服务器下载文件失败";
            }
            else
            {
                //间隔时间避免粘包
                m_timer->start(1000);
            }


            break;

        }
        case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:
        {

            char strsendName[32]={'\0'};
            int num=0;
            sscanf(pdu->caData,"%s %d",strsendName,&num);
            // qDebug()<<strsendName;
            // qDebug()<<num;
            PDU* respdu=mkPDU(pdu->uiMsgLen-num*32);
            respdu->uiMsgType=ENUM_MSG_TYPE_SHARE_FILE_NOTE;
            //分享者姓名
            strcpy(respdu->caData,strsendName);
            //分享文件路径
            memcpy((char*)respdu->caMsg,(char*)pdu->caMsg+num*32,pdu->uiMsgLen-num*32);
            //qDebug()<<(char*)(pdu->caMsg)+num*32;
            for(int i=0;i<num;i++)
            {
                char strName[32]={'\0'};
                memcpy(strName,(char*)pdu->caMsg+32*i,32);
                MyTcpServer::getInstance().reSend(strName,respdu);
            }
            free(respdu);
            respdu=NULL;

            respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
            strcpy(respdu->caData,"share file ok");
            write((char*)respdu,respdu->uiPDULen);
            free(respdu);
            respdu=NULL;
            free(pdu);
            pdu=NULL;
            break;
        }
        case ENUM_MSG_TYPE_SHARE_FILE_NOTE_RESPOND:
        {
            QString strfilePath=QString("./%1").arg(pdu->caData);
            QString strsharefilePath=QString("%1").arg((char*)pdu->caMsg);
            // qDebug()<<strfilePath;
            // qDebug()<<strsharefilePath;
            //  aa/bb/cc
            int index=strsharefilePath.lastIndexOf('/');
            QString filename=strsharefilePath.right(strsharefilePath.size()-index-1);
            strfilePath=strfilePath+"/"+filename;
            QFileInfo file(strsharefilePath);
            if(file.isFile())//为普通文件
            {
                QFile::copy(strsharefilePath,strfilePath);
            }
            else if(file.isDir())//为文件夹
            {
                CopyDir(strsharefilePath,strfilePath);
            }
            free(pdu);
            pdu=NULL;

            break;
        }
        case ENUM_MSG_TYPE_MOVE_FILE_REQUEST:
        {
            int srcsize=0;
            int destsize=0;
            char strfilename[32]={'\0'};
            sscanf(pdu->caData,"%d %d %s",&srcsize,&destsize,strfilename);
            qDebug()<<strfilename<<srcsize<<destsize;
            char* srcfilePath=new char[srcsize+1];
            char* destfilePath=new char[destsize+1+32];
            memset(srcfilePath, '\0', srcsize+1);
            memset(destfilePath, '\0', destsize+1+32);

            memcpy(srcfilePath,(char*)pdu->caMsg,srcsize);
            memcpy(destfilePath,(char*)(pdu->caMsg)+(srcsize+1),destsize);
            qDebug()<<srcfilePath;
            qDebug()<<destfilePath;
            //将移动到新目录拼接一下

            qDebug()<<destfilePath;
            PDU* respdu=mkPDU(0);
            respdu->uiMsgType=ENUM_MSG_TYPE_MOVE_FILE_RESPOND;

            QFileInfo cfile(destfilePath);
            if(cfile.isDir())
            {
                strcat(destfilePath,"/");
                strcat(destfilePath,strfilename);
                //qDebug()<<"=============================";
                //为文件夹
               bool ret=QFile::rename(srcfilePath,destfilePath);
                if(ret)
                {
                   strcpy(respdu->caData,MOVE_FILE_OK);
                }
                else
                {
                   strcpy(respdu->caData,COMMON_ERR);
                }
                //qDebug()<<"=============================";
            }
            else if(cfile.isFile())
            {
                qDebug()<<"------------------------";
                //为普通文件,
                strcpy(respdu->caData,MOVE_FILE_FAILURED);
            }
            delete []srcfilePath;
            srcfilePath=NULL;
            delete[]destfilePath;
            destfilePath=NULL;
            qDebug()<<respdu->caData;
            qDebug()<<MOVE_FILE_FAILURED;
            write((char*)respdu,respdu->uiPDULen);
            free(respdu);
            respdu=NULL;
            free(pdu);
            pdu=NULL;
            break;
        }
        default:
            break;
        }
    }
    else
    {
        PDU* respdu=mkPDU(0);
        respdu->uiMsgType=ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND;
        QByteArray buffer=readAll();
        //往上传文件里写入
        m_file.write(buffer);
        m_iRecved+=buffer.size();
        // qDebug()<<"============";
        // qDebug()<<buffer.size();
        // qDebug()<<m_iRecved;
        // qDebug()<<m_iTotal;
        if(m_iRecved==m_iTotal)
        {
            m_file.close();
            //置为假
            m_bUploaddt=false;
            strcpy(respdu->caData,UPLOAD_FILE_OK);
            write((char*)respdu,respdu->uiPDULen);
        }
        else if(m_iRecved>m_iTotal)
        {
            m_file.close();
            m_bUploaddt=false;
            strcpy(respdu->caData,UPLOAD_FILE_FAILURED);
            write((char*)respdu,respdu->uiPDULen);
        }
        free(respdu);
        respdu=NULL;



    }

    //qDebug()<<pdu->uiMsgType<<(char*)(pdu->caMsg);
}

void MyTcpSocket::sendFileTOClient()
{
      //qDebug()<<"=================================";
    m_timer->stop();
    qint64 filesize=0;
    char* buffer=new char[4096];
    while(true)
    {
       filesize=m_file.read(buffer,4096);
        if(filesize>0&&filesize<=4096)
       {
           write(buffer,filesize);
       }
        else if(filesize==0)
        {
            m_file.close();
            break;
        }
       else if(filesize<0)
       {
           //QMessageBox::warning(Tcpserver,"下载文件","下载失败");
            qDebug()<<"服务端发送给客户端数据时出错";
       }
    }
    delete []buffer;
    buffer=NULL;
      //qDebug()<<"=================================";


}
