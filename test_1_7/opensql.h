#ifndef OPENSQL_H
#define OPENSQL_H

#include <QObject>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QMessageBox>
#include<QString>
class Opensql : public QObject
{
    Q_OBJECT
public:
    ~Opensql();
    //操作数据库函数
    void init();
    explicit Opensql(QObject *parent = nullptr);
    //获取存入信息

    QSqlDatabase m_mysql;

private:

signals:
};

#endif // OPENSQL_H
