#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QString>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionabout_triggered()
{
    QMessageBox::information(this,"点击","QMessageBox::about");
}
void MainWindow::on_toolButton_5_clicked()
{
    this->close();
}


void MainWindow::on_toolButton_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_toolButton_2_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(1);

}


void MainWindow::on_toolButton_3_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(2);

}


void MainWindow::on_toolButton_4_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(3);

}


void MainWindow::on_toolButton_6_clicked()
{
    this->ui->stackedWidget->setCurrentIndex(4);

}


void MainWindow::on_pushButton_clicked()
{
    QString string=QString("您的姓名：%1\n").arg(this->ui->lineEdit_name->text());
    string+=QString("您的性别：%1\n").arg(this->ui->radioButton_man->isChecked()?"男":"女");
    string+=QString("您的职业：%1\n").arg(this->ui->comboBox_work->currentText());

    string+=QString("您的年龄：%1\n").arg(this->ui->spinBox_age->text());
    string+=QString("您的生日：%1\n").arg(this->ui->dateEdit_brithday->text());
    if(this->ui->radioButton_vlike->isChecked())
    {
        string+=QString("您的感觉：%1\n").arg("非常喜欢");
    }
    if(this->ui->radioButton_prelike->isChecked())
    {
        string+=QString("您的感觉：%1\n").arg("较喜欢");
    }
    if(this->ui->radioButton_like->isChecked())
    {
        string+=QString("您的感觉：%1\n").arg("喜欢");
    }
    QMessageBox::information(this,"感谢提交",string);
}

