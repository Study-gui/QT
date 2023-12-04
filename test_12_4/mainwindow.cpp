#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDialog>
#include<QDebug>
#include<QMessageBox>
#include<QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //创建出模态对话框

    connect(ui->actiondia,&QAction::triggered,[=](){
        //创建对话框
        QDialog dig(this);
        //阻塞作用
        dig.exec();
        qDebug()<<"hello";
    });
    //创建非模态对话框
    connect(ui->actiondiamov,&QAction::triggered,[=](){
        QDialog* dig=new QDialog(this);
        //关闭窗口内存并没有释放，要关闭父类窗口内存才会释放，要利用窗口属性让其关闭窗口自动释放
        dig->setAttribute(Qt::WA_DeleteOnClose);
        dig->show();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actiondia_triggered()
{

}


void MainWindow::on_actioninfomation_triggered()
{
    QMessageBox::information(this,"信息","infomation");
}


void MainWindow::on_actionwarning_triggered()
{
    QMessageBox::warning(this,"警告","warning");
}


void MainWindow::on_actioncritical_triggered()
{
    QMessageBox::critical(this,"错误","critical");
}


void MainWindow::on_actionquestion_triggered()
{
    QMessageBox::question(this,"任务","question");
}


void MainWindow::on_actionFiledialog_triggered()
{
   QString name= QFileDialog::getOpenFileName(this,"打开一个文件","C:\\Users\\2814554384");
    qDebug()<<name;
}

