#pragma once
#include"INet.h"

class TcpClientNet : public INet {
public:
	TcpClientNet(INetMediator* pMediator);
	~TcpClientNet(); //ʹ��������������ָ��ָ���������������ʱ����ִ��������������ִ�и�����������ֹ�ڴ�й©
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
