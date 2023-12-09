#include "widget.h"
#include "ui_widget.h"
#include<QFileDialog>
#include<QByteArray>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    //点击浏览，到文件选择文件
    QString filename= QFileDialog::getOpenFileName(this,"文件","D:\\Qt项目\\2023\\test_12_6_mouseEvent");
    //判断是否读取成功
    if(filename.isEmpty())
    {
        return ;
    }
    ui->lineEdit->setText(filename);
    //打开文件
    QFile file(filename);
    //选取读取方式
    file.open(QIODeviceBase::ReadOnly);
    //对读取文件的内容进行存储,进行整行读取，如果要进行单行读取使用函数readLine
    QByteArray array= file.readAll();
    //系统自带的编码时utf8，不会乱码，其他的会乱码,修改编码的格式进行输出



    //把QByrteArray转换成QString
    QString contect=(QString)array;
    ui->textEdit->setText(contect);
    //文件进行关闭
    file.close();
}

