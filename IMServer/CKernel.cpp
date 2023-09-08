#include "CKernel.h"
#include"TcpServerMediator.h"
#include<list>

//������������±�ĺ��滻
#define NetProtocolMap(a) m_netProtocolMap[a - _DEF_PROTOCOL_BASE - 10]

//��̬��Ա���������ʼ��
CKernel* CKernel::pKernel = NULL;

CKernel::CKernel() {
	pKernel = this;
	setProtocolMap();
}

CKernel::~CKernel() {
	closeServer();
}

//�򿪷������������磬���������ݿ�
bool CKernel::startServer(){
	//1.������
	m_pMediator = new TcpServerMediator;
	if (!m_pMediator->OpenNet()) {
		cout << "������ʧ��" << endl;
		return false;
	}
	//2.���������ݿ�
	if (!sql.ConnectMySql("127.0.0.1","root","157140","0321im")) {
        cout << "�������ݿ�ʧ��" << endl;
        return false;
	}
	return true;
}

//�رշ�����
void CKernel::closeServer() {
	//������Դ
	if (m_pMediator) {
		m_pMediator->CloseNet();
		delete m_pMediator;
		m_pMediator = NULL;
	}
}

//��ʼ��Э������
void CKernel::setProtocolMap()
{
    //1.��ʼ������
    memset(m_netProtocolMap, 0, sizeof(pfun) * _DEF_TCP_PROTOCOL_COUNT);

    //2.��Э��ͷ�ʹ��������Ѵ�������Ӧ���±��
    NetProtocolMap(_DEF_PROTOCOL_TCP_REGISTER_RQ) = &CKernel::dealRegisterRq;
    NetProtocolMap(_DEF_PROTOCOL_TCP_LOGIN_RQ) = &CKernel::dealLoginRq;
    NetProtocolMap(_DEF_PROTOCOL_TCP_CHAT_RQ) = &CKernel::dealChatRq;
    NetProtocolMap(_DEF_PROTOCOL_TCP_ADD_FRIEND_RQ) = &CKernel::dealAddFriendRq;
    NetProtocolMap(_DEF_PROTOCOL_TCP_ADD_FRIEND_RS) = &CKernel::dealAddFriendRs;
    NetProtocolMap(_DEF_PROTOCOL_TCP_OFFLINE_RQ) = &CKernel::dealOfflineRq;

}


//�������н��յ�������
void CKernel::dealData(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealData" << endl;
    //1.ȡ��Э��ͷ
    int type = *(int*)buf;

    //2.����Э��ͷ�ж����ĸ��ṹ�壬�߶�Ӧ�Ĵ�������
    //2.1���ж�Э��ͷ�Ƿ�����Ч��Χ��
    if ((type - _DEF_PROTOCOL_BASE - 10) >= 0 &&
        (type - _DEF_PROTOCOL_BASE - 10) < _DEF_TCP_PROTOCOL_COUNT) {
        //2.2ͨ�����������±꣬ȡ��������ַ
        pfun pf = NetProtocolMap(type);
        //2.3���ú���
        if (pf) {
            (this->*pf)(lSendIp, buf, nLen);
        }
        else {
            cout << "type:" << type << endl;
        }
    }
    //3.���������ݣ����տռ�
    delete[] buf;
}

//����ע������
void CKernel::dealRegisterRq(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealRegisterRq" << endl;
    //1.���
    STRU_REGISTER_RQ* rq = (STRU_REGISTER_RQ*)buf;
    cout << "tel:" << rq->tel << ",name:" << rq->name << ",password:" << rq->password << endl;

    //2.У���û������Ƿ�Ϸ���У�������ͻ���һ��
    //����Ϊ�յ�����������������˷�ð�ķǷ��������Կͻ��˺ͷ�����������У�飩--����˲�д��

    STRU_REGISTER_RS rs;
    //3.���ݵ绰�����ѯ���ݿ����Ƿ��Ѿ����������
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "select tel from t_user where tel = '%s';", rq->tel);
    if (!sql.SelectMySql(sqlBuf,1,resultList)) {
        cout << "��ѯ���ݿ�ʧ��,sql:" << sqlBuf << endl;
        return;
    }
    //4.������ѯ���
    if (resultList.size() > 0) {
        //�Ѿ�����ʹ�ù�����绰����ע���ˣ�����ע��ظ�������
        rs.result = tel_is_exist;
    }
    else {
        //û����ʹ������绰����
        //5.�����ǳƲ������ݿ����Ƿ��Ѿ����������
        sprintf_s(sqlBuf, "select name from t_user where name = '%s';", rq->name);
        if (!sql.SelectMySql(sqlBuf, 1, resultList)) {
            cout << "��ѯ���ݿ�ʧ��,sql:" << sqlBuf << endl;
            return;
        }
        //6.������ѯ���
        if (resultList.size() > 0) {
            //���ǳ��Ѿ�������ʹ���ˣ�����ע��ظ�������
            rs.result = name_is_exist;
        }
        else {
            //�ֻ��ź��ǳƶ�û�б�ע���������ע��
            //7.������˵�ע����Ϣд�����ݿ�
            sprintf_s(sqlBuf, "insert into t_user(tel,name,password,icon,feeling)values('%s','%s','%s',1,'ɶҲûд');",
                    rq->tel,rq->name,rq->password);
            if (!sql.UpdateMySql(sqlBuf)) {
                cout << "�������ݿ�ʧ��,sql:" << sqlBuf << endl;
            }
            //8.����ע��ظ�����ɹ�
            rs.result = register_success;
        }
    }
    //9.�ظ��ͻ���ע��ظ�
    m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));
}

//�����¼����
void CKernel::dealLoginRq(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealLoginRq" << endl;
    //1.���
    STRU_LOGIN_RQ* rq = (STRU_LOGIN_RQ*)buf;

    //2.У���û������Ƿ�Ϸ���У�������ͻ���һ��
    //����Ϊ�յ�����������������˷�ð�ķǷ��������Կͻ��˺ͷ�����������У�飩--����˲�д��

    //3.����tel������
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "select id,password from t_user where tel = '%s';", rq->tel);
    if (!sql.SelectMySql(sqlBuf, 2, resultList)) {
        cout << "��ѯ���ݿ�ʧ��,sql:" << sqlBuf << endl;
        return;
    }

    STRU_LOGIN_RS rs;
    //4.�����ѯ���Ϊ��
    if (0==resultList.size()) {
        //�û������ڣ����õ�¼�ظ����Ϊʧ��
        rs.result = user_not_exist;
        //7.�ظ����ͻ���һ����¼�ظ�
        m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));
    }else {
        //5.�����ѯ�����Ϊ�գ�ȡ��������û�id������
        int userId = atoi(resultList.front().c_str());
        resultList.pop_front();

        string pas = resultList.front();
        resultList.pop_front();

        //6.���û����������Ƚϣ��Ƿ���ͬ
        if (0==strcmp(rq->password,pas.c_str())) {
            //������ͬ����¼�ɹ�
            rs.result = login_success;
            rs.userId = userId;

            //���浱ǰ��¼�ɹ��û���socket
            m_mapIdToSocket[userId] = lSendIp;

            //7.�ظ����ͻ���һ����¼�ظ�
            m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));

            //��ѯ��ǰ��¼�û��ĺ�����Ϣ�������Լ�����Ϣ��
            getUserList(userId);
            
        }else {
            //���벻ͬ����¼ʧ��
            rs.result = password_error;

            //7.�ظ����ͻ���һ����¼�ظ�
            m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));
        }
    } 


}

//��ѯ��ǰ��¼�û��ĺ�����Ϣ�������Լ���
void CKernel::getUserList(int userId) {
    //1.��ѯ�Լ�����Ϣ
    STRU_FRIEND_INFO userInfo;
    getUserInfoById(&userInfo, userId);

    //2.��userIdȡ��socket�����Լ�����Ϣ�����ͻ���
    if (m_mapIdToSocket.find(userId)== m_mapIdToSocket.end()) {
        cout << "CKernel::getUserList��ȡsocketʧ�ܣ�userId=" << userId << endl;
        return;
    }
    m_pMediator->SendData(m_mapIdToSocket[userId],(char*)&userInfo,sizeof(userInfo));
    //3.�Ȳ������Щ����
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "select idB from t_friend where idA = '%d';",userId);
    if (!sql.SelectMySql(sqlBuf, 1, resultList)) {
        cout << "��ѯ���ݿ�ʧ��,sql:" << sqlBuf << endl;
        return;
    }
    //4.���ݺ��ѵ�id�б���ѯ��ÿ�����ѵ���Ϣ
    int friendId = 0;
    STRU_FRIEND_INFO friendInfo;
    while (resultList.size() > 0) {
        //5.ȡ�����ѵ�id
        friendId = atoi(resultList.front().c_str());
        resultList.pop_front();

        //6.���ݺ��ѵ�id��ѯ���ѵ���Ϣ
        getUserInfoById(&friendInfo, friendId);

        //7.�Ѻ��ѵ���Ϣ������¼�ͻ���
        m_pMediator->SendData(m_mapIdToSocket[userId], (char*)&friendInfo, sizeof(friendInfo));

        //8.�жϺ����Ƿ�����
        if (m_mapIdToSocket.find(friendId) != m_mapIdToSocket.end()) {
            //9.�������ߣ������ѷ��͵�¼�˵���Ϣ
            m_pMediator->SendData(m_mapIdToSocket[friendId], (char*)&userInfo, sizeof(userInfo));
        }
    }
}

//�����û�id��ѯ�û���Ϣ(userInfo���������)
void CKernel::getUserInfoById(STRU_FRIEND_INFO* userInfo, int userId) {
    
    //1.�����û�id
    userInfo->userId = userId;

    //2.�����û�״̬
    if (m_mapIdToSocket.find(userId)!= m_mapIdToSocket.end()){
        userInfo->state = statue_online;
    }
    else {
        userInfo->state = statue_offline;
    }

    //3.�����ݿ��ѯ�û���������Ϣ
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "select name,icon,feeling from t_user where id = '%d';", userId);
    if (!sql.SelectMySql(sqlBuf, 3, resultList)) {
        cout << "��ѯ���ݿ�ʧ��,sql:" << sqlBuf << endl;
        return;
    }

    //4.��ȡ��ѯ��������õ��ṹ����
    if (3 == resultList.size()) {
        //��ȡ����
        strcpy_s(userInfo->name, resultList.front().c_str());
        resultList.pop_front();
        //��ȥicon
        userInfo->iconId = atoi(resultList.front().c_str());
        resultList.pop_front();
        //���ȡǩ��
        strcpy_s(userInfo->feeling, resultList.front().c_str());
        resultList.pop_front();
    }
}

//������������
void CKernel::dealChatRq(long lSendIp, char* buf, int nLen) {
    cout << "CKernel::dealChatRq" << endl;
    //1.���
    STRU_TCP_CHAT_RQ* rq = (STRU_TCP_CHAT_RQ*)buf;
    cout << "userid:" << rq->userId << ",friendid:" << rq->friendId << ",content:" << rq->content;

    //2.�鿴�����Ƿ�����
    if (m_mapIdToSocket.count(rq->friendId)>0) {
        //�������ߣ��Ͱ���������ת��������
        m_pMediator->SendData(m_mapIdToSocket[rq->friendId], buf, nLen);
    }
    else {
        //���Ѳ����ߣ��ظ����Ͷ˺��Ѳ�����
        STRU_TCP_CHAT_RS rs;
        rs.result = send_fail;
        rs.userId = rq->userId;
        rs.friendId = rq->friendId;
        m_pMediator->SendData(lSendIp,(char*)&rs,sizeof(rs));
    }
}

//������Ӻ�������
void CKernel::dealAddFriendRq(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealAddFriendRq" << endl;
    //1.���
    STRU_ADD_FRIEND_RQ* rq = (STRU_ADD_FRIEND_RQ*)buf;

    //2.�鿴���ݿ�������û���������ӵ��û�
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "select id from t_user where name = '%s';", rq->friendName);
    if (!sql.SelectMySql(sqlBuf, 1, resultList)) {
        cout << "��ѯ���ݿ�ʧ�ܣ�sql:" << sqlBuf << endl;
        return;
    }
    STRU_ADD_FRIEND_RS rs;
    if (resultList.size() > 0) {
        //������������ӵ��û���ȡ��������û���id
        int friendId = atoi(resultList.front().c_str());
        resultList.pop_front();

        //3.�鿴������û��Ƿ�����
        if (m_mapIdToSocket.count(friendId) > 0) {
            //4.���������û����ߣ��Ͱ���Ӻ��ѵ�����ת����������û�
            m_pMediator->SendData(m_mapIdToSocket[friendId], buf, nLen);
        }
        else {
            //5.���������û�������,���ظ��ͻ�����Ӻ���ʧ�ܣ�ԭ������
            rs.result = user_offline;
            rs.friendId = friendId;
            strcpy_s(rs.friendName, rq->friendName);
            rs.userId = rq->userId;
            m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));
        }
    }
    else {
    //6.û�����������û������ظ��ͻ�����Ӻ���ʧ�ܣ�ԭ���û�������
        rs.result = no_this_user;
        strcpy_s(rs.friendName, rq->friendName);
        rs.userId = rq->userId;
        m_pMediator->SendData(lSendIp, (char*)&rs, sizeof(rs));
    }
}

//������Ӻ��ѻظ�
void CKernel::dealAddFriendRs(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealAddFriendRs" << endl;
    //1.���
    STRU_ADD_FRIEND_RS* rs = (STRU_ADD_FRIEND_RS*)buf;

    //2.����Է�ͬ����Ӻ��ѣ��ͰѺ��ѹ�ϵд�����ݿ�(д�����Σ�Ϊ�˱�֤���ɹ������߶�ʧ�ܣ���Ҫʹ������)
    if (add_success == rs->result) {
        char sqlBuf[1024] = "";
        sprintf_s(sqlBuf, "insert into t_friend values ('%d','%d');", rs->userId, rs->friendId);
        if (!sql.UpdateMySql(sqlBuf)) {
            cout << "�������ݿ�ʧ�ܣ�sql:" << sqlBuf << endl;
            return;
        }
        sprintf_s(sqlBuf, "insert into t_friend values ('%d','%d');", rs->friendId, rs->userId);
        if (!sql.UpdateMySql(sqlBuf)) {
            cout << "�������ݿ�ʧ�ܣ�sql:" << sqlBuf << endl;
            return;
        }
        //���º����б�
        getUserList(rs->friendId);
    }

    //3.����ͬ�⻹�Ǿܾ���Ӻ��ѣ���Ҫ��A�ͻ���ת����Ӻ��ѻظ��Ľṹ��
    m_pMediator->SendData(m_mapIdToSocket[rs->userId], buf, nLen);
}

//������������
void CKernel::dealOfflineRq(long lSendIp, char* buf, int nLen)
{
    cout << "CKernel::dealOfflineRq" << endl;
    //1.���
    STRU_TCP_OFFLINE_RQ* rq = (STRU_TCP_OFFLINE_RQ*)buf;

    //2.��ѯ�����û��ĺ���id�б�
    list<string>resultList;
    char sqlBuf[1024] = "";
    sprintf_s(sqlBuf, "selsct idB from t_friend where idA = '%d';", rq->userId);
    if (!sql.SelectMySql(sqlBuf, 1, resultList)) {
        cout << "��ѯ���ݿ�ʧ�ܣ�sql:" << sqlBuf << endl;
        return;
    }

    //3.��������id�б�
    int friendId = 0;
    while (resultList.size() > 0) {
    //ȡ������id
        friendId = atoi(resultList.front().c_str());
        resultList.pop_front();

        //4.����������ߣ�������ת����������
        if (m_mapIdToSocket.count(friendId) > 0) {
            m_pMediator->SendData(m_mapIdToSocket[friendId], buf, nLen);
        }
    }

    //5,�������û���socket�رգ�����map���Ƴ�
    if (m_mapIdToSocket.count(rq->userId) > 0) {
        closesocket(m_mapIdToSocket[rq->userId]);
        m_mapIdToSocket.erase(rq->userId);
    }
}