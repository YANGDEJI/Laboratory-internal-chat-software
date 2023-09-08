#include"TcpServerMediator.h"
#include"TcpServerNet.h"
#include"../CKernel.h"

TcpServerMediator::TcpServerMediator()
{
	m_pNet = new TcpServerNet(this);
}
TcpServerMediator::~TcpServerMediator()
{
	if (m_pNet) {
		delete m_pNet;
		m_pNet = NULL;
	}
}
//ʹ��������������ָ��ָ���������������ʱ����ִ��������������ִ�и�����������ֹ�ڴ�й©
//������
bool TcpServerMediator::OpenNet()
{
	if (!m_pNet->InitNet()) {
		cout << "TcpServerMediator::OpenNet fail" << endl;
		return false;
	}
	return true;
}
//�ر�����
void TcpServerMediator::CloseNet()
{
	m_pNet->UninitNet();
}
//��������
bool TcpServerMediator::SendData(long lSendIp, char* buf, int nLen)
{
	if (!m_pNet->SendData(lSendIp, buf, nLen)) {
		cout << "TcpServerMediator::SendData fail" << endl;
		return false;
	}
	return true;
}
//��������
void TcpServerMediator::DealData(long lSendIp, char* buf, int nLen)
{
	//�ѽ��յ������ݴ���kernel
	CKernel::pKernel->dealData(lSendIp, buf, nLen);
}
