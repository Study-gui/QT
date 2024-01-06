#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include<Mytcpserver.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class Tcpserver;
}
QT_END_NAMESPACE

class Tcpserver : public QWidget
{
    Q_OBJECT

public:
    //获取ip函数
    void loadConfig();
    Tcpserver(QWidget *parent = nullptr);
    ~Tcpserver();

private:
    QString m_strIp;
    quint16 m_usPort;
    Ui::Tcpserver *ui;
};
#endif // TCPSERVER_H
