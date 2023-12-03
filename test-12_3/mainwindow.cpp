#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QLabel>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置状态栏的信息
    ui->statusbar->addWidget(new QLabel("左侧信息",this));
//使用图片资源
//1，使用绝对路径
    //ui->actionnew->setIcon(QIcon("C:\\Users\\2814554384\\Pictures\\timg.jpg"));
//2，使用资源文件拷贝创建
    ui->actionnew->setIcon(QIcon("://new//prefix1//timg.jpg"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
