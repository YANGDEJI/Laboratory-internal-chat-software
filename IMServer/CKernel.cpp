#include "CKernel.h"
#include"TcpServerMediator.h"
#include<list>

//定义计算数组下标的宏替换
#define NetProtocolMap(a) m_netProtocolMap[a - _DEF_PROTOCOL_BASE - 10]

//静态成员变量类外初始化
CKernel* CKernel::pKernel = NULL;

CKernel::CKernel() {
	pKernel = this;
	setProtocolMap();
}

CKernel::~CKernel() {
	closeServer();
}

//打开服务器：打开网络，连接上数据库
bool CKernel::startServer(){
	//1.打开网络
	m_pMediator = new TcpServerMediator;
	if (!m_pMediator->OpenNet()) {
		cout << "打开网络失败" << endl;
		return false;
	}
	//2.连接上数据库
	if (!sql.ConnectMySql("127.0.0.1","root","157140","0321im")) {
        cout << "连接数据库失败" << endl;
        return false;
	}
	return true;
}

//关闭服务器
void CKernel::closeServer() {
	//回收资源
	if (m_pMediator) {
		m_pMediator->CloseNet();
		delete m_pMediator;
		m_pMediator = NULL;
	}
}

//初始化协议数组
void CKernel::setProtocolMap()
{
    //1.初始化数组
    memset(m_netProtocolMap, 0, sizeof(pfun) * _DEF_TCP_PROTOCOL_COUNT);

    //2.绑定协议头和处理函数（把处理函数对应的下标里）
    NetProtocolMap(_DEF_PROTOCOL_TCP_REGISTER_RQ) = &CKernel::dealRegisterRq;
    NetProtocolMap(_DEF_PROTOCOL_TCP_LOGIN_RQ) = &CKernel::dealLoginRq;
    NetProtocolMap(_DEF_PROTOCOL_TCP_CHAT_RQ) = &CKernel::dealChatRq;
    NetProtocolMap(_DEF_PROTOCOL_TCP_ADD_FRIEND_RQ) = &CKernel::dealAddFriendRq;
    NetProtocolMap(_DEF_PROTOCOL_TCP_ADD_FRIEND_RS) = &CKernel::dealAddFriendRs;
    NetProtocolMap(_DEF_PROTOCOL_TCP_OFFLINE_RQ) = &CKernel::dealOfflineRq;

}


//处理所有接收到的数据
void CKernel::dealData(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealData" << endl;
    //1.取出协议头
    int type = *(int*)buf;

    //2.根据协议头判断是哪个结构体，走对应的处理流程
    //2.1、判断协议头是否在有效范围内
    if ((type - _DEF_PROTOCOL_BASE - 10) >= 0 &&
        (type - _DEF_PROTOCOL_BASE - 10) < _DEF_TCP_PROTOCOL_COUNT) {
        //2.2通过计算数组下标，取出函数地址
        pfun pf = NetProtocolMap(type);
        //2.3调用函数
        if (pf) {
            (this->*pf)(lSendIp, buf, nLen);
        }
        else {
            cout << "type:" << type << endl;
        }
    }
    //3.处理完数据，回收空间
    delete[] buf;
}

//处理注册请求
void CKernel::dealRegisterRq(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealRegisterRq" << endl;
    //1.拆包
    STRU_REGISTER_RQ* rq = (STRU_REGISTER_RQ*)buf;
    cout << "tel:" << rq->tel << ",name:" << rq->name << ",password:" << rq->password << endl;

    //2.校验用户输入是否合法，校验规则与客户端一致
    //（因为收到的请求可能是其他人仿冒的非法请求，所以客户端和服务器都必须校验）--服务端不写了

    STRU_REGISTER_RS rs;
    //3.根据电话号码查询数据库中是否已经有这个人了
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "select tel from t_user where tel = '%s';", rq->tel);
    if (!sql.SelectMySql(sqlBuf,1,resultList)) {
        cout << "查询数据库失败,sql:" << sqlBuf << endl;
        return;
    }
    //4.遍历查询结果
    if (resultList.size() > 0) {
        //已经有人使用过这个电话号码注册了，设置注册回复错误码
        rs.result = tel_is_exist;
    }
    else {
        //没有人使用这个电话号码
        //5.根据昵称查阅数据库中是否已经有这个人了
        sprintf_s(sqlBuf, "select name from t_user where name = '%s';", rq->name);
        if (!sql.SelectMySql(sqlBuf, 1, resultList)) {
            cout << "查询数据库失败,sql:" << sqlBuf << endl;
            return;
        }
        //6.遍历查询结果
        if (resultList.size() > 0) {
            //此昵称已经被别人使用了，设置注册回复错误码
            rs.result = name_is_exist;
        }
        else {
            //手机号和昵称都没有被注册过。可以注册
            //7.把这个人的注册信息写入数据库
            sprintf_s(sqlBuf, "insert into t_user(tel,name,password,icon,feeling)values('%s','%s','%s',1,'啥也没写');",
                    rq->tel,rq->name,rq->password);
            if (!sql.UpdateMySql(sqlBuf)) {
                cout << "插入数据库失败,sql:" << sqlBuf << endl;
            }
            //8.设置注册回复结果成功
            rs.result = register_success;
        }
    }
    //9.回复客户端注册回复
    m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));
}

//处理登录请求
void CKernel::dealLoginRq(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealLoginRq" << endl;
    //1.拆包
    STRU_LOGIN_RQ* rq = (STRU_LOGIN_RQ*)buf;

    //2.校验用户输入是否合法，校验规则与客户端一致
    //（因为收到的请求可能是其他人仿冒的非法请求，所以客户端和服务器都必须校验）--服务端不写了

    //3.根据tel查密码
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "select id,password from t_user where tel = '%s';", rq->tel);
    if (!sql.SelectMySql(sqlBuf, 2, resultList)) {
        cout << "查询数据库失败,sql:" << sqlBuf << endl;
        return;
    }

    STRU_LOGIN_RS rs;
    //4.如果查询结果为空
    if (0==resultList.size()) {
        //用户不存在，设置登录回复结果为失败
        rs.result = user_not_exist;
        //7.回复给客户端一个登录回复
        m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));
    }else {
        //5.如果查询结果不为空，取出密码和用户id和密码
        int userId = atoi(resultList.front().c_str());
        resultList.pop_front();

        string pas = resultList.front();
        resultList.pop_front();

        //6.与用户输入的密码比较，是否相同
        if (0==strcmp(rq->password,pas.c_str())) {
            //密码相同，登录成功
            rs.result = login_success;
            rs.userId = userId;

            //保存当前登录成功用户的socket
            m_mapIdToSocket[userId] = lSendIp;

            //7.回复给客户端一个登录回复
            m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));

            //查询当前登录用户的好友信息（包括自己的信息）
            getUserList(userId);
            
        }else {
            //密码不同，登录失败
            rs.result = password_error;

            //7.回复给客户端一个登录回复
            m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));
        }
    } 


}

//查询当前登录用户的好友信息（包括自己）
void CKernel::getUserList(int userId) {
    //1.查询自己的信息
    STRU_FRIEND_INFO userInfo;
    getUserInfoById(&userInfo, userId);

    //2.用userId取出socket，把自己的信息发给客户端
    if (m_mapIdToSocket.find(userId)== m_mapIdToSocket.end()) {
        cout << "CKernel::getUserList获取socket失败，userId=" << userId << endl;
        return;
    }
    m_pMediator->SendData(m_mapIdToSocket[userId],(char*)&userInfo,sizeof(userInfo));
    //3.先查出有哪些好友
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "select idB from t_friend where idA = '%d';",userId);
    if (!sql.SelectMySql(sqlBuf, 1, resultList)) {
        cout << "查询数据库失败,sql:" << sqlBuf << endl;
        return;
    }
    //4.根据好友的id列表，查询出每个好友的信息
    int friendId = 0;
    STRU_FRIEND_INFO friendInfo;
    while (resultList.size() > 0) {
        //5.取出好友的id
        friendId = atoi(resultList.front().c_str());
        resultList.pop_front();

        //6.根据好友的id查询好友的信息
        getUserInfoById(&friendInfo, friendId);

        //7.把好友的信息发给登录客户端
        m_pMediator->SendData(m_mapIdToSocket[userId], (char*)&friendInfo, sizeof(friendInfo));

        //8.判断好友是否在线
        if (m_mapIdToSocket.find(friendId) != m_mapIdToSocket.end()) {
            //9.好友在线，给好友发送登录人的信息
            m_pMediator->SendData(m_mapIdToSocket[friendId], (char*)&userInfo, sizeof(userInfo));
        }
    }
}

//根据用户id查询用户信息(userInfo是输出参数)
void CKernel::getUserInfoById(STRU_FRIEND_INFO* userInfo, int userId) {
    
    //1.设置用户id
    userInfo->userId = userId;

    //2.设置用户状态
    if (m_mapIdToSocket.find(userId)!= m_mapIdToSocket.end()){
        userInfo->state = statue_online;
    }
    else {
        userInfo->state = statue_offline;
    }

    //3.从数据库查询用户的其他信息
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "select name,icon,feeling from t_user where id = '%d';", userId);
    if (!sql.SelectMySql(sqlBuf, 3, resultList)) {
        cout << "查询数据库失败,sql:" << sqlBuf << endl;
        return;
    }

    //4.获取查询结果，设置到结构体中
    if (3 == resultList.size()) {
        //先取名字
        strcpy_s(userInfo->name, resultList.front().c_str());
        resultList.pop_front();
        //再去icon
        userInfo->iconId = atoi(resultList.front().c_str());
        resultList.pop_front();
        //最后取签名
        strcpy_s(userInfo->feeling, resultList.front().c_str());
        resultList.pop_front();
    }
}

//处理聊天请求
void CKernel::dealChatRq(long lSendIp, char* buf, int nLen) {
    cout << "CKernel::dealChatRq" << endl;
    //1.拆包
    STRU_TCP_CHAT_RQ* rq = (STRU_TCP_CHAT_RQ*)buf;
    cout << "userid:" << rq->userId << ",friendid:" << rq->friendId << ",content:" << rq->content;

    //2.查看好友是否在线
    if (m_mapIdToSocket.count(rq->friendId)>0) {
        //好友在线，就把聊天内容转发给好友
        m_pMediator->SendData(m_mapIdToSocket[rq->friendId], buf, nLen);
    }
    else {
        //好友不在线，回复发送端好友不在线
        STRU_TCP_CHAT_RS rs;
        rs.result = send_fail;
        rs.userId = rq->userId;
        rs.friendId = rq->friendId;
        m_pMediator->SendData(lSendIp,(char*)&rs,sizeof(rs));
    }
}

//处理添加好友请求
void CKernel::dealAddFriendRq(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealAddFriendRq" << endl;
    //1.拆包
    STRU_ADD_FRIEND_RQ* rq = (STRU_ADD_FRIEND_RQ*)buf;

    //2.查看数据库里面有没有这个待添加的用户
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "select id from t_user where name = '%s';", rq->friendName);
    if (!sql.SelectMySql(sqlBuf, 1, resultList)) {
        cout << "查询数据库失败，sql:" << sqlBuf << endl;
        return;
    }
    STRU_ADD_FRIEND_RS rs;
    if (resultList.size() > 0) {
        //如果有这个待添加的用户，取出待添加用户的id
        int friendId = atoi(resultList.front().c_str());
        resultList.pop_front();

        //3.查看待添加用户是否在线
        if (m_mapIdToSocket.count(friendId) > 0) {
            //4.如果待添加用户在线，就把添加好友的请求转发给待添加用户
            m_pMediator->SendData(m_mapIdToSocket[friendId], buf, nLen);
        }
        else {
            //5.如果待添加用户不在线,返回给客户端添加好友失败，原因不在线
            rs.result = user_offline;
            rs.friendId = friendId;
            strcpy_s(rs.friendName, rq->friendName);
            rs.userId = rq->userId;
            m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));
        }
    }
    else {
    //6.没有这个待添加用户，返回给客户端添加好友失败，原因用户不存在
        rs.result = no_this_user;
        strcpy_s(rs.friendName, rq->friendName);
        rs.userId = rq->userId;
        m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));
    }
}

//处理添加好友回复
void CKernel::dealAddFriendRs(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealAddFriendRs" << endl;
    //1.拆包
    STRU_ADD_FRIEND_RS* rs = (STRU_ADD_FRIEND_RS*)buf;

    //2.如果对方同意添加好友，就把好友关系写入数据库(写入两次，为了保证都成功，或者都失败，需要使用事务)
    if (add_success == rs->result) {
        char sqlBuf[1024] = "";
        sprintf_s(sqlBuf, "insert into t_friend values ('%d','%d');", rs->userId, rs->friendId);
        if (!sql.UpdateMySql(sqlBuf)) {
            cout << "插入数据库失败，sql:" << sqlBuf << endl;
            return;
        }
        sprintf_s(sqlBuf, "insert into t_friend values ('%d','%d');", rs->friendId, rs->userId);
        if (!sql.UpdateMySql(sqlBuf)) {
            cout << "插入数据库失败，sql:" << sqlBuf << endl;
            return;
        }
        //更新好友列表
        getUserList(rs->friendId);
    }

    //3.不管同意还是拒绝添加好友，都要给A客户端转发添加好友回复的结构体
    m_pMediator->SendData(m_mapIdToSocket[rs->userId], buf, nLen);
}

//处理下线请求
void CKernel::dealOfflineRq(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealOfflineRq" << endl;
    //1.拆包
    STRU_TCP_OFFLINE_RQ* rq = (STRU_TCP_OFFLINE_RQ*)buf;

    //2.查询下线用户的好友id列表
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "selsct idB from t_friend where idA = '%d';", rq->userId);
    if (!sql.SelectMySql(sqlBuf, 1, resultList)) {
        cout << "查询数据库失败，sql:" << sqlBuf << endl;
        return;
    }

    //3.遍历好友id列表
    int friendId = 0;
    while (resultList.size() > 0) {
    //取出好友id
        friendId = atoi(resultList.front().c_str());
        resultList.pop_front();

        //4.如果好友在线，给好友转发下线请求
        if (m_mapIdToSocket.count(friendId) > 0) {
            m_pMediator->SendData(m_mapIdToSocket[friendId], buf, nLen);
        }
    }

    //5,把线下用户的socket关闭，并从map中移除
    if (m_mapIdToSocket.count(rq->userId) > 0) {
        closesocket(m_mapIdToSocket[rq->userId]);
        m_mapIdToSocket.erase(rq->userId);
    }
}