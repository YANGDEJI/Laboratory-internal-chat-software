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
//使用虚析构，父类指针指向子类对象，析构的时候先执行子类析构，再执行父类析构，防止内存泄漏
//打开网络
bool TcpClientMediator::OpenNet()
{
	if (!m_pNet->InitNet()) {
		cout << "TcpClientMediator::OpenNet fail" << endl;
		return false;
	}
	return true;
}
//关闭网络
void TcpClientMediator::CloseNet()
{
	m_pNet->UninitNet();
}
//发送数据
bool TcpClientMediator::SendData(long lSendIp, char* buf, int nLen)
{
	if (!m_pNet->SendData(lSendIp,buf,nLen)) {
		cout << "TcpClientMediator::SendData fail" << endl;
		return false;
	}
	return true;
}
//接收数据
void TcpClientMediator::DealData(long lSendIp, char* buf, int nLen)
{
		//TODO:通过信号把接收到的数据传给kernel类
	cout << "TcpClientMediator::DealData" << buf << endl;
}
