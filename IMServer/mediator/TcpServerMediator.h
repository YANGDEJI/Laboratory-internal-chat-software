#pragma once
#include"INetMediator.h"

class TcpServerMediator :public INetMediator {
public:
	TcpServerMediator();
	~TcpServerMediator(); //ʹ��������������ָ��ָ���������������ʱ����ִ��������������ִ�и�����������ֹ�ڴ�й©
	//������
	bool OpenNet();
	//�ر�����
	void CloseNet();
	//��������
	bool SendData(long lSendIp, char* buf, int nLen);
	//��������
	void DealData(long lSendIp, char* buf, int nLen);
};
