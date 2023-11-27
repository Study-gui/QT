#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include<QPushButton>
#include<QTimer>
#include<iostream>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QObject * btn =new QPushButton;
    qDebug()<<btn->metaObject()->className();

    QPushButton* pushbtn=qobject_cast<QPushButton*>(btn);
    qDebug()<<pushbtn->metaObject()->className();
    cout<<"nihao"<<endl;
    return 0;
}
