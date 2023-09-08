#pragma once
#include"INet.h"

class TcpClientNet : public INet {
public:
	TcpClientNet(INetMediator* pMediator);
	~TcpClientNet(); //使用虚析构，父类指针指向子类对象，析构的时候先执行子类析构，再执行父类析构，防止内存泄漏
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
	HANDLE m_handle;
	SOCKET m_sock;
	bool m_isStop;
};
