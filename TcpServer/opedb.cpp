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

bool OpeDB::handleRegist(const char *name, const char *pwd)
{
    if(NULL==name||pwd==NULL)
    {
        //qDebug()<<"name|pwd failed";
        return false;
    }
    QSqlQuery query;
    QString data=QString("insert into usrInfo(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    //执行成功返回ture 失败返回false
    //qDebug()<<data;
    return query.exec(data);

}

bool OpeDB::handleLogin(const char *name, const char *pwd)
{
    if(NULL==name||pwd==NULL)
    {
        //qDebug()<<"name|pwd failed";
        return false;
    }
    QSqlQuery query;
    QString data=QString("select *from usrInfo where name=\'%1\' and pwd=\'%2\' and online=0").arg(name).arg(pwd);
    query.exec(data);
    //如果找到了next返回真
    if(query.next())
    {
        //修改online值
        data=QString("update usrInfo set online=1 where name=\'%1\' and pwd=\'%2\'").arg(name).arg(pwd);
        //QSqlQuery query;
        return query.exec(data);
        //return true;

    }
    else{
        return false;
    }
}

void OpeDB::handleOffline(const char *name)
{
    if(NULL==name)
    {
        qDebug()<<"name failed";
        return ;
    }
    QSqlQuery quecy;
    QString data=QString("update usrInfo set online=0 where name=\'%1\'").arg(name);
    quecy.exec(data);
    //qDebug()<<"handleoffline";

}

QStringList OpeDB::handleAllOnline()
{
    QSqlQuery quecy;
    QString data=QString("select name from usrInfo where online=1");
    quecy.exec(data);
    QStringList result;
    result.clear();
    while(quecy.next())
    {
        result.append(quecy.value(0).toString());
        qDebug()<<quecy.value(0).toString();
    }
    return result;
}

