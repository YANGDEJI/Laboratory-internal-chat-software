#pragma once
#include"INet.h"

class UdpNet: public INet{
public:
	UdpNet(INetMediator* pMediator);
	~UdpNet(); //ʹ��������������ָ��ָ���������������ʱ����ִ��������������ִ�и�����������ֹ�ڴ�й©
	//��ʼ������
	bool InitNet();
	//�ر�����
	void UninitNet();
	//��������
	bool SendData(long lSendIp, char* buf, int nLen);
protected:
	//��������
	void RecvData();
	static unsigned __stdcall RecvThread(void* lpVoid);
	HANDLE m_handle;
	SOCKET m_sock;
	bool m_isStop;
};