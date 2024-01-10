#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include"protocol.h"
#include<opewidget.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class tcpclient;
}
QT_END_NAMESPACE

class tcpclient : public QWidget
{
    Q_OBJECT

public:
    //获取ip函数
    void loadConfig();
    tcpclient(QWidget *parent = nullptr);
    //设置单例模式函数
    static tcpclient &getInstance();
    //获取m_Tcpsocket
    QTcpSocket &getSocket();
    //自身的用户名
    QString m_UsrName;

    ~tcpclient();
public slots:
    //显示TcpSocket连接信息的槽函数
    void showConnect();
    //创建接收注册信息的函数
    void recvMsg();
private slots:
    //void on_send_pb_clicked();

    void on_login_pb_clicked();

    void on_regist_pb_clicked();

    void on_cancel_pb_clicked();

private:
    //ip地址号
    QString m_strIp;
    //端口号
    quint16 m_usPort;
    Ui::tcpclient *ui;
    //连接服务器，和服务器进行数据交互
    QTcpSocket m_TcpSocket;
};
#endif // TCPCLIENT_H
