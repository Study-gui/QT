#include "sharefile.h"
#include<QCheckBox>
#include<QAbstractButton>
#include<QList>
#include"tcpclient.h"
#include"protocol.h"
ShareFile::ShareFile(QWidget *parent)
    : QWidget{parent}
{
    m_pSelectAllpb=new QPushButton("全选");//全选
    m_pCancelSelectpb=new QPushButton("取消选择");//取消全选

    m_pOKpb=new QPushButton("确定");//ok
    m_pCancelpb=new QPushButton("取消");//取消

    m_pSA=new QScrollArea;//滚动区

    m_pFriendw=new QWidget;//好友列表窗口
    m_pButtonGroup=new QButtonGroup(m_pFriendw);
    //将按钮设置为可多选
    m_pButtonGroup->setExclusive(false);
    //将好友按钮垂直放置
    m_pFriendWVbl=new QVBoxLayout(m_pFriendw);
    //水平放置
    QHBoxLayout* pTopHBL=new QHBoxLayout;
    pTopHBL->addWidget(m_pSelectAllpb);
    pTopHBL->addWidget(m_pCancelSelectpb);
    //水平放置
    QHBoxLayout* pDownHBL=new QHBoxLayout;
    pDownHBL->addWidget(m_pOKpb);
    pDownHBL->addWidget(m_pCancelpb);

    //垂直放置
    QVBoxLayout* pMain=new QVBoxLayout;
    pMain->addLayout(pTopHBL);
    pMain->addWidget(m_pSA);
    pMain->addLayout(pDownHBL);

    setLayout(pMain);


    //全选型号与槽
    connect(m_pSelectAllpb,SIGNAL(clicked(bool)),this,SLOT(selectAll()));

    //取消选择信号与槽
    connect(m_pCancelSelectpb,SIGNAL(clicked(bool)),this,SLOT(cancelSelect()));

    //确定信号与槽
    connect(m_pOKpb,SIGNAL(clicked(bool)),this,SLOT(OKshare()));

    //取消信号与槽
    connect(m_pCancelpb,SIGNAL(clicked(bool)),this,SLOT(cancelShare()));
}

ShareFile &ShareFile::getInstance()
{
    static ShareFile instance;
    return instance;
}

void ShareFile::updataFriend(QListWidget *m_pFriendList)
{
    if(m_pFriendList==NULL)
    {
        qDebug()<<"updataFriend";
        return ;
    }
    //以防再次点击出现上一次的信息，要进行清除上次的信息
    QList<QAbstractButton *>plist=m_pButtonGroup->buttons();
    QAbstractButton* tmp=NULL;
    for(int i=0;i<plist.size();)
    {
        tmp=plist[i];
        m_pFriendWVbl->removeWidget(tmp);
        m_pButtonGroup->removeButton(tmp);
        //先移除这个指针
        plist.removeOne(tmp);
        //在释放这个指针
        delete tmp;
        tmp=NULL;
    }

    //放置好友信息
    //放置可选的
    QCheckBox* PCB=NULL;
    for(int i=0;i<m_pFriendList->count();i++)
    {
        PCB=new QCheckBox(m_pFriendList->item(i)->text());
        m_pFriendWVbl->addWidget(PCB);
        m_pButtonGroup->addButton(PCB);

    }
    m_pSA->setWidget(m_pFriendw);
}

void ShareFile::selectAll()
{
   QList<QAbstractButton*> cblist= m_pButtonGroup->buttons();
    for(int i=0;i<cblist.size();i++)
   {
       if(!cblist[i]->isChecked())
        {
            cblist[i]->setChecked(true);
        }
   }
}

void ShareFile::cancelSelect()
{
    QList<QAbstractButton*> cblist= m_pButtonGroup->buttons();
    for(int i=0;i<cblist.size();i++)
    {
        if(cblist[i]->isChecked())
        {
            cblist[i]->setChecked(false);
        }
    }
}

void ShareFile::OKshare()
{
    //把分享者姓名，和接收者个数都拷贝到pdu->cadata中去
    //把接收者姓名和(当前路径和文件名)拼接好 拷贝到pdu->caMsg中
    //分享者姓名
    QString strcurName=tcpclient::getInstance().m_UsrName;
    //当前路径
    QString strcurFilePath=tcpclient::getInstance().getDir();
    //分享文件名
    QString strsharefileName=OpeWidget::getInstance().getBook()->getShareFileName();
    QString strsharePath=strcurFilePath+"/"+strsharefileName;

    QList<QAbstractButton*> cblist= m_pButtonGroup->buttons();
    int num=0;
    for(int i=0;i<cblist.size();i++)
    {
        if(cblist[i]->isChecked())
        {
            num++;
        }
        //qDebug()<<cblist[i]->isChecked();
    }
     //qDebug()<<num;
    PDU* pdu=mkPDU(32*num+strsharePath.toUtf8().size()+1);
    pdu->uiMsgType=ENUM_MSG_TYPE_SHARE_FILE_REQUEST;
    sprintf(pdu->caData,"%s %d",strcurName.toStdString().c_str(),num);
    //qDebug()<<num;
    int j=0;
    for(int i=0;i<cblist.size();i++)
    {
        if(cblist[i]->isChecked())
        {
            memcpy((char*)pdu->caMsg+32*j,cblist[i]->text().toStdString().c_str(),cblist[i]->text().toUtf8().size());
            j++;
        }
        //qDebug()<<cblist[i]->isChecked();
    }
    //qDebug()<<j;
    memcpy((char*)pdu->caMsg+num*32,strsharePath.toStdString().c_str(),strsharePath.toUtf8().size());

    tcpclient::getInstance().getSocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu=NULL;
    if(!ShareFile::getInstance().isHidden())
    {
        ShareFile::getInstance().hide();
    }
}

void ShareFile::cancelShare()
{
    ShareFile::getInstance().hide();
}
