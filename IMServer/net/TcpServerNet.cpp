#include"TcpServerNet.h"
#include"TcpServerMediator.h"
#include"PackDef.h"
#include<process.h>

TcpServerNet::TcpServerNet(INetMediator* pMediator): m_sockListen(INVALID_SOCKET),  m_isStop(false)
{
	m_pMediator = pMediator;
}
TcpServerNet::~TcpServerNet()
{
    UninitNet();
}
//��ʼ������:���ؿ⡢�����׽��֡���ip��ַ������������һ���������ӵ��߳�
bool TcpServerNet::InitNet() 
{
    //1.ѡ��Ŀ--���ؿ�WSAStartup()
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        return false;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        return false;
    }
    else {
        printf("The Winsock 2.2 dll was found okay\n");
    }
    //2.�Ͱ���--�����׽���socket()
    m_sockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == m_sockListen) {
        cout << "socket error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "socket success" << endl;
    }

    //3.ѡ��ַ--��ip��ַbind()
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_DEF_TCP_SERVER_PORT);//htons--ת���������ֽ���Ҳ����ת���ɴ�˴洢��ʽ
    //ʮ����4�ȷ��ַ������͵�ip��ַ��ת����ulong���͵�ip��ַ��ʹ��inet_addr()
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;//�������Ҫ����������
    err = bind(m_sockListen, (sockaddr*)&serverAddr, sizeof(serverAddr));

    if (SOCKET_ERROR == err) {
        cout << "bind error:" << WSAGetLastError() << endl;
        return false; 
    }
    else {
        cout << "bind success" << endl;
    }

    //4.����
    err = listen(m_sockListen, 10);
    if (SOCKET_ERROR == err) {
        cout << "listen error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "listen success" << endl;
    }

	//5.����һ���������ӵ��߳�
    //CreateThread��ExitThead��һ��ʹ�õģ�������߳���ʹ����C++����ʱ��ĺ�����
    // ����strcpy������һ���ռ䣬����ʹ��ExitThread�˳��̵߳�ʱ�򡣲����������Ŀռ䣬�ͻ���ɿռ�й¶��
    //_beginthreadex��_endtgreadexһ��ʹ�õģ�_endtgreadex���ڽ����̵߳�ʱ���Ȼ��տռ䣬�ٵ���ExitThread
    HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &AcceptThread, this, 0, NULL);
    if (handle) {
        m_listThreadHandle.push_back(handle);
    }
	return true;
}

//�������ӵ��̺߳���:ѭ���ȴ����ܿͻ��˵�����
unsigned __stdcall TcpServerNet::AcceptThread(void* lpVoid) 
{
    TcpServerNet* pThis = (TcpServerNet*)lpVoid;
    sockaddr_in addrClient;
    int addrClientSize = sizeof(addrClient);//���봫������
    while (!pThis->m_isStop) {
        //��������
        SOCKET sockClient = accept(pThis->m_sockListen,(sockaddr*)&addrClient,&addrClientSize);
        //��ӡ���ӳɹ��Ŀͻ��˵�ip��ַ��ֻ�д�ӡ��ip��ַ����˵���ͻ��˺ͷ�������ӳɹ��ˣ�
        cout << "client ip:" << inet_ntoa(addrClient.sin_addr) << "connect success" << endl;

        //����һ�����ӳɹ��Ŀͻ��˶�Ӧ�Ľ������ݵ��߳�
        unsigned int threadId = 0;
        HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, pThis, 0, &threadId);

        //������ͻ��˶�Ӧ��socket�ŵ�map�й���
        pThis->m_mapThreadIdToSocket[threadId] = sockClient;
        //���̵߳ľ������list�������
        if (handle) {
            pThis->m_listThreadHandle.push_back(handle);
        }
    }
    return 0;
}

//�������ݵ��̺߳���
unsigned __stdcall TcpServerNet::RecvThread(void* lpVoid)
{
    TcpServerNet* pThis = (TcpServerNet*)lpVoid;
    pThis->RecvData();
    return 0;
}

//�ر�����:�˳��̡߳��رվ�����ر�socket��ж�ؿ�
void TcpServerNet::UninitNet()
{
    //1.�˳��߳�
    m_isStop = true;
    for (auto ite = m_listThreadHandle.begin(); ite != m_listThreadHandle.end();){
        if (*ite) {
            if (WAIT_TIMEOUT == WaitForSingleObject(*ite, 100)) {
                //��100msʱ���ڣ��߳�û���˳�����ǿ��ɱ��
                TerminateThread(*ite, -1);
            }
            //2.���վ��
            CloseHandle(*ite);
            *ite = NULL;
        }
    ite = m_listThreadHandle.erase(ite); //�Ƴ���Ч�ڵ㣬����ֵ����һ����Ч�ڵ�
}

    //3.�ر�socket
    if (m_sockListen && INVALID_SOCKET != m_sockListen) {
        closesocket(m_sockListen);
    }
    for (auto ite = m_mapThreadIdToSocket.begin(); ite != m_mapThreadIdToSocket.end();) {
        if (ite->second && INVALID_SOCKET != ite->second) {
            closesocket(ite->second);
        }
        ite = m_mapThreadIdToSocket.erase(ite);
    }
    //4.ж�ؿ�
    WSACleanup();
}
//��������
bool TcpServerNet::SendData(long lSendIp, char* buf, int nLen)
{
    //1.У�����
    if (!buf || nLen <= 0) {
        cout << "TcpSeverNet::SendData parameter error" << endl;
        return false;
    }
    //2.�ȷ�����С
    if (send(lSendIp, (char*)&nLen, sizeof(int), 0) <= 0) {
        return false;
    }
    //3.�ٷ�������
    if (send(lSendIp, buf, nLen, 0) <= 0) {
        return false;
    }
	return true;
}

//��������
void TcpServerNet::RecvData()
{
    //��Ϊ�̴߳��������У�����socket����map����Ҫһ��ʱ�䣬�����߳���������һ��
    Sleep(100);
    //1.��ȡ��ǰ�̶߳�Ӧ��socket
    //��ȡ�߳�id
    unsigned int threadId = GetCurrentThreadId();
    SOCKET socket = m_mapThreadIdToSocket[threadId];
    //�ж�socket�ĺϷ���
    if (!socket || INVALID_SOCKET == socket) {
        cout << "socket ���Ϸ�" << endl;
        return;
    }
    int packSize = 0;
    int nRecvNum = 0;
    int offset = 0;
    while (!m_isStop) {
        //�Ƚ��հ���С
        nRecvNum = recv(socket, (char*)&packSize, sizeof(int), 0);
        if (nRecvNum > 0) {//������ճɹ�
            //newһ���µĿռ������հ�����
            char* packBuf = new char[packSize];
            //�ٽ��հ�����
            while (packSize) {
                nRecvNum = recv(socket, packBuf + offset, packSize, 0);
                offset += nRecvNum;
                packSize -= nRecvNum;
            }
            //�ѽ��յ������ݴ����н�����
            m_pMediator->DealData(socket, packBuf, offset);
            //offset��Ҫ�����Ա���һ��ʹ��
            offset = 0;
        }
        else if (10054 == WSAGetLastError()) {
            break;
        }
        else {
            cout << "TcpSeverNet::RecvData recv error:" << WSAGetLastError() << endl;
            break;
        }
    }
}