#include "ckernel.h"
#include"TcpClientMediator.h"
#include"QMessageBox"

//定义计算数组下标的宏替换公式
#define NetProtocolMap(a) m_protoclMap[a - _DEF_PROTOCOL_BASE - 1]

ckernel::ckernel(QObject *parent) : QObject(parent)
{
    //1.初始化协议头数组
    setprotoclMap();

    //2.给好友列表窗口new对象
    m_mainwindow = new MainWindow;
    //m_mainwindow->showNormal();

    //3.初始化登录/注册界面
    m_pLoginDialog = new LoginDialog;
    m_pLoginDialog->showNormal();

    //绑定注册信号
    connect(m_pLoginDialog , SIGNAL(SIG_RegisterCommit(QString , QString , QString )),
            this , SLOT(slot_RegisterCommit(QString , QString , QString )));
    //绑定登录信号
    connect(m_pLoginDialog , SIGNAL(SIG_LoginCommit(QString , QString)),
            this , SLOT(slot_LoginCommit(QString , QString)));

    //4.给中介者类new对象
    m_pMediator = new TcpClientMediator;

    //5.打开网络
    if(!m_pMediator->OpenNet()){
        QMessageBox::about(m_mainwindow,"提示","打开网络失败");
        exit(0);
    }

    //绑定发送所有接收到的数据的信号和槽
    connect(m_pMediator,SIGNAL(SIG_ReadyData(long, char*, int)),
            this,SLOT(slot_ReadyData(long, char*, int)));
}

ckernel::~ckernel()
{
    //1.回收资源
    if(m_mainwindow){
        m_mainwindow->hide();
        delete(m_mainwindow);
        m_mainwindow = NULL;
    }
    if(m_pMediator){
        m_pMediator->CloseNet();
        delete(m_pMediator);
        m_pMediator = NULL;
    }
    if(m_pLoginDialog){
        m_pLoginDialog->hide();
        delete(m_pLoginDialog);
        m_pLoginDialog = NULL;
    }

    for(auto ite = m_mapIdToChatdlg.begin();ite!=m_mapIdToChatdlg.end();){
        //*ite取出map中内容
        ChatDialog* chat = *ite;
        if(chat){
            delete chat;
            chat = NULL;
        }
        ite = m_mapIdToChatdlg.erase(ite);
    }

}

void ckernel::slot_ReadyData(long lSendIp, char *buf, int nLen)
{
    qDebug()<<__func__;
    //1.取出协议头
    PackType type = *(PackType*)buf;
    //2.判断协议头是否合法并调用
    int index = type-1-_DEF_PROTOCOL_BASE;
    if(0<=index&&index<_DEF_TCP_PROTOCOL_COUNT){
        PFun pf = m_protoclMap[index];
        if(pf){
            (this->*pf)(lSendIp, buf, nLen);
        }else{
            qDebug() << "指针函数是空指针，type:" << type;
        }
    }else{
        qDebug() << "type:" << type;
    }

    //3.回收空间
    delete []buf;
    buf = NULL;
}

//注册请求
void ckernel::slot_RegisterCommit(QString name, QString tel, QString password)
{
    qDebug() << __func__;
    //将注册请求发给服务端
    //1.打包
    STRU_REGISTER_RQ rq;
    strcpy(rq.name , name.toStdString().c_str());
    utf8ToGb2312(name , rq.name , sizeof(name));
    strcpy(rq.tel , tel.toStdString().c_str());
    strcpy(rq.password , password.toStdString().c_str());
    //2.发送
    m_pMediator->SendData(0, (char*)&rq , sizeof(rq) );
}

//登录请求
void ckernel::slot_LoginCommit(QString tel, QString password)
{
    qDebug() << __func__;
    //将登录请求发给服务端
    //1.打包
    STRU_LOGIN_RQ rq;
    strcpy(rq.tel , tel.toStdString().c_str());
    strcpy(rq.password , password.toStdString().c_str());
    //2.发送
    m_pMediator->SendData(0, (char*)&rq , sizeof(rq) );
}

//处理显示聊天窗口的信号
void ckernel::slot_showChatDialog(int id)
{
    qDebug()<<__func__;
    //根据ID找到窗口
    if(m_mapIdToChatdlg.find(id) != m_mapIdToChatdlg.end()){
        ChatDialog* chat = m_mapIdToChatdlg[id];
        chat->showNormal();
    }
}

void ckernel::setprotoclMap()
{
    qDebug() << __func__;
    //1.初始化数组
    memset(m_protoclMap,0,sizeof(PFun)*_DEF_TCP_PROTOCOL_COUNT);
    //2.把处理函数的地址存到协议头对应的数组下标中
    NetProtocolMap(_DEF_PROTOCOL_TCP_REGISTER_RS) = &ckernel::dealRegisterRs;
    NetProtocolMap(_DEF_PROTOCOL_TCP_LOGIN_RS) = &ckernel::dealLoginRs;
    NetProtocolMap(_DEF_PROTOCOL_TCP_FRIEND_INFO) = &ckernel::dealFriendInfo;
}

void ckernel::utf8ToGb2312(QString utf8, char *gb2312, int nLen)
{
    QTextCodec *gb2312Code = QTextCodec::codecForName("gb2312");
    QByteArray ba = gb2312Code->fromUnicode(utf8);
    strcpy_s(gb2312 , nLen , ba.data());
}

QString ckernel::gb2312ToTtf8(char *gbBuf)
{
    QTextCodec *gb2312Code = QTextCodec::codecForName("gb2312");
    return gb2312Code->toUnicode(gbBuf);
}

//处理注册回复
void ckernel::dealRegisterRs(long lSendIp, char *buf, int nLen)
{
    qDebug()<<__func__;
    //拆包
    STRU_REGISTER_RS* rs = (STRU_REGISTER_RS*)buf;
    switch(rs->result){
    case register_success:
        QMessageBox::about(m_pLoginDialog , "提示" , "注册成功");
        break;
    case name_is_exist:
        QMessageBox::about(m_pLoginDialog , "提示" , "昵称存在");
        break;
    case tel_is_exist:
        QMessageBox::about(m_pLoginDialog , "提示" , "号码存在");
        break;
    default:
        break;
    }
}

//处理登录回复
void ckernel::dealLoginRs(long lSendIp, char *buf, int nLen)
{
    qDebug()<<__func__;
    //1.拆包
    STRU_LOGIN_RS* rs = (STRU_LOGIN_RS*)buf;
    switch(rs->result){
    case login_success:
        {
            m_pLoginDialog->hide();
            m_mainwindow->show();
            //TEST
            for(int i=1;i<=5;i++){
                userItem* item = new userItem;
                item->setFriendInfo(i,statue_online , i,QString("用户%1").arg(i),"啥也没写");
                m_mainwindow->addFriend(item);
                //绑定点击用户头像的信号和槽
                connect(item , SIGNAL(SIG_showChatDialog(int)),
                        this , SLOT(slot_showChatDialog(int)));
                //创建与该好友的聊天窗口
                ChatDialog* chat = new ChatDialog;
                //设置聊天窗口属性
                chat->setInfo(QString("用户%1").arg(i),i);
                //把聊天窗口保存到map中
                m_mapIdToChatdlg[i] = chat;
            }
            for(int i=6;i<=10;i++){
                userItem* item = new userItem;
                item->setFriendInfo(i,statue_offline , i,QString("用户%1").arg(i),"啥也没写");
                m_mainwindow->addFriend(item);
                ChatDialog* chat = new ChatDialog;
                chat->setInfo(QString("用户%1").arg(i),i);
                m_mapIdToChatdlg[i] = chat;
            }
        }
        break;
    case password_error:
        QMessageBox::about(m_pLoginDialog , "提示" , "密码错误");
    case user_not_exist:
        QMessageBox::about(m_pLoginDialog , "提示" , "号码不存在");
    default:
        break;
    }
    //2.根据登录结果提示用户（成功要跳转到好友列表中）

}

//处理好友信息
void ckernel::dealFriendInfo(long lSendIp, char *buf, int nLen)
{
    qDebug()<<__func__;
}
