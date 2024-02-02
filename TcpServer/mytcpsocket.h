#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include"protocol.h"
#include<opedb.h>
#include<QFile>
#include<QTimer>
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = nullptr);
    //获取是由用户名函数
    QString getName();

    //拷贝文件夹的函数
    void CopyDir(QString strSrcDir,QString strDestDir);
signals:
    //客户端下线信号
    void Offline(MyTcpSocket* mysocket);

public slots:
    //客户端下线槽函数,修改数据库状态
    void  clientOffine();
public slots:
    //服务器接收数据槽函数
    void recvMsg();

    //服务器发送数据槽函数
    void sendFileTOClient();
private:
    //客户端用户名
    QString m_strName;

    QFile m_file;
    qint64 m_iTotal;//文件总大小
    qint64 m_iRecved;//文件接收的大小
    bool m_bUploaddt;

    QTimer* m_timer;
};

#endif // MYTCPSOCKET_H
