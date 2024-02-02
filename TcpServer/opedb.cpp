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
        //qDebug()<<quecy.value(0).toString();
    }
    return result;
}

int OpeDB::handleSearchUsr(const char *name)
{
    if(NULL==name)
    {
        qDebug()<<"name failed";
        return -2;
    }
    QSqlQuery quecy;
    QString data=QString("select online from usrInfo where name=\'%1\'").arg(name);
    quecy.exec(data);
    //返回-1表示没有找到，1表示在线，0表示不在线
    if(quecy.next())
    {
        if(quecy.value(0)==1)
        {
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return -1;
    }

}

int OpeDB::headleAddFriend(const char *perName, const char *UsrName)
{
    //0表示不在线，1表示在线，-1表示没有找到，3表示已经是好友了 -2表示出现错误
    if(NULL==perName||NULL==UsrName)
    {

        qDebug()<<"headleAddFriend";
        return -2;//错误
    }
    QSqlQuery quecy;
    QString data=QString("select *from friend where (id=(select id from usrInfo where name=\'%1\') and friendId=(select id from usrInfo "
                           "where name=\'%2\'))or (id=(select id from usrInfo where name=\'%3\') and friendId=(select id from usrInfo where name=\'%4\'))").arg(perName).arg(UsrName)
                       .arg(UsrName).arg(perName);
    //qDebug()<<data;
    quecy.exec(data);
    if(quecy.next())
    {
        //表示已经是好友了
        return 2;
    }
    else{
        QSqlQuery quecy;
        QString data=QString("select online from usrInfo where name=\'%1\'").arg(perName);
        quecy.exec(data);
        //返回-1表示没有找到，1表示在线，0表示不在线
        if(quecy.next())
        {
            if(quecy.value(0)==1)
            {
                return 1;
            }
            else{
                return 0;
            }
        }
        else{
            return -1;
        }
    }
}

void OpeDB::headleAddFriendAgree(const char *perName, const char *UsrName)
{
    if(NULL==perName||NULL==UsrName)
    {
        qDebug()<<"headleAddFriendAgree";
        return ;
    }
    QSqlQuery quecy;
    QString data=QString("select id from usrInfo where name=\'%1\'").arg(perName);
    quecy.exec(data);
    QVariant id;
    QVariant friendId;
    if(quecy.next())
    {
        id=quecy.value(0);
    }
    data=QString("select id from usrInfo where name=\'%1\'").arg(UsrName);
    quecy.exec(data);
    if(quecy.next())
    {
        friendId=quecy.value(0);
    }
    QString id1=id.toString();
    QString id2=friendId.toString();
    data=QString("insert into friend(id,friendId) values(\'%1\',\'%2\')").arg(id1).arg(id2);
    //qDebug()<<data;
    quecy.exec(data);
    if(quecy.next())
    {
        qDebug()<<"============================================";
    }

}

QStringList OpeDB::headleFlushFriend(const char *name)
{
    if(name==NULL)
    {
        //return ;
        qDebug()<<"headleFlushFriend";

    }
    QStringList strFriendlist;
    strFriendlist.clear();
    QString data=QString("select name from usrInfo where online=1 and id in (select id from friend where friendId=(select id from usrInfo where name=\'%1\'))").arg(name);
    QSqlQuery quecy;
    quecy.exec(data);
    while(quecy.next())
    {
        strFriendlist.append(quecy.value(0).toString());
        //qDebug()<<quecy.value(0);
    }
    data=QString("select name from usrInfo where online=1 and id in (select friendId from friend where id=(select id from usrInfo where name=\'%1\'))").arg(name);
    quecy.exec(data);
    while(quecy.next())
    {
        strFriendlist.append(quecy.value(0).toString());
        //qDebug()<<quecy.value(0);
    }
    return strFriendlist;
}

bool OpeDB::headleDelFriend(const char *Usrname, const char *Pername)
{
    if(Usrname==NULL&&Pername==NULL)
    {
        return false;
    }
    QSqlQuery quecy;
    QString data=QString("delete from friend where id=(select id from usrInfo where name=\'%1\') and friendId=(select id from usrInfo where name=\'%2\')").arg(Usrname).arg(Pername);
    quecy.exec(data);

    data=QString("delete from friend where id=(select id from usrInfo where name=\'%1\') and friendId=(select id from usrInfo where name=\'%2\')").arg(Pername).arg(Usrname);
    quecy.exec(data);
    return true;
}

