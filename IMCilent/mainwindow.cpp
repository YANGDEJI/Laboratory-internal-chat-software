#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置窗口标题
    setWindowTitle("聊天软件v1.0");

    //初始化垂直布局的层
    m_pLayput =  new QVBoxLayout;
    //设置距离外控件上下左右的间距
    m_pLayput->setContentsMargins(0,0,0,0);
    //设置里面每个空间彼此间的距离
    m_pLayput->setSpacing(3);
    //把层设置到大控件上
    ui->wdg_list->setLayout(m_pLayput);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//添加好友到列表上
void MainWindow::addFriend(userItem *item)
{
    //把小控件添加到层(画布)
    m_pLayput->addWidget(item);
}

//从列表上删除好友
void MainWindow::deleteFriend()
{

}

