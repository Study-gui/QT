#include "opensql.h"

Opensql::~Opensql()
{
  m_mysql.close();
}

void Opensql::init()
{
    m_mysql.setHostName("127.0.0.1"); //连接本地主机
    m_mysql.setPort(3306);
    m_mysql.setDatabaseName("test");
    m_mysql.setUserName("root");
    m_mysql.setPassword("20010410");

    if (m_mysql.open())
    {
        QMessageBox::information(NULL, "infor", "link success");

    }
    else {
        QMessageBox::information(NULL, "infor", "link failed");
    }
}

Opensql::Opensql(QObject *parent)
    : QObject{parent}
{
    m_mysql=QSqlDatabase::addDatabase("QMYSQL");
    this->init();

}
