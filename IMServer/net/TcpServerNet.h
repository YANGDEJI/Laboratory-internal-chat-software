#pragma once
#include"INet.h"
#include<map>
#include<list>

class TcpServerNet : public INet {
public:
	TcpServerNet(INetMediator* pMediator);
	~TcpServerNet(); //使用虚析构，父类指针指向子类对象，析构的时候先执行子类析构，再执行父类析构，防止内存泄漏
	//初始化网络
	bool InitNet();
	//关闭网络
	void UninitNet();
	//发送数据
	bool SendData(long lSendIp, char* buf, int nLen);
protected:
	//接收数据
	void RecvData();
	//接收数据的线程
	static unsigned __stdcall RecvThread(void* lpVoid);
	//接受连接的线程
	static unsigned __stdcall AcceptThread(void* lpVoid);
	SOCKET m_sockListen;
	bool m_isStop;
	//定义一个map，key是线程id，value是客户端对应的socket
	map<unsigned int, SOCKET>m_mapThreadIdToSocket;
	//定义一个链表,存储线程的句柄
	list<HANDLE>m_listThreadHandle;
};

