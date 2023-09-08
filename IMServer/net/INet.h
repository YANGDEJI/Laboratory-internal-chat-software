#pragma once
#include<iostream>
#include<winsock.h>
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

class INetMediator;

class INet {
public:
	INet() {}
	virtual ~INet() {} //使用虚析构，父类指针指向子类对象，析构的时候先执行子类析构，再执行父类析构，防止内存泄漏
	//初始化网络
	virtual bool InitNet()=0;
	//关闭网络
	virtual void UninitNet()=0;
	//发送数据
	virtual bool SendData(long lSendIp,char* buf,int nLen)=0;
protected:
	//接收数据
	virtual void RecvData()=0;
	INetMediator* m_pMediator;
};