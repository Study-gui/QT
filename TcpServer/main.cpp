#include "tcpserver.h"
#include"opedb.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tcpserver w;
    OpeDB::getInstance().init();
    w.show();
    return a.exec();
}
