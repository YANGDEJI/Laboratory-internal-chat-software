#pragma once
#include"INet.h"
#include<map>
#include<list>

class TcpServerNet : public INet {
public:
	TcpServerNet(INetMediator* pMediator);
	~TcpServerNet(); //ʹ��������������ָ��ָ���������������ʱ����ִ��������������ִ�и�����������ֹ�ڴ�й©
	//��ʼ������
	bool InitNet();
	//�ر�����
	void UninitNet();
	//��������
	bool SendData(long lSendIp, char* buf, int nLen);
protected:
	//��������
	void RecvData();
	//�������ݵ��߳�
	static unsigned __stdcall RecvThread(void* lpVoid);
	//�������ӵ��߳�
	static unsigned __stdcall AcceptThread(void* lpVoid);
	SOCKET m_sockListen;
	bool m_isStop;
	//����һ��map��key���߳�id��value�ǿͻ��˶�Ӧ��socket
	map<unsigned int, SOCKET>m_mapThreadIdToSocket;
	//����һ������,�洢�̵߳ľ��
	list<HANDLE>m_listThreadHandle;
};

