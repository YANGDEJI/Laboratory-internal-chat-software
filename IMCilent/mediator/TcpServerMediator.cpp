#include"TcpServerMediator.h"
#include"TcpServerNet.h"
#include<iostream>

TcpServerMediator::TcpServerMediator()
{
	m_pNet = new TcpServerNet(this);
}
TcpServerMediator::~TcpServerMediator()
{
	if (m_pNet) {
		m_pNet->UninitNet();
		delete m_pNet;
		m_pNet = NULL;
	}
} 

//打开网络
bool TcpServerMediator::OpenNet()
{
    cout << __func__ <<  endl;
	if (!m_pNet->InitNet()) {
		return false;
	}
	return true;
}

//关闭网络
void TcpServerMediator::CloseNet()
{
    cout << __func__ <<  endl;
	m_pNet->UninitNet();
}
//发送数据
bool TcpServerMediator::SendData(long lSendIp, char* buf, int nLen)
{
    cout << __func__ <<  endl;
	if (!m_pNet->SendData(lSendIp,buf,nLen)) {
		return false;
	}
	return true;
}
//接收数据 lSendIp是刚刚给你发消息的人的sock
void TcpServerMediator::DealData(long lSendIp, char* buf, int nLen)
{
    //传递给kernal类

	//测试代码：打印出接收到数据的内容
	cout << __func__ << buf << endl;
	//给客户端回一个消息
	SendData(lSendIp, "TcpServerMediator::DealData", sizeof("TcpServerMediator::DealData"));
}
