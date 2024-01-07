#include "widget.h"
#include "ui_widget.h"
#include<QDebug>
#include<QMessageBox>
#include<QSqlError>
void Widget::showstudet()
{
    for(QVector<Student>::iterator it=p.begin();it!=p.end();it++)
    {
        qDebug()<<"学生姓名："<<it->m_name<<"学生学号："<<it->m_id;
    }
    qDebug()<<"=========================";
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    //Opensql sql;
    this->init();
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
    m_mysql.close();

}

void Widget::init()
{
        m_mysql.setHostName("127.0.0.1"); //连接本地主机
        m_mysql.setPort(3306);
        m_mysql.setDatabaseName("test");
        m_mysql.setUserName("root");
        m_mysql.setPassword("20010410");
        // 打开数据库
        if (!m_mysql.open()) {	// 数据库打开失败
            QMessageBox::warning(this, "警报", m_mysql.lastError().text());	// 显示错误信息
            return;
        } else {
            QMessageBox::information(this, "提示", "数据库连接成功");
        }
        // if (m_mysql.open())
        // {
        //     QMessageBox::information(NULL, "infor", "link success");

        // }
        // else {
        //     QMessageBox::information(NULL, "infor", "link failed");
        // }

}

void Widget::on_pushButton_clicked()
{
    QString name=this->ui->name_le->text();
    QString id=this->ui->id_le->text();
    Student per;
    if(!name.isEmpty()&& !id.isEmpty())
    {
         //Student per(name,id.toShort());
        // if(m_mysql.open())
        // {
            //QSqlQuery query;
          QSqlQuery query=QSqlQuery(m_mysql);
            QString sql=QString("insert into student(name,id) values('%1,'%2') ").arg(name).arg(id);
            query.exec(sql);
            qDebug()<<"存入成功";
        //}
        // else
        // {
        //     qDebug()<<"存入失败";
        // }
    }
    else
    {
        QMessageBox::critical(NULL,"存入信息","姓名或者学号为空");
    }


    //this->p.push_back(per);
    //this->showstudet();
}
