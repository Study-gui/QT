#include "book.h"
#include<QInputDialog>
#include"tcpclient.h"
#include<QMessageBox>
#include"protocol.h"
#include<QFileDialog>
#include"sharefile.h"
Book::Book(QWidget *parent)
    : QWidget{parent}
{
    m_bDownload=false;


    //初始化定时器·
    m_timer=new QTimer;

    m_strEnterDir.clear();

    m_pBookListW=new QListWidget;//文件窗口

    m_pReturnPB=new QPushButton("返回");//返回按钮
    m_pCreatDirPB=new QPushButton("创建文件夹");//创建文件夹按钮
    m_pDelDirPB=new QPushButton("删除文件夹");//删除文件夹按钮
    m_pRenamePB=new QPushButton("重命名文件夹");//重命名文件夹
    m_pFlushFilePB=new  QPushButton("刷新文件夹");//刷新文件夹
    //垂直布局
    QVBoxLayout* pDirVBL=new QVBoxLayout;
    pDirVBL->addWidget(m_pReturnPB);
    pDirVBL->addWidget(m_pCreatDirPB);
    pDirVBL->addWidget(m_pDelDirPB);
    pDirVBL->addWidget(m_pRenamePB);
    pDirVBL->addWidget(m_pFlushFilePB);


    m_pUpLoadPB=new  QPushButton("上传文件");//上传文件
    m_pDownLoadPB=new QPushButton("下载文件");//下载文件
    m_pDelFilePB=new QPushButton("删除文件");//删除文件
    m_pShareFilePB=new QPushButton("分享文件");//分享文件
    m_pMoveFilepb=new QPushButton("移动文件"); //移动文件
    m_pSelectFilepb=new QPushButton("目标目录");//目标目录
    //设置为不可点击状态
    m_pSelectFilepb->setEnabled(false);
    //垂直布局
    QVBoxLayout* pFileVBL=new QVBoxLayout;
    pFileVBL->addWidget(m_pUpLoadPB);
    pFileVBL->addWidget(m_pDownLoadPB);
    pFileVBL->addWidget(m_pDelFilePB);
    pFileVBL->addWidget(m_pShareFilePB);
    pFileVBL->addWidget(m_pMoveFilepb);
    pFileVBL->addWidget(m_pSelectFilepb);
    //水平布局
    QHBoxLayout* pMain=new QHBoxLayout;
    pMain->addWidget(m_pBookListW);
    pMain->addLayout(pDirVBL);
    pMain->addLayout(pFileVBL);

    //主窗口
    setLayout(pMain);

    //创建文件夹信号与槽
    connect(m_pCreatDirPB,SIGNAL(clicked(bool)),this,SLOT(CreatDir()));

    //刷新文件夹信号与槽
    connect(m_pFlushFilePB,SIGNAL(clicked(bool)),this,SLOT(FlushFile()));

    //删除文件夹信号与槽
    connect(m_pDelDirPB,SIGNAL(clicked(bool)),this,SLOT(Deldir()));

    //重命名文件夹信号与槽
    connect(m_pRenamePB,SIGNAL(clicked(bool)),this,SLOT(reNameFile()));

    //进入文件夹信号与槽
    connect(m_pBookListW,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(enterDir(QModelIndex)));

    //返回上一级信号与槽
    connect(m_pReturnPB,SIGNAL(clicked(bool)),this,SLOT(returnPre()));

    //上传文件信号与槽
    connect(m_pUpLoadPB,SIGNAL(clicked(bool)),this,SLOT(uploadFile()));

    //关联定时器和上传内容信号与槽
    connect(m_timer,SIGNAL(timeout()),this,SLOT(uploadFileData()));

    //删除普通文件信号与槽
    connect(m_pDelFilePB,SIGNAL(clicked(bool)),this,SLOT(DelFile()));

    //下载文件信号与槽
    connect(m_pDownLoadPB,SIGNAL(clicked(bool)),this,SLOT(DownoadFile()));

    //分享文件信号与槽
    connect(m_pShareFilePB,SIGNAL(clicked(bool)),this,SLOT(Sharefile()));

    //移动文件信号与槽
    connect(m_pMoveFilepb,SIGNAL(clicked(bool)),this,SLOT(MoveFile()));

    //选择目标目录信号与槽
    connect(m_pSelectFilepb,SIGNAL(clicked(bool)),this,SLOT(selectDestDir()));

}

void Book::updateFileList(PDU *pdu)
{
    if(NULL==pdu)
    {
        return ;
    }
    //将上一次的刷新纪录清空避免重复出现
    int row=m_pBookListW->count();
    QListWidgetItem* pItemtmp=NULL;

    while(m_pBookListW->count()>0)
    {
        pItemtmp=m_pBookListW->item(row-1);
        m_pBookListW->removeItemWidget(pItemtmp);
        delete pItemtmp;
        pItemtmp=NULL;
        row--;
    }
    int icount=pdu->uiMsgLen/sizeof(FileInfo);
    FileInfo* filelist=NULL;
    for(int i=0;i<icount;i++)
    {
        filelist=(FileInfo*)(pdu->caMsg)+i;
        QListWidgetItem* listWidgetitem=new QListWidgetItem;
        if(filelist->iFileType==0)
        {
            //文件夹
            listWidgetitem->setIcon(QIcon(QPixmap(":/canva-MAB3ja4nM7w.png")));
        }
        else if(filelist->iFileType==1)
        {
            //普通文件
            listWidgetitem->setIcon(QIcon(QPixmap(":/R-C.jfif")));
        }
        //qDebug()<<filelist->caFileName;

        listWidgetitem->setText(filelist->caFileName);
        m_pBookListW->addItem(listWidgetitem);
    }



}

QString Book::enterDir()
{
    return m_strEnterDir;
}



void Book::CreatDir()
{
    QString m_NewDir= QInputDialog::getText(this,"创建文件夹","文件夹名");
    if(m_NewDir.isEmpty())
    {
        QMessageBox::information(this,"创建文件夹","请输入文件夹名");
    }
    else{
        QString strPath=tcpclient::getInstance().getDir();
        PDU* pdu=mkPDU(strPath.toUtf8().size()+1);
        pdu->uiMsgType=ENUM_MSG_TYPE_CREATE_DIR_REQUEST;
        //前32位存放用户名，后32位存放文件夹名
        strncpy(pdu->caData,tcpclient::getInstance().m_UsrName.toStdString().c_str(),32);
        strncpy(pdu->caData+32,m_NewDir.toStdString().c_str(),m_NewDir.toUtf8().size());
        strncpy((char*)pdu->caMsg,strPath.toStdString().c_str(),strPath.toUtf8().size());
        // qDebug()<<m_NewDir;
        // qDebug()<<pdu->caData;
        // qDebug()<<pdu->caData+32;
        tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
    //关联刷新的信号与槽
    // connect(this,SIGNAL(OpeWidget::getInstance().getBook()->CreatDir()),this, SLOT(OpeWidget::getInstance().getBook()->FlushFile()));
}

void Book::FlushFile()
{
    PDU* pdu=mkPDU(tcpclient::getInstance().getDir().toUtf8().size()+1);
    pdu->uiMsgType=ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    memcpy(pdu->caData,tcpclient::getInstance().m_UsrName.toStdString().c_str(),tcpclient::getInstance().m_UsrName.toUtf8().size());
    memcpy((char*)pdu->caMsg,tcpclient::getInstance().getDir().toStdString().c_str(),tcpclient::getInstance().getDir().toUtf8().size());
    tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
    // qDebug()<<tcpclient::getInstance().getDir();
    // qDebug()<<(char*)pdu->caMsg;
    free(pdu);
    pdu=NULL;
}

void Book::Deldir()
{
   QListWidgetItem* pItem=m_pBookListW->currentItem();
   if(NULL==pItem)
   {
       QMessageBox::information(this,"删除文件夹","请选择文件夹");
   }
   else{

       QString caName=pItem->text();
       QString caPath=tcpclient::getInstance().getDir();
       PDU* pdu=mkPDU(caPath.toUtf8().size()+1);
       pdu->uiMsgType=ENUM_MSG_TYPE_DEL_DIR_REQUEST;
       //删除文件夹名
       strncpy(pdu->caData,caName.toStdString().c_str(),caName.toUtf8().size());
       //删除文件夹路径
       strncpy((char*)pdu->caMsg,caPath.toStdString().c_str(),caPath.toUtf8().size());
       tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
       free(pdu);
       pdu=NULL;

   }
   //关联刷新的信号与槽
   // connect(this,&OpeWidget::getInstance().getBook()->Deldir(),this, &OpeWidget::getInstance().getBook()->FlushFile());

   //删除文件夹信号与槽
   //connect(m_pDelDirPB,SIGNAL(clicked(bool)),this,SLOT(FlushFile()));
   //FlushFile();
}

void Book::reNameFile()
{
   QListWidgetItem* pItem=m_pBookListW->currentItem();
   if(NULL==pItem)
   {
       QMessageBox::information(this,"重命名文件夹","请选择重命名文件夹");
   }
   else{
       QString strOldname=pItem->text();
       QString strNewname=QInputDialog::getText(this,"重命名文件夹","请输入文件夹名");
       if(strNewname.isEmpty())
       {
           QMessageBox::information(this,"重命名文件夹","请输入文件夹名");
       }
       else{
           QString strPath=tcpclient::getInstance().getDir();
           PDU* pdu=mkPDU(strPath.toUtf8().size()+1);
           pdu->uiMsgType=ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
           //前32位放旧的文件夹名，后32位放重命名文件名
           strncpy(pdu->caData,strOldname.toStdString().c_str(),strOldname.toUtf8().size());
           strncpy(pdu->caData+32,strNewname.toStdString().c_str(),strNewname.toUtf8().size());
           //文件路径
           strncpy((char*)pdu->caMsg,strPath.toStdString().c_str(),strPath.toUtf8().size());
           tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
           free(pdu);
           pdu=NULL;
       }
   }
   //关联刷新的信号与槽
   // connect(this,&OpeWidget::getInstance().getBook()->reNameFile(),this, &OpeWidget::getInstance().getBook()->FlushFile());

  // FlushFile();


}

void Book::enterDir(const QModelIndex &index)
{
    QString enterName=index.data().toString();
    if(!enterName.isEmpty())
    {
        //更新最新路径
        m_strEnterDir=enterName;
        QString strPath=tcpclient::getInstance().getDir();
        PDU* pdu=mkPDU(strPath.toUtf8().size()+1);
        pdu->uiMsgType=ENUM_MSG_TYPE_ENTER_DIR_REQUEST;
        strncpy(pdu->caData,enterName.toStdString().c_str(),enterName.toUtf8().size());
        strncpy((char*)pdu->caMsg,strPath.toStdString().c_str(),strPath.toUtf8().size());
        tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
}

void Book::returnPre()
{
    QString strCurPath=tcpclient::getInstance().getDir();
    QString strRootPath="./"+tcpclient::getInstance().m_UsrName;
    if(strCurPath==strRootPath)
    {
        QMessageBox::information(this,"返回","已在根目录无法返回");
    }
    else
    {
        //  aa/bb/cc 从后往前找，返回下标
      int index=strCurPath.lastIndexOf('/');
        //去掉下标index后的字符
        strCurPath.remove(index,strCurPath.size()-index);

      tcpclient::getInstance().setCurPath(strCurPath);
        //返回上一级了，就将最新路径清空,以免重复添加到当前目录中去
        clearEnterDir();

        FlushFile();

    }
}

void Book::uploadFile()
{
    //当前路径名
    QString strcurName=tcpclient::getInstance().getDir();
    //选择上传文件名路径,利用文件对话框选择文件名
    m_uploadFileNamePath=QFileDialog::getOpenFileName();
    if(!m_uploadFileNamePath.isEmpty())
    {
        // /aa/bb/cc --->提取cc
       int index=m_uploadFileNamePath.lastIndexOf('/');
       QString strFileName=m_uploadFileNamePath.right(m_uploadFileNamePath.size()-index-1);

       QFile file(m_uploadFileNamePath);
       qint64 filesize= file.size();

       PDU* pdu=mkPDU(strcurName.toUtf8().size()+1);
       pdu->uiMsgType=ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
       //将要上传文件的文件名和大小写入 pdu->cadata
       sprintf(pdu->caData,"%s %lld",strFileName.toStdString().c_str(),filesize);
       strncpy((char*)pdu->caMsg,strcurName.toStdString().c_str(),strcurName.toUtf8().size());
       // qDebug()<<strFileName;
       // qDebug()<<pdu->caData;
       // qDebug()<<filesize;
       tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
       free(pdu);
       pdu=NULL;

       //上传文件请求和上传文件内容请求会形成粘包，用定时器进行时间间隔
       m_timer->start(1000);
    }
    else
    {
        QMessageBox::information(this,"上传文件","请选择文件");

    }

}

void Book::uploadFileData()
{
    m_timer->stop();
    QFile m_File(m_uploadFileNamePath);
    if(m_File.open(QIODevice::ReadOnly))
    {//QIODeviceBase
        qint64 filesize=0;
        char* buffer=new char[4096];
        //循环读这个文件，每次读4096个字节，知道读完
        while(true)
        {
            filesize=m_File.read(buffer,4096);
            //qDebug()<<filesize;
            if(filesize>0&&filesize<=4096)
            {
                //每次读完直接发送
                tcpclient::getInstance().getSocket().write(buffer,filesize);
            }
            else if(filesize==0)
            {
                m_File.close();
                break;
            }
            else if(filesize<0)
            {
                QMessageBox::information(this,"上传文件","上传文件失败");
            }
        }
        delete []buffer;
        buffer=NULL;


    }
    else
    {
        QMessageBox::warning(this,"上传文件","文件打开失败");
    }


}

void Book::DelFile()
{
   QListWidgetItem* pItem=m_pBookListW->currentItem();
   if(NULL==pItem)
   {
       QMessageBox::information(this,"删除文件","请选择文件");
   }
   else{

       QString caName=pItem->text();
       QString caPath=tcpclient::getInstance().getDir();
       PDU* pdu=mkPDU(caPath.toUtf8().size()+1);
       pdu->uiMsgType=ENUM_MSG_TYPE_DIR_FILE_REQUEST;
       //删除文件名
       strncpy(pdu->caData,caName.toStdString().c_str(),caName.toUtf8().size());
       //删除文件路径
       strncpy((char*)pdu->caMsg,caPath.toStdString().c_str(),caPath.toUtf8().size());
       // qDebug()<<caName;
       // qDebug()<<caPath;
       // qDebug()<<pdu->caData;
       // qDebug()<<(char*)pdu->caMsg;
       tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
       free(pdu);
       pdu=NULL;

   }
}

void Book::DownoadFile()
{
    QListWidgetItem* pItem=m_pBookListW->currentItem();
    if(NULL==pItem)
    {
        QMessageBox::information(this,"下载文件","请选择下载文件");
    }
    else{



        QString strcurPath=tcpclient::getInstance().getDir();
        QString strcurFileName=pItem->text();
        //QString strPath=QString("%1/%2").arg(strcurPath).arg(strcurFileName);
        // qDebug()<<strcurFileName;
        // qDebug()<<strcurPath;
        //选择要保存的路径
        QString strsaveFilePath=QFileDialog::getSaveFileName();
        qDebug()<<strsaveFilePath;

        if(strsaveFilePath.isEmpty())
        {
            QMessageBox::information(this,"下载文件","请选择保存的位置");
            m_strsaveFilePath.clear();
        }
        else{
            //更新路径
            m_strsaveFilePath=strsaveFilePath;
        }

        PDU* pdu=mkPDU(strcurPath.toUtf8().size()+1);
        strncpy((char*)pdu->caMsg,strcurPath.toStdString().c_str(),strcurPath.toUtf8().size());
        strncpy(pdu->caData,strcurFileName.toStdString().c_str(),strcurFileName.toUtf8().size());
        //sprintf(pdu->caData,"%s %lld",strcurFileName.toStdString().c_str(),strcurFileName.size(),filesize);
        // qDebug()<<pdu->caData;
        // qDebug()<<(char*)pdu->caMsg;

        pdu->uiMsgType=ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
        tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;


    }
}

void Book::Sharefile()
{
    //判断是否选中分享文件
    QListWidgetItem* pItem=m_pBookListW->currentItem();
    if(pItem==NULL)
    {

        QMessageBox::information(this,"分享文件","请选择要分享的文件");
    }
    else
    {
         m_strShareFileName=pItem->text();
    QListWidget* m_pfriendlist=OpeWidget::getInstance().getFriend()->getFriendList();
    // for(int i=0;i<m_pfriendlist->count();i++)
    // {
    //     qDebug()<<m_pfriendlist->item(i)->text();
    // }
    ShareFile::getInstance().updataFriend(m_pfriendlist);
    if(ShareFile::getInstance().isHidden())
    {
        ShareFile::getInstance().show();
    }
    }
}

 void Book::MoveFile()
{
    QListWidgetItem* pItem=m_pBookListW->currentItem();

    if(pItem!=NULL)
    {
         m_strmovefilename =pItem->text();
        //qDebug()<<m_strmovefilename;
        QString strcurPath=tcpclient::getInstance().getDir();
        m_strmovefilePath=strcurPath+'/'+m_strmovefilename;
        m_pSelectFilepb->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this,"移动文件","请选择要移动的文件");
    }
}

void Book::selectDestDir()
{
    QListWidgetItem* pItem=m_pBookListW->currentItem();
    if(pItem!=NULL)
    {
          QString strdestfilename =pItem->text();
        m_strselectDir=strdestfilename;
        QString strSrcpath=tcpclient::getInstance().getDir();
        //QString strSrcfilepath=strSrcpath+"/"+m_strmovefilename;//移动文件名路径
        m_strselectDir=strSrcpath+'/'+strdestfilename;//目标文件名路径
        int srcsize=m_strmovefilePath.toUtf8().size();
        int destsize=m_strselectDir.toUtf8().size();
        // qDebug()<<srcsize;
        // qDebug()<<destsize;
        // qDebug()<<m_strmovefilePath;
        // qDebug()<<m_strselectDir;
        // qDebug()<<m_strmovefilename;
        PDU* pdu=mkPDU(srcsize+destsize+2);
        pdu->uiMsgType=ENUM_MSG_TYPE_MOVE_FILE_REQUEST;

        sprintf(pdu->caData,"%d %d %s",srcsize,destsize,m_strmovefilename.toStdString().c_str());
        memcpy((char*)pdu->caMsg,m_strmovefilePath.toStdString().c_str(),srcsize);
        memcpy((char*)pdu->caMsg+srcsize+1,m_strselectDir.toStdString().c_str(),destsize);

        tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
        m_strmovefilename.clear();
    }
    else
    {
        QMessageBox::information(this,"移动文件","请选择目标目录");
    }
    m_pSelectFilepb->setEnabled(false);

}

void Book::clearEnterDir()
{
    m_strEnterDir.clear();
}

QString Book::getSaveFilePath()
{
    return m_strsaveFilePath;
}

void Book::setDownLoadStatus(bool status)
{
    m_bDownload=status;
}

QString Book::getShareFileName()
{
    return m_strShareFileName;
}

// void Book::setcurPath(QString Pathname)
// {
//     m_strEnterDir=Pathname;
// }
