#include "mywidget.h"
#include "ui_mywidget.h"

Mywidget::Mywidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Mywidget)
{
    ui->setupUi(this);
    //设置滚条和计数器范围
    ui->horizontalSlider->setRange(0,100);
    ui->spinBox->setRange(0,100);


    //用信号与槽，控制滚条动控制计数器
    connect(ui->horizontalSlider,&QSlider::valueChanged,[=](int value){
        ui->spinBox->setValue(value);
    });
    //利用信号与槽，计数器动控制滚条动
    connect(ui->spinBox,&QSpinBox::valueChanged,[=](int value){
        ui->horizontalSlider->setValue(value);
    });

}

Mywidget::~Mywidget()
{
    delete ui;
}

int Mywidget::getvalue()
{
    ui->horizontalSlider->value();
}

void Mywidget::setvalue(int value)
{
    ui->horizontalSlider->setValue(value);
}
