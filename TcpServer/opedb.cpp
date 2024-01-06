#include "opedb.h"
#include<QMessageBox>
#include<QDebug>
#include<QString>
OpeDB::OpeDB(QObject *parent)
    : QObject{parent}
{
    //连接sqlite3数据库
    m_db=QSqlDatabase::addDatabase("QSQLITE");

}

OpeDB& OpeDB::getInstance()
{
    static OpeDB instance;
    return instance;
}

void OpeDB::init()
{
    //连接数据库用户地址，如果不是本地写远程ip地址
    m_db.setHostName("localhost");
    //操作数据库的名字
    m_db.setDatabaseName("D:\\Qt项目\\2023\\TcpServer\\cloud.db");
    if(m_db.open())
    {
        QSqlQuery query;
        query.exec("select * from usrInfo");
        while(query.next())
        {
            QString data=QString("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
            qDebug()<<data;
        }
    }
    else{
        QMessageBox::critical(NULL,"打开数据库","打开数据库失败");

    }

}

OpeDB::~OpeDB()
{
    m_db.close();
}
