#include"TcpClientMediator.h"
#include"TcpClientNet.h"

TcpClientMediator::TcpClientMediator()
{
	m_pNet = new TcpClientNet(this);
}
TcpClientMediator::~TcpClientMediator()
{
	if (m_pNet) {
		delete m_pNet;
		m_pNet = NULL;
	}
}
//ʹ��������������ָ��ָ���������������ʱ����ִ��������������ִ�и�����������ֹ�ڴ�й©
//������
bool TcpClientMediator::OpenNet()
{
	if (!m_pNet->InitNet()) {
		cout << "TcpClientMediator::OpenNet fail" << endl;
		return false;
	}
	return true;
}
//�ر�����
void TcpClientMediator::CloseNet()
{
	m_pNet->UninitNet();
}
//��������
bool TcpClientMediator::SendData(long lSendIp, char* buf, int nLen)
{
	if (!m_pNet->SendData(lSendIp,buf,nLen)) {
		cout << "TcpClientMediator::SendData fail" << endl;
		return false;
	}
	return true;
}
//��������
void TcpClientMediator::DealData(long lSendIp, char* buf, int nLen)
{
		//TODO:ͨ���źŰѽ��յ������ݴ���kernel��
	cout << "TcpClientMediator::DealData" << buf << endl;
}
