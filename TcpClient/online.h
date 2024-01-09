#ifndef ONLINE_H
#define ONLINE_H

#include <QWidget>
#include<protocol.h>
namespace Ui {
class Online;
}

class Online : public QWidget
{
    Q_OBJECT

public:
    explicit Online(QWidget *parent = nullptr);
    //创建显示查看好友的函数
    void ShowUsr(PDU* pdu);
    ~Online();

private:
    Ui::Online *ui;
};

#endif // ONLINE_H
