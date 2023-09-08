#include "useritem.h"
#include "ui_useritem.h"

userItem::userItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userItem)
{
    ui->setupUi(this);
}

userItem::~userItem()
{
    delete ui;
}

void userItem::setFriendInfo(int id, int state, int iconId, QString name, QString feeling)
{
    //1.保存好友属性
    m_id = id;
    m_state = state;
    m_iconId = iconId;
    m_name = name;
    m_feeling = feeling;

    //2.把属性设置到控件上
    ui->lb_name->setText(m_name);
    ui->lb_feeling->setText(m_feeling);

    //设置头像需要先判断用户状态
    QString iconPath = QString(":/tx/%1.png").arg(m_iconId);
    if(m_state == statue_online){
        //在线亮显
        ui->pb_icon->setIcon(QIcon(iconPath));
    }else{
        //不在线暗显
        QBitmap bmp;
        bmp.load(iconPath);
        ui->pb_icon->setIcon(bmp);
    }
    //3.重绘
    repaint();
}

void userItem::on_pb_icon_clicked()
{
    //点击用户头像发送显示窗口的信号
    Q_EMIT SIG_showChatDialog(m_id);
}

