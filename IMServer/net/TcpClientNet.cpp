#include"TcpClientNet.h"
#include"PackDef.h"
#include<process.h>
#include"TcpClientMediator.h"

TcpClientNet::TcpClientNet(INetMediator* pMediator):m_sock(INVALID_SOCKET),m_handle(NULL),m_isStop(false)
{
	m_pMediator = pMediator;
}
TcpClientNet::~TcpClientNet()
{
	UninitNet();
}
//��ʼ������:���ؿ⡢�����׽��֡����ӷ���ˡ�����һ���������ݵ��߳�
bool TcpClientNet::InitNet()
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

    /* Confirm that the WinSock DLL supports 2.2.*/
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */

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
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == m_sock) {
        cout << "socket error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "socket success" << endl;
    }

    //3.���ӷ�����
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_DEF_TCP_SERVER_PORT);
    serverAddr.sin_addr.S_un.S_addr = inet_addr(_DEF_TCP_SERVER_IP);
    err = connect(m_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (SOCKET_ERROR == err) {
        cout << "connect error:" << WSAGetLastError() << endl;
        return false;
    }
    else {
        cout << "connect success" << endl;
    }

    //4.����һ���������ݵ��߳�
    //CreateThread��ExitThead��һ��ʹ�õģ�������߳���ʹ����C++����ʱ��ĺ�����
    // ����strcpy������һ���ռ䣬����ʹ��ExitThread�˳��̵߳�ʱ�򡣲����������Ŀռ䣬�ͻ���ɿռ�й¶��
    //_beginthreadex��_endtgreadexһ��ʹ�õģ�_endtgreadex���ڽ����̵߳�ʱ���Ȼ��տռ䣬�ٵ���ExitThread
    m_handle = (HANDLE)_beginthreadex(NULL, 0, &RecvThread, this, 0, NULL);

	return true;
}

//�������ݵ��̺߳���
unsigned __stdcall TcpClientNet::RecvThread(void* lpVoid)
{
    TcpClientNet* pThis = (TcpClientNet*)lpVoid;
    pThis->RecvData();
    return 0;
}

//�ر�����:�ر��׽��֡�ж�ؿ⡢�����߳�+���վ����Ϊ�˻����ں˶������ü�����Ϊ0����
void TcpClientNet::UninitNet()
{
    //1.�ر��׽���
    if (m_sock&& INVALID_SOCKET != m_sock)
    {
        closesocket(m_sock);
    }
    //2.ж�ؿ�
    WSACleanup();
    //3.���ŵĽ����̹߳���(��־λ�ó�false���ȴ��߳��Լ��������������һ��ʱ���ڣ��̲߳��ܽ�����������ǿ��ɱ���߳�)
    m_isStop = true;
    if (m_handle) {
        if (WAIT_TIMEOUT == WaitForSingleObject(m_handle, 100)) {
            //��100msʱ���ڣ��߳�û���˳�����ǿ��ɱ��
            TerminateThread(m_handle, -1);
        }
        //4.���վ��
        CloseHandle(m_handle);
        m_handle = NULL;
    }
}
//��������
bool TcpClientNet::SendData(long lSendIp, char* buf, int nLen)
{
    //1.У�����
    if (!buf||nLen<=0) {
        cout << "TcpClientNet::SendData parameter error" << endl;
        return false;
    }
    //2.�ȷ�����С
    if (send(m_sock,(char*)&nLen,sizeof(int),0)<=0) {
        return false;
    }
    //3.�ٷ�������
    if (send(m_sock,buf,nLen,0 ) <= 0) {
        return false;
    }
	return true;
}

//��������
void TcpClientNet::RecvData()
{
    int packSize = 0;
    int nRecvNum = 0;
    int offset = 0;
    while (!m_isStop) {
        //�Ƚ��հ���С
        nRecvNum = recv(m_sock,(char*)&packSize,sizeof(int),0);
        if (nRecvNum > 0) {//������ճɹ�
            //newһ���µĿռ������հ�����
            char* packBuf = new char[packSize];
            //�ٽ��հ�����
            while (packSize) {
                nRecvNum = recv(m_sock, packBuf + offset, packSize, 0);
                offset += nRecvNum;
                packSize -= nRecvNum;
            }
            //�ѽ��յ������ݴ����н�����
            m_pMediator->DealData(m_sock, packBuf, offset);
            //offset��Ҫ�����Ա���һ��ʹ��
            offset = 0;
        }
        else {
            cout << "TcpClientNet::RecvData recv error:" << WSAGetLastError() << endl;
            break;
        }
    }
}