#ifndef OPEDB_H
#define OPEDB_H
#include<QStringList>
#include <QObject>
//连接数据库头文件
#include<QSqlDatabase>
//查询数据库头文件
#include<QSqlQuery>
class OpeDB : public QObject
{
    Q_OBJECT
public:
    explicit OpeDB(QObject *parent = nullptr);
    //定义为单例模式
    static OpeDB& getInstance();
    void init();
    ~OpeDB();
    //注册函数
    bool handleRegist(const char* name,const char* pwd);
    //登陆函数
    bool handleLogin(const char* name,const char* pwd);
    //退出函数
    void handleOffline(const char* name);
    //查询在线用户函数
    QStringList handleAllOnline();
private:
    QSqlDatabase m_db; //连接数据库

signals:
};

#endif // OPEDB_H
