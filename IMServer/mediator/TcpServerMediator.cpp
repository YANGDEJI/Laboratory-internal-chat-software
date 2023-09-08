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
//使用虚析构，父类指针指向子类对象，析构的时候先执行子类析构，再执行父类析构，防止内存泄漏
//打开网络
bool TcpServerMediator::OpenNet()
{
	if (!m_pNet->InitNet()) {
		cout << "TcpServerMediator::OpenNet fail" << endl;
		return false;
	}
	return true;
}
//关闭网络
void TcpServerMediator::CloseNet()
{
	m_pNet->UninitNet();
}
//发送数据
bool TcpServerMediator::SendData(long lSendIp, char* buf, int nLen)
{
	if (!m_pNet->SendData(lSendIp, buf, nLen)) {
		cout << "TcpServerMediator::SendData fail" << endl;
		return false;
	}
	return true;
}
//接收数据
void TcpServerMediator::DealData(long lSendIp, char* buf, int nLen)
{
	//把接收到的数据传给kernel
	CKernel::pKernel->dealData(lSendIp, buf, nLen);
}
