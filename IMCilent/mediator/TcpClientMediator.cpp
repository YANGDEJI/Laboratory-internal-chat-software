#include"TcpClientMediator.h"
#include"TcpClientNet.h"
#include<iostream>

TcpClientMediator::TcpClientMediator()
{
	m_pNet = new TcpClientNet(this);
}
TcpClientMediator::~TcpClientMediator()
{
	if (m_pNet) {
		m_pNet->UninitNet();
		delete m_pNet;
		m_pNet = NULL;
	}
}

//打开网络
bool TcpClientMediator::OpenNet()
{
	if (!m_pNet->InitNet()) {
		return false;
	}
    cout << "OpenNet seccess" <<endl;
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
	if (!m_pNet->SendData(lSendIp, buf, nLen)) {
		return false;
	}
	return true;
}
//接收数据
void TcpClientMediator::DealData(long lSendIp, char* buf, int nLen)
{
	//TODO:传递给kernal类
    Q_EMIT SIG_ReadyData(lSendIp,buf, nLen);
}
