#pragma once
#include<QObject>

class INet; //声明有INet这个类

class INetMediator:public QObject {
public:
	INetMediator() {}
	virtual ~INetMediator() {} //使用虚析构，父类指针指向子类对象，析构的时候先执行子类析构，再执行父类析构，防止内存泄漏
	//初始化网络
	virtual bool OpenNet() = 0;
	//关闭网络
	virtual void CloseNet() = 0;
	//发送数据
	virtual bool SendData(long lSendIp, char* buf, int nLen) = 0;
	//接收数据
	virtual void DealData(long lSendIp, char* buf, int nLen) = 0;
protected:
	INet* m_pNet;
	
};
