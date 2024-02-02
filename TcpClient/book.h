#ifndef BOOK_H
#define BOOK_H

#include <QWidget>
#include<QListWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QPushButton>
#include"protocol.h"
#include<QTimer>
class Book : public QWidget
{
    Q_OBJECT
public:
    explicit Book(QWidget *parent = nullptr);
    //接受刷新文件夹信息函数
    void updateFileList(PDU* pdu);
    //获取路径
    QString enterDir();
   //清空路径
    void clearEnterDir();
    //或下载文件保存路径函数
    QString getSaveFilePath();
    //返回上一级更新目录
    void setcurPath(QString Pathname);

    //创建是否在下载文件状态的函数
    void setDownLoadStatus(bool status);

    //获得分享文件路径的函数
    QString getShareFileName();

    qint64 m_iTotal;//文件总大小
    qint64 m_iRecved;//文件接收的大小
    bool m_bDownload;
private:
    QListWidget* m_pBookListW;//文件窗口
    QPushButton* m_pReturnPB;//返回按钮
    QPushButton* m_pCreatDirPB;//创建文件夹按钮
    QPushButton* m_pDelDirPB;//删除文件夹按钮
    QPushButton* m_pRenamePB;//重命名文件夹
    QPushButton* m_pFlushFilePB;//刷新文件夹
    QPushButton* m_pUpLoadPB;//上传文件
    QPushButton* m_pDownLoadPB;//删除文件
    QPushButton* m_pDelFilePB;//下载文件
    QPushButton* m_pShareFilePB;//分享文件
    QPushButton* m_pMoveFilepb;//移动文件
    QPushButton* m_pSelectFilepb;//目标目录

    //最新路径
    QString m_strEnterDir;

    //创建定时器
    QTimer* m_timer;

    //上传文件路径
    QString m_uploadFileNamePath;

    //下载文件保存路径
    QString m_strsaveFilePath;


     //分享文件路径
    QString m_strShareFileName;
    //移动文件名
    QString m_strmovefilename;
    //移动文件名路径
    QString m_strmovefilePath;
    //选择目标文件目录
    QString m_strselectDir;
public slots:
    //创建文件夹槽函数
    void CreatDir();

    //创建刷新文件槽函数
    void FlushFile();

    //删除文件夹槽函数
    void Deldir();

    //重命名文件夹槽函数
    void reNameFile();

    //进入文件夹槽函数
    void enterDir(const QModelIndex &index);

    //返回上一级槽函数
    void returnPre();

    //上传文件槽函数
    void uploadFile();

    //上传文件内容槽函数
    void uploadFileData();

    //删除普通文件槽函数
    void DelFile();

    //下载文件槽函数
    void DownoadFile();

    //分享文件槽函数
    void Sharefile();

    //移动文件槽函数
    void MoveFile();

    //选择目标目录槽函数
    void selectDestDir();


signals:
};

#endif // BOOK_H
