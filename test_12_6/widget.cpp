#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //listWidget
    //ui->listWidget->addItem("你好");

    //treeWidget
    // //1,添加标签
    // QTreeWidget* tree=new QTreeWidget;
    // ui->treeWidget->setHeaderLabels(QStringList()<<"英雄"<<"简介");
    // //2,添加根节点
    // QTreeWidgetItem* powerItem=new QTreeWidgetItem(QStringList()<<"力量");
    // QTreeWidgetItem* mjItem=new QTreeWidgetItem(QStringList()<<"敏捷");
    // QTreeWidgetItem* zlItem=new QTreeWidgetItem(QStringList()<<"智力");
    // ui->treeWidget->addTopLevelItem(powerItem);
    // ui->treeWidget->addTopLevelItem(mjItem);
    // ui->treeWidget->addTopLevelItem(zlItem);
    // //3,添加相应子节点
    // powerItem->addChild(new QTreeWidgetItem(QStringList()<<"李青"<<"英雄介绍"));
    // powerItem->addChild(new QTreeWidgetItem(QStringList()<<"船长"<<"英雄介绍"));
    // mjItem->addChild(new QTreeWidgetItem(QStringList()<<"卡特"<<"英雄介绍"));
    // mjItem->addChild(new QTreeWidgetItem(QStringList()<<"妖姬"<<"英雄介绍"));
    // zlItem->addChild(new QTreeWidgetItem(QStringList()<<"卡牌"<<"英雄介绍"));
    // zlItem->addChild(new QTreeWidgetItem(QStringList()<<"老鼠"<<"英雄介绍"));

    //tableWidget
    //1，添加行数，列数
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(3);
    //2，添加标签
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"姓名"<<"性别"<<"年龄");
    //添加数据
      //利用数组，进行添加
    QStringList list=QStringList()<<"张飞"<<"小乔"<<"刘备"<<"关羽"<<"曹操";
    QStringList list1=QStringList()<<"男"<<"女"<<"男"<<"男"<<"男";
    QStringList list2=QStringList()<<"25"<<"18"<<"23"<<"22"<<"28";

    for(int row=0;row<5;row++)
    {
        ui->tableWidget->setItem(row,0,new QTableWidgetItem(list[row]));
        ui->tableWidget->setItem(row,1,new QTableWidgetItem(list1[row]));
        //
        //ui->tableWidget->setItem(row,2,new QTableWidgetItem(list2[row]));
        //
        ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(row+20)));
    }


}

Widget::~Widget()
{
    delete ui;
}
