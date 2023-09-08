#pragma once
#include<iostream>
#include"CMySql.h"
#include"packDef.h"
#include"INetMediator.h"
#include<map>

using namespace std;

//定义函数指针
class CKernel;
typedef void(CKernel::* pfun)(long, char*, int);
class CKernel
{
public:
	CKernel();
	~CKernel();
	//打开服务器
	bool startServer();
	//关闭服务器
	void closeServer();
	//初始化协议数组
	void setProtocolMap();
	//查询当前登录用户的好友信息（包括自己）
	void getUserList(int userId);
	//根据用户id查询用户信息(userInfo是输出参数)
	void getUserInfoById(STRU_FRIEND_INFO* userInfo, int userId);

	//处理所有收到的数据
	void dealData(long lSendIp, char* buf, int nLen);
	//处理注册请求
	void dealRegisterRq(long lSendIp, char* buf, int nLen);
	//处理登录请求
	void dealLoginRq(long lSendIp, char* buf, int nLen);
	//处理聊天请求
	void dealChatRq(long lSendIp, char* buf, int nLen);
	//处理添加好友请求
	void dealAddFriendRq(long lSendIp, char* buf, int nLen);
	//处理添加好友回复
	void dealAddFriendRs(long lSendIp, char* buf, int nLen);
	//处理下线请求
	void dealOfflineRq(long lSendIp, char* buf, int nLen);
private:
	INetMediator* m_pMediator;
	CMySql sql;
	//定义个数组，下标是协议头计算的，数组的内容数函数指针
	pfun m_netProtocolMap[_DEF_TCP_PROTOCOL_COUNT];
	//定义有关map用来管理客户端对应的socket
	map<int, SOCKET>m_mapIdToSocket;
public:
	static CKernel* pKernel;



};

