#ifndef OPEDB_H
#define OPEDB_H

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
private:
    QSqlDatabase m_db; //连接数据库

signals:
};

#endif // OPEDB_H
