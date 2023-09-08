#pragma once
#include<string.h>

#define _DEF_UDP_SERVER_PORT (84567)
#define _DEF_NAME_SIZE (1024)
#define _DEF_CONTENT_SIZE (1024)
#define _DEF_TCP_SERVER_PORT (84567)
#define _DEF_TCP_SERVER_IP ("10.3.12.129")
#define _DEF_TCP_PROTOCOL_COUNT (10)

//请求协议头
#define _DEF_PROTOCOL_UDP_BASE (500)
#define _DEF_PROTOCOL_BASE (1000)

//Upd协议头
//上线请求
#define _DEF_PROTOCOL_DUP_ONLINE_RQ (_DEF_PROTOCOL_UDP_BASE+1)
//上线回复
#define _DEF_PROTOCOL_DUP_ONLINE_BS (_DEF_PROTOCOL_UDP_BASE+2)
//下线请求
#define _DEF_PROTOCOL_DUP_OFFLINE_RQ (_DEF_PROTOCOL_UDP_BASE+3)
//聊天请求
#define _DEF_PROTOCOL_DUP_CHAT_RQ (_DEF_PROTOCOL_UDP_BASE+4)


//请求结构体

//上线请求&回复:协议头、主机名、IP地址(receivefrom函数里已经有发送端的ip地址了，不需要在结构体里再发过去了)
struct STRU_ONLINE {
    STRU_ONLINE() :nType(_DEF_PROTOCOL_DUP_ONLINE_RQ)
    {
        memset(name, 0, _DEF_NAME_SIZE);
    }
    int nType;
    char name[_DEF_NAME_SIZE];
    //long ip;
};
//上线回复:协议头、主机名、IP地址


//下线请求：协议头
struct STRU_OFFLINE_RQ {
    STRU_OFFLINE_RQ():nType(_DEF_PROTOCOL_DUP_OFFLINE_RQ)
    {}
    int nType;
    //long ip;
};
//聊天请求:协议头、聊天内容
struct STRU_CHAT_RQ {
    STRU_CHAT_RQ() :nType(_DEF_PROTOCOL_DUP_CHAT_RQ)
    {
        memset(content, 0, _DEF_NAME_SIZE);
    }
    int nType;
    char content[_DEF_CONTENT_SIZE];
};

//TCP协议头
//注册请求
#define _DEF_PROTOCOL_TCP_REGISTER_RQ   (_DEF_PROTOCOL_BASE + 1)

//注册回复
#define _DEF_PROTOCOL_TCP_REGISTER_RS   (_DEF_PROTOCOL_BASE + 2)

//登录请求
#define _DEF_PROTOCOL_TCP_LOGIN_RQ   (_DEF_PROTOCOL_BASE + 3)

//登录回复
#define _DEF_PROTOCOL_TCP_LOGIN_RS   (_DEF_PROTOCOL_BASE + 4)

//好友信息
#define _DEF_PROTOCOL_TCP_FRIEND_INFO   (_DEF_PROTOCOL_BASE + 5)

//聊天请求
#define _DEF_PROTOCOL_TCP_CHAT_RQ   (_DEF_PROTOCOL_BASE + 6)

//聊天回复
#define _DEF_PROTOCOL_TCP_CHAT_RS   (_DEF_PROTOCOL_BASE + 7)

//添加好友请求
#define _DEF_PROTOCOL_TCP_ADD_FRIEND_RQ   (_DEF_PROTOCOL_BASE + 8)

//添加好友回复
#define _DEF_PROTOCOL_TCP_ADD_FRIEND_RS   (_DEF_PROTOCOL_BASE + 9)

//下线
#define _DEF_PROTOCOL_TCP_OFFLINE_RQ   (_DEF_PROTOCOL_BASE + 10)

//请求结果宏定义
//注册结果
#define register_success   (0)
#define name_is_exist      (1)
#define tel_is_exist       (2)
//登录结果
#define login_success      (0)
#define password_error     (1)
#define user_not_exist     (2)
//用户状态
#define statue_online      (0)
#define statue_offline     (1)
//聊天请求结果
#define send_success       (0)
#define send_fail          (1)
//添加好友请求结果
#define add_success        (0)
#define no_this_user       (1)
#define user_offline       (2)
#define user_refuse        (3)


//TCP请求结构体
typedef int PackType;
//注册请求:协议头、昵称、密码、手机号
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

//注册回复:协议头、注册结果
typedef struct STRU_REGISTER_RS {
    STRU_REGISTER_RS() :type(_DEF_PROTOCOL_TCP_REGISTER_RS), result(tel_is_exist)
    {

    }
    PackType type;
    int result;
}STRU_REGISTER_RS;

//登录请求:协议头、电话号码、密码
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


//登录回复:协议头、登录结果、自己的id
typedef struct STRU_LOGIN_RS {
    STRU_LOGIN_RS() :type(_DEF_PROTOCOL_TCP_LOGIN_RS), result(user_not_exist), userId(0)
    {
    }
    PackType type;
    int result;
    int userId;
}STRU_LOGIN_RS;

//好友信息:协议头、用户ID、昵称、头像ID、在线状态、签名
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

//聊天请求:协议头、自己的id、聊天内容、好友id
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

//聊天回复:协议头、自己的id、好友的id、请求结果
typedef struct STRU_TCP_CHAT_RS {
    STRU_TCP_CHAT_RS() :type(_DEF_PROTOCOL_TCP_CHAT_RS), userId(0), friendId(0),result(send_fail)
    {
    }
    PackType type;
    int userId;
    int friendId;
    int result;
}STRU_TCP_CHAT_RS;

//添加好友请求:协议头、自己的id、好友的昵称、自己的昵称
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

//添加好友回复:协议头、自己的id、好友的id、添加结果、好友的昵称
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

//下线:协议头、自己的id、
typedef struct STRU_TCP_OFFLINE_RQ {
    STRU_TCP_OFFLINE_RQ() :type(_DEF_PROTOCOL_TCP_OFFLINE_RQ),userId(0)
    {
    }
    PackType type;
    int userId;
}STRU_TCP_OFFLINE_RQ;
