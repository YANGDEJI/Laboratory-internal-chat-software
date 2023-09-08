#pragma once
#include<iostream>
#include"CMySql.h"
#include"packDef.h"
#include"INetMediator.h"
#include<map>

using namespace std;

//���庯��ָ��
class CKernel;
typedef void(CKernel::* pfun)(long, char*, int);
class CKernel
{
public:
	CKernel();
	~CKernel();
	//�򿪷�����
	bool startServer();
	//�رշ�����
	void closeServer();
	//��ʼ��Э������
	void setProtocolMap();
	//��ѯ��ǰ��¼�û��ĺ�����Ϣ�������Լ���
	void getUserList(int userId);
	//�����û�id��ѯ�û���Ϣ(userInfo���������)
	void getUserInfoById(STRU_FRIEND_INFO* userInfo, int userId);

	//���������յ�������
	void dealData(long lSendIp, char* buf, int nLen);
	//����ע������
	void dealRegisterRq(long lSendIp, char* buf, int nLen);
	//�����¼����
	void dealLoginRq(long lSendIp, char* buf, int nLen);
	//������������
	void dealChatRq(long lSendIp, char* buf, int nLen);
	//������Ӻ�������
	void dealAddFriendRq(long lSendIp, char* buf, int nLen);
	//������Ӻ��ѻظ�
	void dealAddFriendRs(long lSendIp, char* buf, int nLen);
	//������������
	void dealOfflineRq(long lSendIp, char* buf, int nLen);
private:
	INetMediator* m_pMediator;
	CMySql sql;
	//��������飬�±���Э��ͷ����ģ����������������ָ��
	pfun m_netProtocolMap[_DEF_TCP_PROTOCOL_COUNT];
	//�����й�map��������ͻ��˶�Ӧ��socket
	map<int, SOCKET>m_mapIdToSocket;
public:
	static CKernel* pKernel;



};

