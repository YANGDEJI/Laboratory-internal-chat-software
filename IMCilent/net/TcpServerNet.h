#pragma once
#include"INet.h"

class TcpServerNet: public INet{
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
	static unsigned __stdcall RecvThread(void* lpVoid);
	static unsigned __stdcall AcceptThread(void* lpVoid);
	list<HANDLE> m_handlelist;
	SOCKET m_sock;
	bool m_isStop;
	map<unsigned int, SOCKET> m_threadidToSocket;
};