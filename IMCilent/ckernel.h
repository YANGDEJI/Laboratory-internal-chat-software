#ifndef CKERNEL_H
#define CKERNEL_H

#include <QObject>
#include"mainwindow.h"
#include"INetMediator.h"
#include<QDebug>
#include"packDef.h"
#include<string>
#include"logindialog.h"
#include<QTextCodec>
#include"chatdialog.h"
#include<QMap>

class ckernel;
typedef void (ckernel::*PFun)(long, char*, int);

class ckernel : public QObject
{
    Q_OBJECT
public:
    explicit ckernel(QObject *parent = nullptr);
    ~ckernel();

signals:

public slots:
    //处理所有数据
    void slot_ReadyData(long lSendIp, char* buf, int nLen);
    //注册请求
    void slot_RegisterCommit(QString name , QString tel , QString password);
    //登录请求
    void slot_LoginCommit(QString tel , QString password);
    //显示聊天窗口
    void slot_showChatDialog(int id);

private:
    //初始化协议头函数
    void setprotoclMap();
    //QString(UTF-8)->char*(gb2312)
    void utf8ToGb2312(QString utf8 , char* gb2312 , int);
    //char*(gb2312)->QString(UTF-8)
    QString gb2312ToTtf8(char* gbBuf);
    //处理注册回复
    void dealRegisterRs(long lSendIp, char* buf, int nLen);
    //处理登录回复
    void dealLoginRs(long lSendIp, char* buf, int nLen);
    //处理好友信息
    void dealFriendInfo(long lSendIp, char* buf, int nLen);

private:
   MainWindow* m_mainwindow;
   INetMediator* m_pMediator;
   LoginDialog* m_pLoginDialog;
    //协议头对应的处理函数数组
   PFun m_protoclMap[_DEF_TCP_PROTOCOL_COUNT];
   //保存所有好友的聊天窗口，key是好友id
   QMap<int,ChatDialog*> m_mapIdToChatdlg;
};

#endif // CKERNEL_H
