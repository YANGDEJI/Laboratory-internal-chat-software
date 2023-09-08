#pragma once

#define _DEF_UDP_SERVER_PORT (84567)
#define _DEF_NAME_SIZE (1024)
#define _DEF_CONTENT_SIZE (1024)
#define _DEF_TCP_SERVER_PORT (84567)
#define _DEF_TCP_SERVER_IP ("192.168.215.1")
#define _DEF_TCP_PROTOCOL_COUNT (10)

//����Э��ͷ
#define _DEF_PROTOCOL_BASE (1000)

//UpdЭ��ͷ
//��������
#define _DEF_PROTOCOL_DUP_ONLINE_RQ (_DEF_PROTOCOL_BASE+1)
//���߻ظ�
#define _DEF_PROTOCOL_DUP_ONLINE_BS (_DEF_PROTOCOL_BASE+2)
//��������
#define _DEF_PROTOCOL_DUP_OFFLINE_RQ (_DEF_PROTOCOL_BASE+3)
//��������
#define _DEF_PROTOCOL_DUP_CHAT_RQ (_DEF_PROTOCOL_BASE+4)


//����ṹ��

//��������&�ظ�:Э��ͷ����������IP��ַ(receivefrom�������Ѿ��з��Ͷ˵�ip��ַ�ˣ�����Ҫ�ڽṹ�����ٷ���ȥ��)
struct STRU_ONLINE {
	STRU_ONLINE() :nType(_DEF_PROTOCOL_DUP_ONLINE_RQ) 
	{
		memset(name, 0, _DEF_NAME_SIZE);
	}
	int nType;
	char name[_DEF_NAME_SIZE];
	//long ip;
};
//���߻ظ�:Э��ͷ����������IP��ַ


//��������Э��ͷ
struct STRU_OFFLINE_RQ {
	STRU_OFFLINE_RQ():nType(_DEF_PROTOCOL_DUP_OFFLINE_RQ) 
	{}
	int nType;
	//long ip;
};
//��������:Э��ͷ����������
struct STRU_CHAT_RQ {
	STRU_CHAT_RQ() :nType(_DEF_PROTOCOL_DUP_CHAT_RQ)
	{
		memset(content, 0, _DEF_NAME_SIZE);
	}
	int nType;
	char content[_DEF_CONTENT_SIZE];
};

//TCPЭ��ͷ
//ע������
#define _DEF_PROTOCOL_TCP_REGISTER_RQ   (_DEF_PROTOCOL_BASE + 10)

//ע��ظ�
#define _DEF_PROTOCOL_TCP_REGISTER_RS   (_DEF_PROTOCOL_BASE + 11)

//��¼����
#define _DEF_PROTOCOL_TCP_LOGIN_RQ   (_DEF_PROTOCOL_BASE + 12)

//��¼�ظ�
#define _DEF_PROTOCOL_TCP_LOGIN_RS   (_DEF_PROTOCOL_BASE + 13)

//������Ϣ
#define _DEF_PROTOCOL_TCP_FRIEND_INFO   (_DEF_PROTOCOL_BASE + 14) 

//��������
#define _DEF_PROTOCOL_TCP_CHAT_RQ   (_DEF_PROTOCOL_BASE + 15)

//����ظ�
#define _DEF_PROTOCOL_TCP_CHAT_RS   (_DEF_PROTOCOL_BASE + 16)

//��Ӻ�������
#define _DEF_PROTOCOL_TCP_ADD_FRIEND_RQ   (_DEF_PROTOCOL_BASE + 17)

//��Ӻ��ѻظ�
#define _DEF_PROTOCOL_TCP_ADD_FRIEND_RS   (_DEF_PROTOCOL_BASE + 18)

//����
#define _DEF_PROTOCOL_TCP_OFFLINE_RQ   (_DEF_PROTOCOL_BASE + 19)

//�������궨��
//ע����
#define register_success   (0)
#define name_is_exist      (1)
#define tel_is_exist       (2)
//��¼���
#define login_success      (0)
#define password_error     (1)
#define user_not_exist     (2)
//�û�״̬
#define statue_online      (0)
#define statue_offline     (1)
//����������
#define send_success       (0)
#define send_fail          (1)
//��Ӻ���������
#define add_success        (0)
#define no_this_user       (1)
#define user_offline       (2)
#define user_refuse        (3)


//TCP����ṹ��
typedef int PackType;
//ע������:Э��ͷ���ǳơ����롢�ֻ���
typedef struct STRU_REGISTER_RQ {
	STRU_REGISTER_RQ() :type(_DEF_PROTOCOL_TCP_REGISTER_RQ)
	{
		memset(name, 0, _DEF_NAME_SIZE);
		memset(password, 0, _DEF_NAME_SIZE);
		memset(tel, 0, _DEF_NAME_SIZE);
	}
	PackType type;
	char name[_DEF_NAME_SIZE];
	char password[_DEF_NAME_SIZE];
	char tel[_DEF_NAME_SIZE];
}STRU_REGISTER_RQ;

//ע��ظ�:Э��ͷ��ע����
typedef struct STRU_REGISTER_RS {
	STRU_REGISTER_RS() :type(_DEF_PROTOCOL_TCP_REGISTER_RS), result(tel_is_exist)
	{

	}
	PackType type;
	int result;
}STRU_REGISTER_RS;

//��¼����:Э��ͷ���绰���롢����
typedef struct STRU_LOGIN_RQ {
	STRU_LOGIN_RQ() :type(_DEF_PROTOCOL_TCP_LOGIN_RQ)
	{
		memset(password, 0, _DEF_NAME_SIZE);
		memset(tel, 0, _DEF_NAME_SIZE);
	}
	PackType type;
	char password[_DEF_NAME_SIZE];
	char tel[_DEF_NAME_SIZE];
}STRU_LOGIN_RQ;


//��¼�ظ�:Э��ͷ����¼������Լ���id
typedef struct STRU_LOGIN_RS {
	STRU_LOGIN_RS() :type(_DEF_PROTOCOL_TCP_LOGIN_RS), result(user_not_exist), userId(0)
	{
	}
	PackType type;
	int result;
	int userId;
}STRU_LOGIN_RS;

//������Ϣ:Э��ͷ���û�ID���ǳơ�ͷ��ID������״̬��ǩ��
typedef struct STRU_FRIEND_INFO {
	STRU_FRIEND_INFO() :type(_DEF_PROTOCOL_TCP_FRIEND_INFO), userId(0), iconId(0),state(statue_offline)
	{
		memset(name, 0, _DEF_NAME_SIZE);
		memset(feeling, 0, _DEF_NAME_SIZE);
	}
	PackType type;
	int userId;
	int iconId;
	int state;
	char name[_DEF_NAME_SIZE];
	char feeling[_DEF_NAME_SIZE];
}STRU_FRIEND_INFO;

//��������:Э��ͷ���Լ���id���������ݡ�����id
typedef struct STRU_TCP_CHAT_RQ {
	STRU_TCP_CHAT_RQ() :type(_DEF_PROTOCOL_TCP_CHAT_RQ), userId(0), friendId(0)
	{
		memset(content, 0, _DEF_CONTENT_SIZE);
	}
	PackType type;
	int userId;
	int friendId;
	char content[_DEF_CONTENT_SIZE];
}STRU_TCP_CHAT_RQ;

//����ظ�:Э��ͷ���Լ���id�����ѵ�id��������
typedef struct STRU_TCP_CHAT_RS {
	STRU_TCP_CHAT_RS() :type(_DEF_PROTOCOL_TCP_CHAT_RS), userId(0), friendId(0),result(send_fail)
	{
	}
	PackType type;
	int userId;
	int friendId;
	int result;
}STRU_TCP_CHAT_RS;

//��Ӻ�������:Э��ͷ���Լ���id�����ѵ��ǳơ��Լ����ǳ�
typedef struct STRU_ADD_FRIEND_RQ {
	STRU_ADD_FRIEND_RQ() :type(_DEF_PROTOCOL_TCP_ADD_FRIEND_RQ), userId(0)
	{
		memset(userName, 0, _DEF_CONTENT_SIZE);
		memset(friendName, 0, _DEF_CONTENT_SIZE);
	}
	PackType type;
	int userId;
	char userName[_DEF_NAME_SIZE];
	char friendName[_DEF_NAME_SIZE];
}STRU_ADD_FRIEND_RQ;

//��Ӻ��ѻظ�:Э��ͷ���Լ���id�����ѵ�id����ӽ�������ѵ��ǳ�
typedef struct STRU_ADD_FRIEND_RS {
	STRU_ADD_FRIEND_RS() :type(_DEF_PROTOCOL_TCP_ADD_FRIEND_RS), userId(0),friendId(0),result(user_refuse)
	{
		memset(friendName, 0, _DEF_NAME_SIZE);
	}
	PackType type;
	int userId;
	int friendId;
	int result;
	char friendName[_DEF_NAME_SIZE];
}STRU_ADD_FRIEND_RS;

//����:Э��ͷ���Լ���id��
typedef struct STRU_TCP_OFFLINE_RQ {
	STRU_TCP_OFFLINE_RQ() :type(_DEF_PROTOCOL_TCP_OFFLINE_RQ),userId(0)
	{
	}
	PackType type;
	int userId;
}STRU_TCP_OFFLINE_RQ;