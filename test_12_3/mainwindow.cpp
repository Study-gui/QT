#include "mainwindow.h"
#include<QMenuBar>
#include<QToolBar>
#include<QStatusBar>
#include<QLabel>
#include<QDockWidget>
#include<QTextEdit>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(600,600);
    this->setWindowTitle("文本编辑器");
    //获取当前窗口的菜单栏，没有会自动添加
    QMenuBar* menu=this->menuBar();
    //利用菜单栏进行添加
    QMenu* menuFile=menu->addMenu("文件");
    QMenu* menuEdit=menu->addMenu("编辑");
    //通过菜单，进行添加功能
    QAction*Filenew= menuFile->addAction("新建");
    QAction*FileEdit=menuFile->addAction("编辑");
    //添加分割符
    menuFile->addSeparator();
    //添加二级菜单
    QMenu* Filetwo=menuFile->addMenu("最近打开的文件");
    QAction* file=Filetwo->addAction("1.txt");

    //添加工具栏
    QToolBar* toolbar=this->addToolBar("工具栏");
    //往工具栏里添加工具
    toolbar->addAction(Filenew);
    toolbar->addAction(FileEdit);
    //默认设置工具栏停靠在左边
    this->addToolBar(Qt::LeftToolBarArea,toolbar);
    //只允许设置左边和右边
    toolbar->setAllowedAreas(Qt::LeftToolBarArea|Qt::RightToolBarArea);
    //不允许浮动
    toolbar->setFloatable(false);
    //添加状态栏
    QStatusBar* statusbar=this->statusBar();
    //添加状态栏功能
    QLabel* leftlabel=new QLabel("左侧信息",statusbar);
    statusbar->addWidget(leftlabel);
    QLabel* rightlabel=new QLabel("右侧信息",statusbar);
    statusbar->addWidget(rightlabel);
    //设置停靠部件
    QDockWidget* dockwidget=new QDockWidget("停靠部件",this);
    this->addDockWidget(Qt::BottomDockWidgetArea,dockwidget);
    //添加核心部件
    QTextEdit* text= new QTextEdit(this);
    this->setCentralWidget(text);




}

MainWindow::~MainWindow() {}
